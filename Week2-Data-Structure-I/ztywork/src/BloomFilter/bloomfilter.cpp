#include"bloomfilter.h"


BloomFilter::BloomFilter(){
    bittable.reset();
    memset(hash_out, 0, 7*4);
}

bool BloomFilter::add(std::string key){
    if(!Hash(key)) 
        return false;
    if(contains(key))
        return false;
    for(uint32_t index:hash_out)
       bittable.set(index%TABLE_SIZE);
    return true; 
}

bool BloomFilter::Hash(std::string key){
    MurmurHash3_x86_32(key.c_str(),key.length(),101,&hash_out[0]);
    MurmurHash3_x86_32(key.c_str(),key.length(),213,&hash_out[1]);
    MurmurHash3_x86_32(key.c_str(),key.length(),344,&hash_out[2]);
    MurmurHash3_x86_32(key.c_str(),key.length(),452,&hash_out[3]);
    MurmurHash3_x86_32(key.c_str(),key.length(),598,&hash_out[4]);
    MurmurHash3_x86_32(key.c_str(),key.length(),641,&hash_out[5]);
    MurmurHash3_x86_32(key.c_str(),key.length(),702,&hash_out[6]);
    return true;
}
bool BloomFilter::contains(std::string key){
    if(!Hash(key)) 
        return false;
    for(uint32_t index:hash_out){
        if(!bittable.test(index%TABLE_SIZE))
            return false;
    }
    return true;
}

