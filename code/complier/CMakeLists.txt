cmake_minimum_required(VERSION 3.14)
project(complier)

set(CMAKE_CXX_STANDARD 11)

add_executable(complier main.cpp tokenType/tokenType.h symbolTable/symbolTable.cpp symbolTable/symbolTable.h)