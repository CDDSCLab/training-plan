#ifndef _BLOOMFILTER_H_
#define _BLOOMFILTER_H_

#include <vector>
#include <string>
#include "bitmap.h"
#include "murmur3.h"
using namespace std;

namespace bloomfilter{

    class Bloomfilter{
    public:
        Bloomfilter();
        Bloomfilter(int expo);
        bool isContain(std::string obj);
        int add(std::string obj);
        int getExpo();
        Bitmap* getBitmap();
        ~Bloomfilter();
    private:
        int _expo;
        Bitmap *_bitmap;
        vector<int> seeds = {11, 13, 17, 19, 23, 31, 37, 41};
    };

}   // namespace bloomfilter


# endif