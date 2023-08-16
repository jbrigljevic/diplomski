cmake_minimum_required(VERSION 3.5)
project(tree VERSION 0.1.0)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(tree graph.h graph.cpp algorithm.h algorithm.cpp main.cpp)