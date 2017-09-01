/*
 * 
 * 
 * 
 */
#include <thread>
#include <stdlib.h>
#include "simple_protocol.hpp"
#include "server.hpp"

void usage(const std::string &usg)
{
    errno = EINVAL;
    perror("Error");
    std::cout << "Usage: \n" << usg << std::endl;
    exit (errno);
}

int main(int ac, char* av[]) {

    std::string usg = std::string(av[0]) + " <port>";
    
     if (ac != 2) {
      usage(usg);
      }
 
    int port = atoi(av[1]);
    
    std::unique_ptr<Server> connection(new Server(port));

    
    std::unique_ptr<IProtocol> srv_proto(new SimpleProtocol());
    
    connection->setProtocol(std::move(srv_proto));
    connection->run();

    return 0;
}
