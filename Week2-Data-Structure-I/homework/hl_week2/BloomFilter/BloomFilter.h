#ifndef BloomFilter_H
#define BloomFilter_H
#include <cstddef>
#include <string>
#include "Hash.h"
#include <bitset>
#define N 100000
//N>>m

class BloomFilter
{
private:
    uint32_t n , m=100 , k=2;        //过滤器输入总数 ，哈希表长 ，哈希函数个数
    bitset<N> bitarray;       //位数组 用于实现hash
    Hash hash;
public:
    BloomFilter(/* args */);
    ~BloomFilter();
    BloomFilter(uint32_t n,uint32_t m);

    int Add(string str);         //向过滤器中添加
    int Lookup(string str);      //查询某数据是否在库中
};

BloomFilter::BloomFilter(uint32_t n,uint32_t m){
    this->k=k;
    this->hash = Hash(k);
    this->n=n;
    this->m=m;
}

BloomFilter::BloomFilter(/* args */){};

BloomFilter::~BloomFilter(){};

#endif