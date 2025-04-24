cmake_minimum_required(VERSION 3.30)
project(cpp_ca3)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_ca3 main.cpp
        Crawler.cpp
        Crawler.h
        Board.cpp
        Board.h
        Bug.cpp
        Bug.h
        Hopper.cpp
        Hopper.h
)
