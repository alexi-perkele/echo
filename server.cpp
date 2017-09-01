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
