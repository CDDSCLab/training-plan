#include<iostream>
#include"bloomfilter.h"

BloomFilter bloom;

int main()
{
    std::string key;
    while(std::cin>>key){
        if(bloom.contains(key))
            std::cout<<"Contains!"<<std::endl;
        else{
            bloom.add(key);
            std::cout<<"Add!"<<std::endl;
        }   
    }
    return 0;
}