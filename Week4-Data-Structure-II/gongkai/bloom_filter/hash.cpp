//
// Created by Administrator on 2020/7/28 0028.
//

#include "hash.h"
#define M 65535
using namespace std;
unsigned int hash::RSHash(const char *str)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash %M);
}

unsigned int hash::JSHash(const char *str)
{
    unsigned int hash = 1315423911;

    while (*str)
    {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash  %M);
}

unsigned int hash::BKDRHash(const char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash  %M);
}
/*
unsigned int hash::Hash(char *str)
{
    unsigned int hash = 0;

    while (*str)
    {
// equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}
*/
unsigned int hash::PJWHash(const char *str)
{
    unsigned int BitsInUnignedInt=(unsigned int)(sizeof(unsigned int)*8);
    unsigned int ThreeQuarters=(unsigned int)((BitsInUnignedInt*3)/4);
    unsigned int OneEighth=(unsigned int)(BitsInUnignedInt/8);
    unsigned int HighBits=(unsigned int)(0xFFFFFFFF)<<(BitsInUnignedInt-OneEighth);
    unsigned int hash=0 ;
    unsigned int test=0 ;
    while(*str)
    {
        hash=(hash<<OneEighth)+(*str++);
        if((test=hash&HighBits)!=0)
        {
            hash=((hash^(test>>ThreeQuarters))&(~HighBits));
        }
    }
    return (hash %M);
}

unsigned int hash::APHash(const char *str) {
    unsigned int hash=0 ;
    int i ;
    for(i=0;*str;i++)
    {
        if((i&1)==0)
        {
            hash^=((hash<<7)^(*str++)^(hash>>3));
        }
        else
        {
            hash^=(~((hash<<11)^(*str++)^(hash>>5)));
        }
    }
    return (hash %M);
}