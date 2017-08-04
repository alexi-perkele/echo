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


#include "server.hpp"

Server::Server(const unsigned int &port, const int &type) : 
socketFd_(socket(PF_INET, type, 0)), 
ready_(false) {

  /* Initialize socket structure */
    bzero((char *) &sin_, sizeof( sin_ ));

    sin_.sin_addr.s_addr = INADDR_ANY;
    sin_.sin_port = htons(port);

  // clear sockaddr_in to sockaddr padding buffer
   memset(&sin_.sin_zero, 0, sizeof( sin_.sin_zero));

   if (bind(socketFd_, (struct sockaddr *) &sin_, sizeof( sin_ )) < 0) {
     std::stringstream msg;
     msg << std::to_string(type) << " Binding error" << std::endl;
     errno = EACCES;
     perror(msg.str().c_str());
     exit(errno);
  }
  else ready_ = true;
}

void  Server::setProtocol(std::unique_ptr<IProtocol> protocol) {
  server_protocol_ = std::move(protocol);
}

void Server::run() {
  std::cout << "Server runnn!!" << std::endl;
  
  if(!ready_) return;
  
  listen(socketFd_, 500);  //TODO: Mind backlog argument (max conn pending)
  
  char buf[64*1024];
  int ssock;
  unsigned int sin_len = sizeof(sin_);
  
  while(1)
  {
      ssock = accept(socketFd_, (struct sockaddr *)&sin_, &sin_len);
      
      if (ssock < 0) return;
      
      int q = read(ssock, buf, sizeof(buf));
      
      while (q>0) 
      {
          server_protocol_->process_data(buf);
          std::cout << *server_protocol_;
          bzero((char *) &buf, sizeof( buf ));
          q = read(ssock, buf, sizeof(buf));
          if (q ==0) break;
    }
       
   }
  
  std::cout << "Shutdown server" << std::endl;
  return;
}
Server::~Server() {
  if (socketFd_ > 0) {
    close(socketFd_);
  }

}

