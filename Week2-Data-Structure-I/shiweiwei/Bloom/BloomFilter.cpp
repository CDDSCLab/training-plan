#include "BloomFilter.h"
#define hash_num 8

int bloomfilter::gethash(string& data, int num)
{
    int hashresult;
    switch(num)
    {
        case 0:
            hashresult = hash.BKDRHash(data);
            break;
        case 1:
            hashresult = hash.APHash(data);
            break;
        case 2:
            hashresult = hash.RSHash(data);
            break;
        case 3:
            hashresult = hash.JSHash(data);
            break;
        case 4:
            hashresult = hash.SDBMHash(data);
            break;
        case 5:
            hashresult = hash.DJBHash(data);
            break;
        case 6:
            hashresult = hash.DEKHash(data);
            break;
        case 7:
            hashresult = hash.MurmurHash(data);
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