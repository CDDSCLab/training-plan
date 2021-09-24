#include "cuckoo.h"

using namespace cuckoo;

Cuckoofilter::Cuckoofilter()
: _expo(16)
, _numKick(16)
, _bck()
{}

void Cuckoofilter::_encode(std::string str){
    // get fingerprint fp
    MD5 _md5(str);
    const byte *b = _md5.getDigest();
    memcpy(&fp, b, sizeof(fp));
    // get Hash1
    MurmurHash3_x64_128(str.c_str(), str.length(), _seed[0], hash1);
    // get Hash2 = Hash1 ^ hash(fp)
    unsigned int hashfp[4];
    fingerprint f = fp[0];
    MurmurHash3_x64_128(&f, sizeof(f), _seed[1], hashfp);
    for(int i = 0; i < 4; i++) hash2[i] = hash1[i] ^ hashfp[i];
}

int Cuckoofilter::_pickone(){
    unsigned int seed;
    seed = std::time(0);
    std::srand(seed);
    return (std::rand() % 2);
}

int Cuckoofilter::insert(std::string str){
    _encode(str);
    if(this->_bck.avalable(hash1, 0)){
        this->_bck.record(hash1, 0, fp[0]);
        return S_OK;
    }else if(this->_bck.avalable(hash2, 1)){
        this->_bck.record(hash2, 1, fp[0]);
        return S_OK;
    }
    int sb = _pickone();
    fingerprint cur_fp = fp[0];
    unsigned int *cur_index = (sb == 0?hash1:hash2);
    for(int n = 0; n < this->_numKick; n++){
        fingerprint f = this->_bck.fetch(cur_index, sb);
        std::swap(f, cur_fp);
        this->_bck.record(cur_index, sb, f);
        sb = 1 - sb;
        unsigned int hashfp[4];
        MurmurHash3_x64_128(&cur_fp, sizeof(cur_fp), _seed[1], hashfp);
        for(int i = 0; i < 4; i++) cur_index[i] = cur_index[i] ^ hashfp[i];
        if(this->_bck.avalable(cur_index, sb)){
            this->_bck.record(cur_index, sb, cur_fp);
            return S_OK;
        }
    }
    return E_OUTOFKICK;
}

bool Cuckoofilter::lookup(std::string str){
    _encode(str);
    fingerprint f = this->_bck.fetch(hash1, 0);
    bool b = this->_bck.avalable(hash1, 0);
    if(f == fp[0] && b == false) return true;
    f = this->_bck.fetch(hash2, 1);
    b = this->_bck.avalable(hash2, 1);
    if(f == fp[0] && b == false) return true;
    return false;
}

int Cuckoofilter::del(std::string str){
    _encode(str);
    fingerprint f = this->_bck.fetch(hash1, 0);
    if(f == fp[0]){
        this->_bck.del(hash1, 0);
        return S_OK;
    }
    f = this->_bck.fetch(hash2, 1);
    if(f == fp[0]){
        this->_bck.del(hash2, 1);
        return S_OK;
    }
    return E_NOT_FOUND;
}

int Cuckoofilter::getExpo(){
    return this->_expo;
}

int Cuckoofilter::getNumkick(){
    return this->_numKick;
}

Bucket Cuckoofilter::getBucket(){
    return this->_bck;
}