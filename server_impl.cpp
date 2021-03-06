#include "functional"
#include "server.hpp"

Server::Server ( const unsigned int &port ) :
    port_(port),
    buf_sz_(64*1024),
    tcp_socketFd_ ( socket ( PF_INET, SOCK_STREAM, 0 ) ),
    udp_socketFd_ ( socket ( PF_INET, SOCK_DGRAM, 0 ) ),
    ready_ ( false )
    {
    }
    
void Server::init()
{
     int bind_tcp = passive_socket(tcp_socketFd_, tcp_sin_);
     int bind_udp = passive_socket(udp_socketFd_, udp_sin_);
     
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
}

int Server::passive_socket(const int& socket, sockaddr_in& sock_struct)
{
     /* Initialize socket structure */
    bzero ( ( char * ) &sock_struct, sizeof ( sock_struct ) );

    sock_struct.sin_addr.s_addr = INADDR_ANY;
    
    sock_struct.sin_port = htons(port_);
    
    // clear sockaddr_in to fit sockaddr padding buffer
    memset ( &sock_struct.sin_zero, 0, sizeof (sock_struct.sin_zero ));
    
    int ret = bind ( socket, (struct sockaddr*) &sock_struct, sizeof(sock_struct) );
    
    return ret;
}

void  Server::setProtocol ( IProtocol* protocol )
    {
    server_protocol_ = protocol;
    }

void Server::run()
    {
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
    std::cout << "Server ready" << std::endl;
    std::for_each(threads.begin(), threads.end(), 
                  std::mem_fn(&std::thread::join));
    
    std::cout << "Shutdown server" << std::endl;
    }

void Server::tcp_conn_handle()
    {
    int slave_sock;
    unsigned int sin_len = sizeof ( tcp_sin_ );

    while ( true )
        {
        slave_sock = accept ( tcp_socketFd_, (struct sockaddr*) &tcp_sin_, &sin_len );
        if ( slave_sock < 0 ) break;
   //TODO thread pool this:
        std::thread t ( &Server::tcp_conn_worker, this, std::cref(slave_sock) );
        t.detach();

        }
    }

void Server::tcp_conn_worker ( const int ssock )
    {
    std::vector<char> buf ( buf_sz_ );
    
    int read_msg_len = 10;
    while ( read_msg_len > 2 )
        {
        read_msg_len = read ( ssock, buf.data(), buf_sz_ );

        server_protocol_->process_data ( {std::begin ( buf ), std::end ( buf ) } );
        std::cout << *server_protocol_;

        int send_back_len = write ( ssock, buf.data(), read_msg_len );
        if ( send_back_len==-1 )  break;
        
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
        int data_read = recvfrom(udp_socketFd_, udp_buf.data(), buf_sz_, 0, (struct sockaddr*)&cli_addr, &cli_addrlen);
        if(data_read<2) continue;
        //TODO Threadpool this:
        std::thread(&Server::udp_worker, this, udp_buf, data_read, cli_addr).detach();
       
        udp_buf.resize ( buf_sz_ );
    }
    
}

    
void Server::udp_worker(std::vector<char> data, const int data_len, sockaddr_in cli)
{
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
    std::cout << "Closing connections" << std::endl;
    if ( tcp_socketFd_ > 0 )
        {
        close ( tcp_socketFd_ );
        }
    if ( udp_socketFd_ > 0 )
        {
        close ( udp_socketFd_ );
        }
    }
