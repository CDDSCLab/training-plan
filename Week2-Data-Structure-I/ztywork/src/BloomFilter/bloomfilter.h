#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <bitset>
#include <string>
#include<cstring>
#include "Hash.h"
#define TABLE_SIZE 800000


class BloomFilter{
    std::bitset<TABLE_SIZE> bittable;
    uint32_t hash_out[7];
public:
    BloomFilter();
    bool add(std::string key);
    bool Hash(std::string key);
    bool contains(std::string key);
};


#endif