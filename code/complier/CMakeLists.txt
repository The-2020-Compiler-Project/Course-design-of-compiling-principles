cmake_minimum_required(VERSION 3.14)
project(complier)

set(CMAKE_CXX_STANDARD 11)

add_executable(complier main.cpp tokenType/tokenType.h STable/STable.cpp STable/STable.h STable/Sheet.cpp STable/Sheet.h XTable/XTable.cpp XTable/XTable.h XTable/funSheet.cpp XTable/funSheet.h XTable/typeSheet.cpp XTable/typeSheet.h type/type.h)