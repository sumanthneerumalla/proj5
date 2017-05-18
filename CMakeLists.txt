cmake_minimum_required(VERSION 3.6)
project(proj5)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES test1.cpp HashTable.cpp HashTable.h)
add_executable(proj5 ${SOURCE_FILES})