#ifndef HASHS_H
#define HASHS_H

// using namespace std; 

#include <string>
#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;


string intToString(const int &i);


//hash_method:dazons hashs
class hash_method{
    public:
        // unsigned int choosehash(int,string);
        
        //additional hash
        unsigned int RSHash(string str,int bs_size);
        unsigned int SDBMHash(string str,int bs_size);
        unsigned int JSHash(string str,int bs_size);
        unsigned int DJBHash(string str,int bs_size);

        unsigned int PJWHash(string str,int bs_size);
        unsigned int ELFHash(string str,int bs_size);
        unsigned int BKDRHash(string str, int bs_size);
        unsigned int APHash(string str,int bs_size);
};

//hashchooser: choose one hash_method by switch
class hashchooser{
    public:
        unsigned int gethash(int i,string str,int bs_size);
        unsigned int cuckoo_hash(string str,int bucket_size);
        unsigned int fingerprint(string str);
};

#endif
