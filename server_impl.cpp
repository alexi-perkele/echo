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


#include <stdlib.h>
#include "Server.hpp"
#include <mutex>


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

  std::thread t(&Server::tcp_conn_handle, this);
   t.join();
/*
  while(1)
  {
      int ssock = accept(tcp_socketFd_, (struct sockaddr *)&tcp_sin_, &sin_len);
      
      if (ssock < 0) return;
      
      int q = read(ssock, buf, sizeof(buf));
      
      while (q>0) 
      {
          server_protocol_->process_data(buf);
          std::cout << *server_protocol_;
          bzero((char *) &buf, sizeof( buf ));
          q = read(ssock, buf, sizeof(buf));
          std::cout << "ECHO: " << buf << " " << q << std::endl;
          if (q == 0) {
                std::cout << "recv 0 " << buf << std::endl;
                break;
        }
            
          if (strcmp(buf, "q\n") == 0){ //FIXME: this is not work
              std::cout << "Q pressed " << buf << std::endl;
              break;
        } 
      }
     close(ssock); 
   }
   */
  std::cout << "Shutdown server" << std::endl;
}


void Server::tcp_conn_handle() {
  listen(tcp_socketFd_, 500);  //TODO: Mind backlog argument (max conn pending)

  const int buf_sz = 64*1024;
  char buf[buf_sz];
  int slave_sock;
  unsigned int sin_len = sizeof(tcp_sin_);

  while(1) {
     slave_sock = accept(tcp_socketFd_, (struct sockaddr *) &tcp_sin_, &sin_len);
    if (slave_sock <0)
      break;

  }

}

void Server::tcp_conn_worker() {

}


Server::~Server() {

  if (tcp_socketFd_ > 0) close(tcp_socketFd_);

  if (udp_socketFd_ > 0) close(udp_socketFd_);
}

