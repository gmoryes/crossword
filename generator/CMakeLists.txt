cmake_minimum_required(VERSION 3.0)
project(crossword)

set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_subdirectory(src)

add_executable(crossword main.cpp)
target_link_libraries(crossword generator helper server)

