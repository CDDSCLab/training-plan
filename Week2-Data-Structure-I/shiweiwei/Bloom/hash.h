#ifndef HASH_H
#define HASH_H

#include <string>
using namespace std;

class Hash {
public:
    static unsigned int BKDRHash(const string& str);
    static unsigned int APHash(const string& str);
    static unsigned int RSHash(const string& str);
    static unsigned int JSHash(const string& str);
    static unsigned int SDBMHash(const string& str);
    static unsigned int DJBHash(const string& str);
    static unsigned int DEKHash(const string& str);
    static unsigned int MurmurHash(const string& str);
};

#endif // HASH_H