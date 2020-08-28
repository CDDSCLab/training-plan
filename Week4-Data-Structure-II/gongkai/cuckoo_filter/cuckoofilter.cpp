//
// Created by Administrator on 2020/7/29 0029.
//

#include "cuckoofilter.h"
#include <iostream>

using namespace std;

unsigned short cuckoofilter::fingerprint(std::string word) {
    int seed = 293;
    unsigned long hash = 0;
    for (int i = 0; i < word.length(); i++)
        hash = (hash * seed) + word[i];
    return (unsigned short) (hash % 65535);
}

unsigned int cuckoofilter::hashfunction(std::string word) {
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    const char *str = word.c_str();
    while (*str) {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash % BUCKET_SIZE);
}

bool cuckoofilter::insert(std::string word) {
    unsigned short fingerPrint = fingerprint(word);
    unsigned int i1 = hashfunction(word);
    unsigned int i2 = i1 ^hashfunction(to_string(fingerPrint));
    if (bucket[i1] == 65535) {
        bucket[i1] = fingerPrint;
        return true;
    } else if (bucket[i2] == 65535) {
        bucket[i2] = fingerPrint;
        return true;
    }
    srand((unsigned int) time(NULL));
    int random = rand() % 2;
    unsigned int index = (random == 0 ? i1 : i2);
    for (int n = 0; n < MAXNUMKICKS; n++) {
        unsigned short temp = bucket[index];
        bucket[index] = fingerPrint;
        fingerPrint = temp;
        index = index ^ hashfunction(to_string(fingerPrint));
        if (bucket[index] == 65535) {
            bucket[index] = fingerPrint;
            return true;
        }

        }

    cout << "false" << endl;
    return false;
}

void cuckoofilter::printbucket() {
    for (int i = 0; i < bucket.size(); i++)
        cout << bucket[i] << endl;
}

bool cuckoofilter::isInBucket(std::string word) {
    unsigned short fingerPrint = fingerprint(word);
    unsigned int i1 = hashfunction(word);
    unsigned int i2 = i2 = i1 ^ hashfunction(to_string(fingerPrint));
    if (bucket[i1] == fingerPrint || bucket[i2] == fingerPrint)
        return true;
    else
        return false;
}


bool cuckoofilter::Delete(std::string word) {
    unsigned short fingerPrint = fingerprint(word);
    unsigned int i1 = hashfunction(word);
    unsigned int i2 = i1 ^hashfunction(to_string(fingerPrint));
    if (bucket[i1] == fingerPrint) {
        bucket[i1] = 65535;
        return true;
    } else if (bucket[i2] == fingerPrint) {
        bucket[i2] = 65535;
        return true;
    } else
        return false;
}





