#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <bitset>
#include <string>
#include<cstring>
#include "Hash.h"
#define TABLE_SIZE 1048576 //32768*4*8


class BloomFilter{
    std::bitset<TABLE_SIZE> bittable;
    uint32_t hash_out[7];
    uint32_t elementNum;
public:
    BloomFilter();
    bool add(std::string key);
    bool Hash(std::string key);
    bool contains(std::string key);
    uint32_t getElementNum();
};


#endif