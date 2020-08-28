//
// Created by Administrator on 2020/7/28 0028.
//

#include "bloomfilter.h"
void bloomfilter::bitset_insert(std::string& str) {
    bs.set(hashset.APHash(str.c_str()));
    bs.set(hashset.RSHash(str.c_str()));
    bs.set(hashset.BKDRHash(str.c_str()));
    bs.set(hashset.JSHash(str.c_str()));
    bs.set(hashset.PJWHash(str.c_str()));
}
bool bloomfilter::isin(std::string &str) {
    if(!(bs.test(hashset.APHash(str.c_str()))))
        return false;
    if(!(bs.test(hashset.RSHash(str.c_str()))))
        return false;
    if(!(bs.test(hashset.BKDRHash(str.c_str()))))
        return false;
    if(!(bs.test(hashset.JSHash(str.c_str()))))
        return false;
    if(!(bs.test(hashset.PJWHash(str.c_str()))))
        return false;
    return true;
}