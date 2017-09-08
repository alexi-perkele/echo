/*
 * 
 * Server class providing two threads for UDP and TCP connections.
 * And easy replacable protocol for input data processing.
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
#include <future>
#include <utility>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <arpa/inet.h>
#include "iprotocol.hpp"
#include "thread_guard.hpp"


class ServerContext
    {
    protected:
        std::unique_ptr<IProtocol> server_protocol_;
    public:
        ServerContext() {};
        virtual ~ServerContext() {};
        virtual void setProtocol ( std::unique_ptr<IProtocol> ) = 0;
        ServerContext ( const ServerContext& ) = delete;
        ServerContext& operator= ( const ServerContext& ) = delete;   
    };

// An RAII armored base class for handling sockets.
class Server : public ServerContext
    {
        
    public:
        explicit Server ( const unsigned int& port);
        void setProtocol ( std::unique_ptr<IProtocol> protocol ) override;
        void run();
        ~Server();
    private:
        void init();
        void tcp_conn_handle();
        void tcp_conn_worker(const int& ssock);
        void udp_handler();
        void udp_worker(std::vector<char> data, const int& data_len, sockaddr_in cli);

        u_int16_t port_;
        const int buf_sz_;  
        int tcp_socketFd_;
        int udp_socketFd_;
        struct sockaddr_in tcp_sin_;
        struct sockaddr_in udp_sin_;
        bool ready_;    
    };



#endif // ECHOSERVER_H
