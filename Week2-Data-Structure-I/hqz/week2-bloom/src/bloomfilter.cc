#include "bloomfilter.h"
using namespace bloomfilter;

Bloomfilter::Bloomfilter()
: _expo(20)
{
    _bitmap = new Bitmap();
}

Bloomfilter::Bloomfilter(int expo)
: _expo(expo)
{
    if(_expo<5 || _expo>32) _expo = 20;
    _bitmap = new Bitmap(_expo);
}

int Bloomfilter::add(std::string obj){
    unsigned int index[4];
    const char *obj_c = obj.c_str();
    for(auto &seed : seeds){
        MurmurHash3_x64_128(obj_c, obj.length(), seed, index);
        this->_bitmap->bit_set(index);
    }
    return S_OK;
}

bool Bloomfilter::isContain(std::string obj){
    unsigned int index[4];
    const char *obj_c = obj.c_str();
    int result = true;
    for(auto &seed : seeds){
        MurmurHash3_x64_128(obj_c, obj.length(), seed, index);
        if(!this->_bitmap->bit_query(index)){
            result = false;
            break;
        }
    }
    return result;
}

Bloomfilter::~Bloomfilter(){
    delete _bitmap;
    _bitmap = nullptr;
}

int Bloomfilter::getExpo(){ return this->_expo; }

Bitmap* Bloomfilter::getBitmap(){ return this->_bitmap; }