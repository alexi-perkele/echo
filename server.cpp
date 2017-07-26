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
#include "simpleprotocol.hpp"
#include "server.hpp"

// TODO this is looks ugly. 
std::ostream &operator<<(std::ostream& os, const IProtocol &proto_obj) {
  return proto_obj.print(os);
}

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
    
    std::string test = "92qw1e8rt4t56";
    
    std::unique_ptr<Server> tcp_connection(new Server(port, SOCK_STREAM));
    std::unique_ptr<Server> udp_connection(new Server(port, SOCK_DGRAM));
    
    std::unique_ptr<IProtocol> srv_proto(new SimpleProtocol(test));
    
   //SimpleProtocol* sprt = new SimpleProtocol(test);
  //  IProtocol* srv_proto = sprt; //new SimpleProtocol(test);

    std::cout << *srv_proto;
    
    tcp_connection->setProtocol(std::move(srv_proto));
    tcp_connection->run();
 /*   
    if(!srv_proto->storage_empty())
    {
        srv_proto->process_data(); // TODO: coredumped if data not processed
       // std::cout << *srv_proto;
    }
    else std::cout << "nothing to process" << std::endl;
   */ 
    std::cout << "The end!" << std::endl;

    return 0;
}
