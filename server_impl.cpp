/*
 * Copyright (c) 2017 alexi kulmametiev@mail.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */


//#include <stdlib.h>
#include "server.hpp"



Server::Server(const unsigned int &port) :
    //server_protocol_(nullptr),
    tcp_socketFd_(socket(PF_INET, SOCK_STREAM, 0)),
    udp_socketFd_(socket(PF_INET, SOCK_DGRAM, 0)),
    ready_(false)
{

  /* Initialize socket structure */
    bzero((char *) &tcp_sin_, sizeof(tcp_sin_));
    bzero((char *) &udp_sin_, sizeof(udp_sin_));

    tcp_sin_.sin_addr.s_addr = INADDR_ANY;
    tcp_sin_.sin_port = htons(port);

  // clear sockaddr_in to sockaddr padding buffer
   memset(&tcp_sin_.sin_zero, 0, sizeof( tcp_sin_.sin_zero));
   memset(&udp_sin_.sin_zero, 0, sizeof( udp_sin_.sin_zero));

  int bind_tcp = bind(tcp_socketFd_, (struct sockaddr *) &tcp_sin_, sizeof(tcp_sin_));
  std::cout << "bind to port errno: " << errno << std::endl;
  int bind_udp = bind(udp_socketFd_, (struct sockaddr *) &udp_sin_, sizeof(udp_sin_));
   if (bind_tcp && bind_udp != 0) {
     std::stringstream msg;
     msg << " Binding error" << std::endl;
     errno = EACCES;
     perror(msg.str().c_str());
     exit(errno);
  }
  else
   {
     std::cout << "listening port: " << port << std::endl;
     ready_ = true;
   }
}

void  Server::setProtocol(std::unique_ptr<IProtocol> protocol) {
  server_protocol_ = std::move(protocol);  //FIXME: // Disable copy from lvalue.
                                           //unique_ptr(const unique_ptr&) = delete;
                                           //unique_ptr& operator=(const unique_ptr&) = delete;
}

void foo(){
    std::cout <<"Hello foo!" << std::endl;
}

void Server::run() {
  std::cout << "Server runnn!!" << std::endl;
  
  if(!ready_) return;

    //TODO: Mind backlog argument (max conn pending)
  int lstn =  listen(tcp_socketFd_, 500);
  if ( errno != 0)
    {
      perror("Error");
      std::cout << "socket failed!!" << std::endl;
      return;
    }
  std::cout << "passive tcp(listen) errno: " << lstn << std::endl;
 // tcp_conn_handle();
  std::thread t(&Server::tcp_conn_handle, this);
  t.join();

  std::cout << "Shutdown server" << std::endl;
}


void Server::tcp_conn_handle() {
  std::cout << "handler run!" << std::endl;

  int slave_sock;
  unsigned int sin_len = sizeof(tcp_sin_);

  while(1) {
     std::cout << "handler loop" << std::endl;
     slave_sock = accept(tcp_socketFd_, (struct sockaddr *) &tcp_sin_, &sin_len);
     std::cout << "accepted socket: " << slave_sock << " errno: " << errno << std::endl;
    if (slave_sock < 0)
    {
        break;
    }
      
    std::thread t(&Server::tcp_conn_worker, this, slave_sock);
    t.detach();

    std::cout << "worker finished" << std::endl;
  }

}

void Server::tcp_conn_worker(const int& ssock) {
  std::cout << "Worker! capacity: " << buf_sz_/sizeof(char) << std::endl;
  std::vector<char> buf(buf_sz_/sizeof(char));
  std::cout << "buf capacity " << buf.capacity() << " size: " << buf.size() << std::endl;
  
  int q = read(ssock, buf.data(), buf_sz_);
  std::cout << "Worker read bytes: " << q << " errno: " << errno << std::endl;
  while (q!=2)
  {
    server_protocol_->process_data({std::begin(buf), std::end(buf)});
    std::cout << *server_protocol_;
    buf.clear();
    buf.resize(buf_sz_/sizeof(char));
    std::cout << "buffer flushed"  << std::endl;
    std::cout << "buf capacity " << buf.capacity() << " size: " << buf.size() << std::endl;
    q = read(ssock, buf.data(), buf_sz_);
    
    std::cout<<std::endl;
    std::cout << "Worker read bytes: " << q << std::endl;
    if (q == 2) {
      break;
    }
  }

  close(ssock);
}


Server::~Server() {
  std::cout << "Server destructor" << std::endl;
  if (tcp_socketFd_ > 0) 
  {
      std::cout << "closing tcp" << std::endl;
      close(tcp_socketFd_);
    }
  if (udp_socketFd_ > 0) 
  {
      std::cout << "closing udp" << std::endl;
      close(udp_socketFd_);
    }
      
}

