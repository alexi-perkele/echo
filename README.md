Linux and C++ based multyprotocol echo server with following data processing protocol:

1)Expecting string of digits and letters on input(not more than 60kb). 
2)Extract digits
4)Output sum of digits, digits in descending order, min and max values.

Server runs both TCP and UDP using same port number for both protocols. 

Building instractions:

1) Check out the project source code.
2) Go to desired build directory, or the source dir if you plan to do an in-source build.
3) Run:
$cmake path/to/sources
$make

Usage:
$echo_srv <port>
