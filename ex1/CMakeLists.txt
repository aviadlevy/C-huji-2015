cmake_minimum_required(VERSION 2.8.4)
project(ex1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES shift.c)
add_executable(ex1 ${SOURCE_FILES})
add_definitions(-std=c99)