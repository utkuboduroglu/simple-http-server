cmake_minimum_required(VERSION 3.17)
project(simplehttp)

set(CMAKE_CXX_STANDARD 20)

# some compiler flags for better practices
set(GCC_ADDITIONAL_FLAGS "-Wall -Wextra -Weffc++ -Wpedantic -pedantic-errors -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_FLAGS}")

# vcpkg package for boost::asio
include_directories("/home/utku/vcpkg/packages/boost-asio_x64-linux/include")

add_executable(simplehttp src/main.cpp src/addr_struct.h src/connection_thread.h)