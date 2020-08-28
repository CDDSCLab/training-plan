//
// Created by Administrator on 2020/7/29 0029.
//

#ifndef MYCUCKOOFILTER_CUCKOOFILTER_H
#define MYCUCKOOFILTER_CUCKOOFILTER_H

#include <vector>
#include <string>
#include <time.h>
#define BUCKET_SIZE (10)
#define MAXNUMKICKS (8)
class cuckoofilter {
public:
    cuckoofilter(){
        bucket.resize(BUCKET_SIZE,-1);
    };
    unsigned short fingerprint(std::string word);
    unsigned int hashfunction(std::string word);
    bool insert(std::string word);
    void printbucket();
    bool isInBucket(std::string word);
    bool Delete(std::string word);
private:
    std::vector<unsigned short> bucket;
};


#endif //MYCUCKOOFILTER_CUCKOOFILTER_H
