cmake_minimum_required(VERSION 3.17)
project(simplehttp)

set(CMAKE_CXX_STANDARD 20)

set(GCC_ADDITIONAL_FLAGS "-Wall -Wextra -Weffc++ -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_FLAGS}")

add_executable(simplehttp src/main.cpp src/addr_struct.h src/connection_thread.h)