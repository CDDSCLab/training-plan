#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <bitset>
#include "hash.h"
#include <string>
#define BIT_SIZE  249997

using namespace std;

class bloomfilter{
private:
    bitset<BIT_SIZE> bst;
    Hash hash;
public:
    bloomfilter(){};
    int gethash(string& data, const int num);
    void add(string& str);
    bool isContain(string& str);
};

#endif // BLOOMFILTER_H