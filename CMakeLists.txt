cmake_minimum_required(VERSION 3.5)
project(pocket_calculator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    pc.cpp)

add_executable(pocket_calculator ${SOURCE_FILES})