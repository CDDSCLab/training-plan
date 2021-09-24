include_directories(${CMAKE_SOURCE_DIR}/include)
set(BLOOM_SRC bitmap.cc bloomfilter.cc murmur3.c)
add_library(bloom SHARED ${BLOOM_SRC})