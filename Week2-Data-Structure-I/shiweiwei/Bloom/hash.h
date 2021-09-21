#ifndef HASH_H
#define HASH_H

#include <string>
using namespace std;

class Hash {
public:
    unsigned int BKDRHash(string& str);
    unsigned int APHash(string& str);
    unsigned int RSHash(string& str);
    unsigned int JSHash(string& str);
    unsigned int SDBMHash(string& str);
    unsigned int DJBHash(string& str);
    unsigned int DEKHash(string& str);
    unsigned int MurmurHash(string& str);
};

#endif // HASH_H