cmake_minimum_required(VERSION 3.6)
project(../../drivr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/cpp/main.cpp)
add_executable(drivr ${SOURCE_FILES})
