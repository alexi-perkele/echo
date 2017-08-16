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

#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <utility>
#include <thread>
#include "IProtocol.hpp"


class ServerContext
    {
    protected:
        std::unique_ptr<IProtocol> server_protocol_;
    public:
        ServerContext() {};
        virtual ~ServerContext() {};
        virtual void setProtocol ( std::unique_ptr<IProtocol> ) = 0;
    private:
        // Non copyable:
        ServerContext ( const ServerContext& );
        ServerContext& operator= ( const ServerContext& );       
    };

// An RAII armored base class for handling sockets.
class Server : public ServerContext
    {

    public:
        Server ( const unsigned int& port);
        ~Server();
        void setProtocol ( std::unique_ptr<IProtocol> protocol ) override;
        void run();

   //     void tcp_listen();
        void tcp_conn_handle();
        void tcp_conn_worker();

    private:
        int tcp_socketFd_;
        int udp_socketFd_;
        // socket exists and binded to port.
        struct sockaddr_in tcp_sin_;
        struct sockaddr_in udp_sin_;
        bool ready_;
      
    };



#endif // ECHOSERVER_H
