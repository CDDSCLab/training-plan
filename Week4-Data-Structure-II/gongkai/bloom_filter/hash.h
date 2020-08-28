//
// Created by Administrator on 2020/7/28 0028.
//

#ifndef BLOOM_FILTER_HASH_H
#define BLOOM_FILTER_HASH_H


class hash {
public:
    hash(){};
    unsigned int RSHash(const char *str);
    unsigned int JSHash(const char *str);
    unsigned int BKDRHash(const char *str);
    unsigned int PJWHash(const char *str);
    unsigned int APHash(const char *str);
};


#endif //BLOOM_FILTER_HASH_H
