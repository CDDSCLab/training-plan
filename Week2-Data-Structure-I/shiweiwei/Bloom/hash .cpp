#include "hash.h"
#define M 249997

using namespace std;

// BKDR Hash Function 
unsigned int Hash::BKDRHash(const string& str)
{
    unsigned int seed=131 ;// 31 131 1313 13131 131313 etc.. 
    unsigned int hash=0 ;
       
    for (int i = 0; i < str.length(); i++)
    {
        hash = hash*seed+ str.at(i);
    }
       
    return(hash % M);
}

// AP Hash Function 
unsigned int Hash::APHash(const string& str)
{
    unsigned int hash=0 ;
       
    for(int i=0; i < str.length(); i++)
    {
        if((i&1)==0)
        {
            hash^=((hash<<7)^(str.at(i))^(hash>>3));
        }
        else
        {
            hash^=(~((hash<<11)^(str.at(i))^(hash>>5)));
        }
    }
       
    return(hash % M);
}

// RS Hash Function 
unsigned int Hash::RSHash(const string& str)
{
    unsigned int b=378551 ;
    unsigned int a=63689 ;
    unsigned int hash=0 ;
       
    for (int i = 0; i < str.length(); i++)
    {
        hash=hash*a+str.at(i);
        a*=b ;
    }
       
    return(hash % M);
}
   
// JS Hash Function 
unsigned int Hash::JSHash(const string& str)
{
    unsigned int hash=1315423911 ;
       
    for (int i = 0; i < str.length(); i++)
    {
        hash^=((hash<<5)+str.at(i)+(hash>>2));
    }
       
    return(hash % M);
}

// SDBM Hash Function 
unsigned int Hash::SDBMHash(const string& str)
{
    unsigned int hash=0 ;
       
    for (int i = 0; i < str.length(); i++)
    {
        hash=str.at(i)+(hash<<6)+(hash<<16)-hash ;
    }
       
    return(hash % M);
}
   
// DJB Hash Function 
unsigned int Hash::DJBHash(const string& str)
{
    unsigned int hash=5381 ;
       
    for (int i = 0; i < str.length(); i++)
    {
        hash+=(hash<<5)+str.at(i);
    }
       
    return(hash % M);
}

unsigned int Hash::DEKHash(const string& str)
{
    unsigned int hash = str.length();
    for (int i = 0; i < str.length() ; i++) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (str.at(i));
    }
    return (hash % M);
}

unsigned int Hash::MurmurHash(const string& str)
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    const int seed = 97;
    int len = str.length();
    unsigned int h = seed ^ len;
    const unsigned char *data = (const unsigned char *)(str.c_str());
    while(len >= 4)
    {
        unsigned int k = *(unsigned int *)data;
        k *= m; 
        k ^= k >> r; 
        k *= m; 
        h *= m; 
        h ^= k;
        data += 4;
        len -= 4;
    }
    switch(len)
    {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
        h *= m;
    };
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return (h % M);
}