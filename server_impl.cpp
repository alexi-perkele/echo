#include "server.hpp"



Server::Server ( const unsigned int &port ) :
    port_(port),
    tcp_socketFd_ ( socket ( PF_INET, SOCK_STREAM, 0 ) ),
    udp_socketFd_ ( socket ( PF_INET, SOCK_DGRAM, 0 ) ),
    ready_ ( false )
    {   
    }
    
void Server::init()
{
    std::cout << "init server" << std::endl;
     /* Initialize socket structure */
    bzero ( ( char * ) &tcp_sin_, sizeof ( tcp_sin_ ) );
    bzero ( ( char * ) &udp_sin_, sizeof ( udp_sin_ ) );

    tcp_sin_.sin_addr.s_addr = INADDR_ANY;
    udp_sin_.sin_addr.s_addr = INADDR_ANY;
    
    udp_sin_.sin_port = htons(port_);
    tcp_sin_.sin_port = htons (port_);

    // clear sockaddr_in to sockaddr padding buffer
    memset ( &tcp_sin_.sin_zero, 0, sizeof ( tcp_sin_.sin_zero ) );
    memset ( &udp_sin_.sin_zero, 0, sizeof ( udp_sin_.sin_zero ) );

    int bind_tcp = bind ( tcp_socketFd_, ( struct sockaddr * ) &tcp_sin_, sizeof ( tcp_sin_ ) );
    int bind_udp = bind ( udp_socketFd_, ( struct sockaddr * ) &udp_sin_, sizeof ( udp_sin_ ) );
    
    if ( bind_tcp && bind_udp != 0 )
        {
        std::stringstream msg;
        msg << " Binding error" << std::endl;
        errno = EACCES;
        perror ( msg.str().c_str() );
        exit ( errno );
        }
    else
        {
        ready_ = true;
        }
    std::cout << "init finished" << std::endl;
}


void  Server::setProtocol ( std::unique_ptr<IProtocol> protocol )
    {
    server_protocol_ = std::move ( protocol ); //FIXME: // Disable copy from lvalue.
    }

void Server::run()
    {
    std::cout << "Server starting" << std::endl;
    
    Server::init();

    if ( !ready_ ) return;

    //TODO: Mind backlog argument (max conn pending)
    listen ( tcp_socketFd_, 500 );
    if ( errno != 0 )
        {
        perror ( "Error" );
        std::cout << "socket failed!!" << std::endl;
        return;
        }

    std::vector<std::thread> threads;
    
    threads.push_back(std::thread(&Server::tcp_conn_handle, this));
    threads.push_back(std::thread(&Server::udp_handler, this));
    
    std::for_each(threads.begin(), threads.end(), 
                  std::mem_fn(&std::thread::join));
    
    std::cout << "Shutdown server" << std::endl;
    }


void Server::tcp_conn_handle()
    {
        std::cout << "tcp handle" << std::endl;
    int slave_sock;
    unsigned int sin_len = sizeof ( tcp_sin_ );

    while ( true )
        {
        slave_sock = accept ( tcp_socketFd_, ( struct sockaddr * ) &tcp_sin_, &sin_len );
        std::cout << "slave sock: " << slave_sock << std::endl;
        if ( slave_sock < 0 ) break;
        
        std::async( &Server::tcp_conn_worker, this, std::cref(slave_sock));
    //    std::thread t ( &Server::tcp_conn_worker, this, std::cref(slave_sock) );
    //    t.detach();

        }
    }

void Server::tcp_conn_worker ( const int& ssock )
    {
    std::vector<char> buf ( buf_sz_ );
    
    int q = 10;
    while ( q > 1 )
        {
        q = read ( ssock, buf.data(), buf_sz_ );
        if (q<2) break; //FIXME

        server_protocol_->process_data ( {std::begin ( buf ), std::end ( buf ) } );
        std::cout << *server_protocol_;

        int k = write ( ssock, buf.data(), q );
        if ( k==-1 )  break;
        
        buf.clear();
        buf.resize ( buf_sz_ );
        }

    close ( ssock );
    }

    
void Server::udp_handler()
{
    std::vector<char> udp_buf( buf_sz_ );
    
    std::shared_ptr<std::vector<char>> buf (new std::vector<char>(buf_sz_));
    sockaddr_in cli_addr;
    unsigned int cli_addrlen = sizeof(cli_addr);
    while(true){
        int k = recvfrom(udp_socketFd_, udp_buf.data(), buf_sz_, 0, (struct sockaddr*)&cli_addr, &cli_addrlen);
        if(k<2) continue;
      
        std::thread(&Server::udp_worker, this, udp_buf, std::cref(k), cli_addr).detach();
       
        udp_buf.resize ( buf_sz_ );
    }
    
}

    
void Server::udp_worker(std::vector<char> data, const int& data_len, sockaddr_in cli)
{
     std::cout << "udp worker" << std::endl;
     server_protocol_->process_data ( {std::begin(data), std::end(data)} );
     std::cout << *server_protocol_;
     
     int k = sendto(udp_socketFd_, data.data(), data_len, 0, (struct sockaddr*)&cli, sizeof(cli));
     if (k <= 0)
     {
         std::cout << "udp connection lost: " << inet_ntoa(cli.sin_addr) << std::endl;
    }
}


Server::~Server()
    {
    std::cout << "Server destructor" << std::endl;
    if ( tcp_socketFd_ > 0 )
        {
        std::cout << "closing tcp" << std::endl;
        close ( tcp_socketFd_ );
        }
    if ( udp_socketFd_ > 0 )
        {
        std::cout << "closing udp" << std::endl;
        close ( udp_socketFd_ );
        }
    }
