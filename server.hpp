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


class ServerContext
    {
    protected:
        IProtocol* server_protocol_;
    public:
        ServerContext() {};
        virtual ~ServerContext() {};
        virtual void setProtocol ( IProtocol* protocol) = 0;
        ServerContext ( const ServerContext& ) = delete;
        ServerContext& operator= ( const ServerContext& ) = delete;   
    };

// An RAII armored base class for handling sockets.
class Server : public ServerContext
    {
        
    public:
        explicit Server ( const unsigned int& port);
        void setProtocol ( IProtocol* protocol ) override;
        void run();
        ~Server();
    private:
        void init();
        int passive_socket(const int& socket, sockaddr_in& sock_struct);
        void tcp_conn_handle();
        void tcp_conn_worker(const int ssock);
        void udp_handler();
        void udp_worker(std::vector<char> data, const int data_len, sockaddr_in cli);
        
        u_int16_t port_;
        const int buf_sz_;  
        int tcp_socketFd_;
        int udp_socketFd_;
        struct sockaddr_in tcp_sin_;
        struct sockaddr_in udp_sin_;
        bool ready_;    
    };



#endif // ECHOSERVER_H
