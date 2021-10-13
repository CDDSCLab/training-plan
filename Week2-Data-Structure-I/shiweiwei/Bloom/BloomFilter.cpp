#include "BloomFilter.h"
#define hash_num 8

int bloomfilter::gethash(string& data,const int num)
{
    int hashresult;
    switch(num)
    {
        case 0:
            hashresult = Hash::BKDRHash(data);
            break;
        case 1:    
            hashresult = Hash::APHash(data);
            break;
        case 2:
            hashresult = Hash::RSHash(data);
            break;
        case 3:
            hashresult = Hash::JSHash(data);
            break;
        case 4:
            hashresult = Hash::SDBMHash(data);
            break;
        case 5:
            hashresult = Hash::DJBHash(data);
            break;
        case 6:
            hashresult = Hash::DEKHash(data);
            break;
        case 7:
            hashresult = Hash::MurmurHash(data);
    }
    return hashresult;
}
void bloomfilter::add(string& str)
{
    for(int i=0; i<hash_num;i++){
        bst.set(gethash(str,i));
    }
}
bool bloomfilter::isContain(string& str)
{
    bool result = true;
    for(int i=0;i<hash_num;i++){
        result = result && bst.test(gethash(str,i));
    }
    return result;
}