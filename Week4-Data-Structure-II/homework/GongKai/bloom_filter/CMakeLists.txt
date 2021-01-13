cmake_minimum_required(VERSION 3.16)
project(Bloom_Filter)

set(CMAKE_CXX_STANDARD 14)

add_executable(Bloom_Filter main.cpp hash.cpp hash.h bloomfilter.cpp bloomfilter.h)