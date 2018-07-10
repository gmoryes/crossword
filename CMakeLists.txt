cmake_minimum_required(VERSION 3.0)
project(crossword)

set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_STANDARD 14)

include_directories(src)
set(SOURCE_FILE src/Generator/Generator.cpp)

add_executable(crossword main.cpp ${SOURCE_FILE})