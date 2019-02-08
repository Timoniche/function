cmake_minimum_required(VERSION 3.13)
project(functional)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")

add_executable(Function
        main.cpp
        function.h
        test_loskutov.cpp
        my_base_test.cpp
        test_eugene.cpp
        )