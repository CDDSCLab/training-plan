include_directories(${CMAKE_SOURCE_DIR}/include)
set(CUCKOO_SRC bucket.cc md5.cc murmur3.c cuckoo.cc)
add_library(cuckoo SHARED ${CUCKOO_SRC})