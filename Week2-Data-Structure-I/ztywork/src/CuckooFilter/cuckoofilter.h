#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

#include "Hash.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>
#define BUCKET_NUM 32768
#define BUCKET_SIZE 4

typedef unsigned char bucket[BUCKET_SIZE];

class cuckoofilter{
private:
    bucket buckets[BUCKET_NUM];
    unsigned int element_num;
    unsigned int kick_max;
public:
    cuckoofilter();
    unsigned char fingerprint(std::string key);
    unsigned int position_now(std::string key);
    unsigned int position_dual(unsigned int now,unsigned char fp);
    bool contains(std::string key);
    bool deletes(std::string key);
    bool inserts(std::string key);
    unsigned int getElementNum();
};





#endif

