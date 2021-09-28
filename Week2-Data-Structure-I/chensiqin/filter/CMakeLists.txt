#CMakeLists.txt 
cmake_minimum_required(VERSION 3.0)
project(CMakeFilter)
 
# c++按照11标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE release)

set(SRC bloom_filter.cpp counting_bloom_filter.cpp cuckoo_filter.cpp ../hashs/hashs.cpp)

include_directories(../hashs)

add_compile_options(-wall -std=c++11 -o2)

aux_source_directory(. SRC)

add_executable(main ${SRC})