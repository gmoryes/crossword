cmake_minimum_required(VERSION 2.8.0)
project(crossword)

set(CMAKE_CXX_FLAGS "-g -pthread")
set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_subdirectory(src)

add_executable(crossword main.cpp)
target_link_libraries(crossword generator helper server)

