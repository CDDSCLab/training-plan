#ifndef _CUCKOO_H_
#define _CUCKOO_H_
#include <string>
#include "bucket.h"
#include "md5.h"
#include "murmur3.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace cuckoo{

    class Cuckoofilter{
        int _expo;
        int _numKick;
        Bucket _bck;
        std::vector<int> _seed = {11,23};
        void _encode(std::string str);
        int _pickone();
        unsigned int fp[4];
        unsigned int hash1[4];
        unsigned int hash2[4];
    public:
        Cuckoofilter();
        int insert(std::string str);
        bool lookup(std::string str);
        int del(std::string str);
        int getExpo();
        int getNumkick();
        Bucket getBucket();
        ~Cuckoofilter() = default;
    };

} // namespace cuckoo

#endif