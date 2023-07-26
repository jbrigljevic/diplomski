cmake_minimum_required(VERSION 3.0.0)
project(MWIS VERSION 0.1.0)

add_executable(MWIS main.cpp graph.h graph.cpp)

target_compile_options(MWIS PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)