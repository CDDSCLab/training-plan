#include "bucket.h"
using namespace cuckoo;

container::container()
: _occupy(0)
, _fp{0}{}

Bucket::Bucket()
: _expo(16)
, _bucket(new std::vector<container>(1<<16, container()))
{}

unsigned int Bucket::_applyMask(unsigned int *index){
    unsigned int mask = 1;
    mask = (mask << _expo) - 1;
    unsigned int ind = index[3] & mask;
    return ind;
}

// standby: {0 - 首选位, 1 - 次选位}
bool Bucket::avalable(unsigned int *index, int standby){
    unsigned int i = _applyMask(index);
    return (( ( (*this->_bucket) [i]._occupy) & (0x01 << standby)) == 0);
}

int Bucket::record(unsigned int *index, int standby, fingerprint fp){
    unsigned int i = _applyMask(index);
    (*this->_bucket)[i]._occupy |= 0x01 << standby;
    (*this->_bucket)[i]._fp[standby] = fp;
    return S_OK;
}

fingerprint Bucket::fetch(unsigned int *index, int standby){
    unsigned int i = _applyMask(index);
    return (*this->_bucket)[i]._fp[standby];
}

int Bucket::del(unsigned int *index, int standby){
    unsigned int i = _applyMask(index);
    (*this->_bucket)[i]._occupy &= ~(0x01 << standby);
    return S_OK;
}

int Bucket::getExpo(){
    return this->_expo;
}

std::vector<container>* Bucket::getBucket(){
    return this->_bucket;
}
