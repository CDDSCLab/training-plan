#ifndef CuckooFilter_H
#define CuckooFilter_H
#include <iostream>
#include "Hash.h"

#define N 10000

using namespace std;

class CuckooFilter
{
private:
    uint8_t bucket[N]={0};
    uint32_t bucket_size;  //N>>>bucket_size    bucket_size = 2^n
    Hash hash;
    uint32_t conflict_count;
public:
    CuckooFilter(/* args */);
    ~CuckooFilter();
    CuckooFilter(uint32_t bszie);
    int Add(string str);
    int Lookup(string str);
    int Delete(string str);
    int Add_Conflict(uint32_t idx , uint8_t fingerprint);

};

CuckooFilter::CuckooFilter(/* args */)
{
    this->bucket_size = 32;
    this->hash = Hash(1);
    this->conflict_count = 0;
}

CuckooFilter::CuckooFilter(uint32_t bsize)
{
    this->bucket_size = bsize;
    this->hash = Hash(1);
    this->conflict_count = 0;
}

CuckooFilter::~CuckooFilter()
{
}


#endif