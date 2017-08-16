cmake_minimum_required(VERSION 3.3)
project(echo_cs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -pedantic-errors")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (PROTOCOL_FILES simple_protocol.cpp)
set(SOURCE_FILES main.cpp)
set(CLI_SOURCE_FILES client.cpp)
set(SRV_SOURCE_FILES server_impl.cpp server.cpp)

add_executable(echo_cli ${CLI_SOURCE_FILES})
add_executable(echo_srv ${SRV_SOURCE_FILES} ${PROTOCOL_FILES})

