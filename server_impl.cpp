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

Server::Server(const unsigned int &port, const int &type) : socketFd_(socket(PF_INET, type, 0)) {

  struct sockaddr_in sin;
  /* Initialize socket structure */
  bzero((char *) &sin, sizeof(sin));

  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);

  // clear sockaddr_in to sockaddr padding buffer
  memset(&sin.sin_zero, 0, sizeof(sin.sin_zero));

  if (bind(socketFd_, (struct sockaddr *) &sin, sizeof(sin)) < 0) {

    std::stringstream msg;
    msg << std::to_string(type) << " Binding error" << std::endl;
    errno = EACCES;
    perror(msg.str().c_str());
    exit(errno);

  }
}

void  Server::setProtocol(std::unique_ptr<IProtocol> protocol) {
  server_protocol_ = std::move(protocol);
}

void Server::run() {
  std::cout << "Server runnn!!" << std::endl;
  server_protocol_->process_data();
  std::cout << *server_protocol_;
  return;
}
Server::~Server() {
  if (socketFd_ > 0) {
    close(socketFd_);
  }

}

