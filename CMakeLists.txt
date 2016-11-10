cmake_minimum_required(VERSION 3.2)
project(pc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    pc.cpp)

add_executable(pc ${SOURCE_FILES})