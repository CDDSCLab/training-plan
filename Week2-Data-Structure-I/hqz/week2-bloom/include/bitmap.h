#ifndef _BITMAP_H_
#define _BITMAP_H_

#define S_OK 0
#include <vector>

namespace bloomfilter{

    class Bitmap{
    public: 
        Bitmap();
        Bitmap(int expo);
        int bit_set(unsigned int *index);
        int bit_reset(unsigned int *index);
        bool bit_query(unsigned int *index);
        // bool mapStatus(int expo, unsigned long map_size);
        int getExpo();
        std::vector<int>& getBit();
        ~Bitmap() = default;
    private: 
        unsigned int _applyMask(unsigned int *index);
        int _expo;
        std::vector<int> _bit;
    };

}   // namespace bloomfilter

#endif