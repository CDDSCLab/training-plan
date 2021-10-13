#pragma once
#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

#include <string>
using namespace std;
//#define bucket_num 262144
//#define bucket_size 1
//#define max_kicks 10

class cuckoofilter {
    
private:
    enum {
        bucket_num = 262144,
        bucket_size = 1,
        max_kicks = 10
    };
    unsigned short bucket[bucket_num][bucket_size] = { };
public:
    cuckoofilter() {};
    unsigned int FingerPrint(const string& str);
    unsigned int MurmurHash(const string& str);
    bool add(const string& str);
    bool isContain(const string& str);
    bool Delete(const string& str);
};

#endif // CUCKOOFILTER_H