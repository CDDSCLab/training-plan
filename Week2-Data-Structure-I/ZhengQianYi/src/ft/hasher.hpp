#ifndef FT_HASHER_HPP
#define FT_HASHER_HPP

#include "hash/MurmurHash3.hpp"

#include <string>
#include <random>
#include <string.h>
namespace ft
{

    enum class HashType
    {
        murmur
    };



    size_t hashf(HashType name, size_t seed, std::string const &s)
    {
        
        const char *o = s.c_str();

        //std::default_random_engine e1;
        size_t result[2];
        MurmurHash3_x64_128(o, strlen(o), seed, result);
        return result[0];
    }

    size_t hashf(HashType name, size_t seed, size_t const &o)
    {
        //std::default_random_engine e1;
        size_t result[2];
        MurmurHash3_x64_128(&o, sizeof(o), seed, result);
        return result[0];
    }

} //name space ft

#endif