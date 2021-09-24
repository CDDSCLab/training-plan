#ifndef _BUCKET_H_
#define _BUCKET_H_

#include <vector>

#define S_OK 0
#define E_OUTOFKICK -1
#define E_NOT_FOUND -2

typedef unsigned int fingerprint;

namespace cuckoo{

    struct container{
        unsigned char _occupy;
        fingerprint _fp[2];
        container();
    };

    class Bucket{
        int _expo;
        std::vector<container> *_bucket;
        unsigned int _applyMask(unsigned int *index);
    public:
        Bucket();
        bool avalable(unsigned int *index, int standby);
        int record(unsigned int *index, int standby, fingerprint fp);
        fingerprint fetch(unsigned int *index, int standby);
        int del(unsigned int *index, int standby);
        int getExpo();
        std::vector<container>* getBucket();
        ~Bucket() = default;
    };

}   // namespace cuckoo


#endif