# ECHO client/server


Linux and C++ meets sockets handling to get multiprotocol, multithreaded echo server.

Server runs TCP and UDP using same port number for both protocols. 

# Building instractions:

1) Check out the project source code.
2) Go to desired build directory, or the source dir if you plan to do an in-source build.
3) Run:
$cmake path/to/sources
$make

Usage:
$echo_srv <port>
