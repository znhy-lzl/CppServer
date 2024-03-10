cmake_minimum_required(VERSION 3.20.1)
project(CPPSERVER)
set(CMAKE_CXX_STANDARD 17)
add_executable(server server.cpp util.cpp)
