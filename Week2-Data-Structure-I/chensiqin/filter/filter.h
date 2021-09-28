#ifndef FITLER_H
#define FITLER_H
// #pragma once

#define MAX_BLOOM_BS_SIZE 20000
#define CUCKOO_BUCKET_SIZE 80
#define CUCKOO_BUCKET_NUM 256
#define MAX_CUCKOO_KICKS 200
#define ERROR_RATE 0.1

#include <string>
#include <bitset>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <time.h>
#include <iostream>
#include <cstring>


using std::string;
using std::vector;
using std::bitset;
using std::cout;
using std::endl;

class filter{
    public:
        virtual int insert(string key)=0;
        virtual int lookup(string key)=0;
        virtual int del(string key)=0;
};

class bloom_filter: public filter{
     private:
        int bs_size;
        int hash_size;
        bitset<MAX_BLOOM_BS_SIZE> bs;

    public:
        int insert(string key);
        int lookup(string key);
        int del(string key);
        void print();
        bloom_filter(int m, int k);
        bloom_filter(int n);  //可通过数据量n来构造bf，利用公式计算出m和k
};


class counting_bloom_filter: public filter{
     private:
        int bs_size;
        int hash_size;
        vector<int> bscount; 
    public:
        int insert(string key);
        int lookup(string key);
        int del(string key);
        void print();
        counting_bloom_filter(int m, int k);
        counting_bloom_filter(int n);
};

class cuckoo_filter: public filter{
     private:
        int bucket_size;
        int bucket_num;
        unsigned int bucket[CUCKOO_BUCKET_NUM][CUCKOO_BUCKET_SIZE];
        // vector<unsigned int[CUCKOO_BUCKET_SIZE]> bucket;
        
    public:
        int insert(string key);
        int lookup(string key);
        int del(string key);
        void print();
        cuckoo_filter(int m);
};

#endif