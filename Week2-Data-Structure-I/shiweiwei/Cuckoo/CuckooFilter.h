#pragma once
#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

#include <string>
using namespace std;
#define bucket_num 8629
#define bucket_size 1
#define max_kicks 10

class cuckoofilter {
private:
    int bucket[bucket_num][bucket_size] = { };
public:
    cuckoofilter() {};
    unsigned int FingerPrint(string& str);
    unsigned int MurmurHash(string& str);
    bool add(string& str);
    bool isContain(string& str);
    bool Delete(string& str);
};

#endif // CUCKOOFILTER_H