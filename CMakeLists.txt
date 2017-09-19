cmake_minimum_required(VERSION 3.3)
project(echo_cs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -pedantic-errors")

set (PROTOCOL_FILES simple_protocol.cpp)
set(SOURCE_FILES main.cpp)
#set(CLI_SOURCE_FILES client.cpp)   it's coding now. always. constantly in development
set(SRV_SOURCE_FILES server_impl.cpp server.cpp)

#add_executable(echo_cli ${CLI_SOURCE_FILES})
add_executable(echo_srv ${SRV_SOURCE_FILES} ${PROTOCOL_FILES})

