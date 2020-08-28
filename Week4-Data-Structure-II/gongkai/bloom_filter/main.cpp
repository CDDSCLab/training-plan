#include <iostream>
#include "bloomfilter.h"
#include "hash.h"
#include <vector>
int main() {
    bloomfilter bf;
    std::vector<std::string> teststr={
            "hello worid",
            "worid",
            "rid",
            "hello ",
            "rder positio",
            "d from the right",
            "Returns whethe",
            "signed integra",
            "the bit at",
            "position pos",
            "at position",
    };
    for(std::vector<std::string>::size_type i=0;i<teststr.size();i++){
        std::cout<<bf.isin(teststr[i])<<std::endl;
    }
    for(std::vector<std::string>::size_type i=0;i<teststr.size();i++){
        bf.bitset_insert(teststr[i]);
    }
    for(std::vector<std::string>::size_type i=0;i<teststr.size();i++){
        std::cout<<bf.isin(teststr[i])<<std::endl;
    }
    return 0;
}
