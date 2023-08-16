cmake_minimum_required(VERSION 3.5)
project(MWIS VERSION 0.1.0)

# for optimization purposes
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -Wall -O3 -no-pie")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(MWIS graph.h graph.cpp solution.h solution.cpp
               ILS-VND.h ILS-VND.cpp file.h file.cpp test.h test.cpp)