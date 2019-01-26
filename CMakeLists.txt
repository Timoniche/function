cmake_minimum_required(VERSION 3.13)
project(functional)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(Function
        main.cpp
        function.h
        )