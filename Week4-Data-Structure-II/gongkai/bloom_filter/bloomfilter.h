//
// Created by Administrator on 2020/7/28 0028.
//

#ifndef BLOOM_FILTER_BLOOMFILTER_H
#define BLOOM_FILTER_BLOOMFILTER_H

#include <bitset>
#include "hash.h"
#include <string>
#define BIT_SIZE 65535
class bloomfilter {
private:
    std::bitset<BIT_SIZE> bs;
    hash hashset;
public:
    bloomfilter(){};
    void bitset_insert(std::string &str);
    bool isin(std::string &str);

};


#endif //BLOOM_FILTER_BLOOMFILTER_H
