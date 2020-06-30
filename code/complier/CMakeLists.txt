cmake_minimum_required(VERSION 3.14)
project(complier)

set(CMAKE_CXX_STANDARD 11)

add_executable(complier main.cpp tokenType/tokenType.h STable/STable.cpp STable/STable.h STable/Sheet.cpp STable/Sheet.h)