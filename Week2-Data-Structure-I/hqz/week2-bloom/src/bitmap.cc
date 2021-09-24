#include "bitmap.h"
using namespace bloomfilter;

Bitmap::Bitmap()
    : _expo(20)
    , _bit(1<<(_expo-5), 0)
    {}

    Bitmap::Bitmap(int expo)
    : _expo(expo)
    {
        if(_expo<5 || _expo>32) _expo = 20;
        _bit = std::vector<int>(1<<(_expo-5), 0);
    }

    unsigned int Bitmap::_applyMask(unsigned int *index){
        unsigned int mask = 1;
        mask = (mask << _expo) - 1;
        unsigned int ind = index[3] & mask;
        return ind;
    }

    int Bitmap::bit_set(unsigned int *index){
        int ind = _applyMask(index);
        unsigned int i = ind / 32;
        unsigned int j = ind % 32;
        _bit[i] |= (0x00000001 << j); 
        return S_OK;
    }

    int Bitmap::bit_reset(unsigned int *index){
        int ind = _applyMask(index);
        unsigned int i = ind / 32;
        unsigned int j = ind % 32;
        _bit[i] &= ~(0x00000001 << j); 
        return S_OK;
    }

    bool Bitmap::bit_query(unsigned int *index){
        int ind = _applyMask(index);
        unsigned int i = ind / 32;
        unsigned int j = ind % 32;
        unsigned int target_bit = (0x00000001 << j) & _bit[i];
        return (target_bit > 0);
    }

    // bool Bitmap::mapStatus(int expo, unsigned long map_size){
    //     bool result = true;
    //     if(expo != this->_expo) result = false;
    //     if(map_size != this->_bit.size()) result = false;
    //     return result;
    // }

    int Bitmap::getExpo(){
        return this->_expo;
    }

    std::vector<int>& Bitmap::getBit(){
        return this->_bit;
    }