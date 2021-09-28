// #pragma once
#include "hashs.h"

string intToString(const int&i){
    string s;
    stringstream ss;
    ss << i;
    ss >> s;
    return s;
}

//hash_method:many methods
//additional hash
//1. SDBMHash
unsigned int hash_method::SDBMHash(string str, int bs_size)
{
    unsigned int hash = 0;

    for (int i = 0; i < str.length(); i++)
    {
        hash = 65599 * hash + str[i];
    }
    return (hash % bs_size);
}

// 2. RS Hash Function
unsigned int hash_method::RSHash(string str, int bs_size)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    for (int i = 0; i < str.length(); i++)
    {
        hash = hash * a + str[i];
        a *= b;
    }
    return (hash % bs_size);
}

//3.  JS Hash Function
unsigned int hash_method::JSHash(string str, int bs_size)
{
    unsigned int hash = 1315423911;

    for (int i = 0; i < str.length(); i++)
    {
        hash ^= ((hash << 5) + str[i] + (hash >> 2));
    }
    return (hash % bs_size);
}

// 4. DJB Hash Function
unsigned int hash_method::DJBHash(string str, int bs_size)
{
    unsigned int hash = 5381;

    for (int i = 0; i < str.length(); i++)
    {
        hash += (hash << 5) + str[i];
    }
    return (hash % bs_size);
}

// 5. P. J. Weinberger Hash Function
unsigned int hash_method::PJWHash(string str, int bs_size)
{
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters = (unsigned int)((BitsInUnignedInt * 3) / 4);
    unsigned int OneEighth = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;

    for (int i = 0; i < str.length(); i++)
    {
        hash = (hash << OneEighth) + str[i];
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }
    return (hash % bs_size);
}

// 6. ELF Hash Function
unsigned int hash_method::ELFHash(string str, int bs_size)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    for (int i = 0; i < str.length(); i++)
    {
        hash = (hash << 4) + str[i];
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
    return (hash % bs_size);
}

// 7. BKDR Hash Function
unsigned int hash_method::BKDRHash(string str, int bs_size)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    for (int i = 0; i < str.length(); i++)
    {
        hash = hash * seed + str[i];
    }
    return (hash % bs_size);
}

// 8.AP Hash Function
unsigned int hash_method::APHash(string str, int bs_size)
{
    unsigned int hash = 0;
    int i;

    for (i = 0; str[i]; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ str[i] ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ str[i] ^ (hash >> 5)));
        }
    }
    return (hash % bs_size);
}

//hashchosser:add switch
unsigned int hashchooser::gethash(int i, string str, int bs_size)
{
    hash_method hashmethod;
    unsigned int hash;
    switch (i)
    {
    case 0 /* constant-expression */:
        /* code */
        hash = hashmethod.RSHash(str, bs_size);
        break;
    case 1:
        hash = hashmethod.SDBMHash(str, bs_size);
        break;
    case 2:
        hash = hashmethod.JSHash(str, bs_size);
        break;
    case 3:
        hash = hashmethod.DJBHash(str, bs_size);
        break;
    case 4:
        hash = hashmethod.PJWHash(str, bs_size);
        break;
    case 5:
        hash = hashmethod.ELFHash(str, bs_size);
        break;
    case 6:
        hash = hashmethod.BKDRHash(str, bs_size);
        break;
    case 7:
        hash = hashmethod.APHash(str, bs_size);
        break;
    default:
        hash = 0;
        break;
    }
    return hash;
};

//hashmethod for cuckoo
unsigned int hashchooser::cuckoo_hash(string str,int bucket_size)
{
    hash_method hashmethod;
    unsigned int hash;
    hash = hashmethod.SDBMHash(str, bucket_size);
    return hash;
};

unsigned int hashchooser::fingerprint(string str)
{
    hash_method hashmethod;
    unsigned int hash;
    hash = hashmethod.RSHash(str, 10000)+1;//取一个极大的数求模,并最小为1
    return hash;
}
