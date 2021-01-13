cmake_minimum_required(VERSION 3.16)
project(myCuckooFilter)

set(CMAKE_CXX_STANDARD 14)

add_executable(myCuckooFilter main.cpp cuckoofilter.cpp cuckoofilter.h)