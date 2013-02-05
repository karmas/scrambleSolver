cmake_minimum_required(VERSION 2.6)

set(SRC "main.cc")

set(CMAKE_CXX_FLAGS "-Wall -g")

add_executable(scrambleSolver ${SRC})
