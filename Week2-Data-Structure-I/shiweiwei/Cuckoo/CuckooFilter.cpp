#include "CuckooFilter.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime> 

using namespace std;

unsigned int cuckoofilter::FingerPrint(const string& str)
{
    unsigned int seed = 131;// 31 131 1313 13131 131313 etc.. 
    unsigned int hash = 0;

    for (int i = 0; i < str.length(); i++)
    {
        hash = hash * seed + str.at(i);
    }

    return(hash % 100 + 1);
}

unsigned int cuckoofilter::MurmurHash(const string& str)
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    const int seed = 97;
    int len = str.length();
    unsigned int h = seed ^ len;
    const unsigned char* data = (const unsigned char*)(str.c_str());
    while (len >= 4)
    {
        unsigned int k = *(unsigned int*)data;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }
    switch (len)
    {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
        h *= m;
    };
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return (h % bucket_num);
}
bool cuckoofilter::add(const string& str) {
    int finger = FingerPrint(str);
    string F = to_string(finger);
    int hash1 = MurmurHash(str);
    int hash2 = hash1 ^ MurmurHash(F);
    //�п�λ��
    for (int i = 0;i < bucket_size;i++) {
        if (bucket[hash1][i] == 0) {
            bucket[hash1][i] = finger;
            return true;
        }
    }
    for (int i = 0;i < bucket_size;i++) {
        if (bucket[hash2][i] == 0) {
            bucket[hash2][i] = finger;
            return true;
        }
    }
    //û�п�λ��
    srand((int)time(0));
    int hash = (rand() % 2 == 0) ? hash1 : hash2;
    for (int i = 0;i < max_kicks;i++) {
        int index = rand() % bucket_size;
        int kickedFinger = bucket[hash][index];
        bucket[hash][index] = finger;
        //Ϊ�߳�ȥ���ҵ��µ�λ��
        string F = to_string(kickedFinger);
        hash = hash ^ MurmurHash(F);
        for (int i = 0;i < bucket_size;i++) {
            if (bucket[hash][i] == 0) {
                bucket[hash][i] = kickedFinger;
                return true;
            }
        }
    }
    //��ʱ�ߵĴ����ﵽ��ֵ
    cout << "��������Ҫ��������" << endl;
    return true;
}
bool cuckoofilter::isContain(const string& str) {
    int finger = FingerPrint(str);
    string F = to_string(finger);
    int hash1 = MurmurHash(str);
    int hash2 = hash1 ^ MurmurHash(F);
    for (int i = 0;i < bucket_size;i++) {
        if (bucket[hash1][i] == finger) {
            return true;
        }
    }
    for (int i = 0;i < bucket_size;i++) {
        if (bucket[hash2][i] == finger) {
            return true;
        }
    }
    return false;
}
bool cuckoofilter::Delete(const string& str) {
    int finger = FingerPrint(str);
    string F = to_string(finger);
    int hash1 = MurmurHash(str);
    int hash2 = hash1 ^ MurmurHash(F);
    for (int i = 0;i < bucket_size;i++) {
        if (bucket[hash1][i] == finger) {
            bucket[hash1][i] = 0;
            return true;
        }
    }
    for (int i = 0;i < bucket_size;i++) {
        if (bucket[hash2][i] == finger) {
            bucket[hash2][i] = 0;
            return true;
        }
    }
    return false;
}