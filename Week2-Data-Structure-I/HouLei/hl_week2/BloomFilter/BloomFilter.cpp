#include <iostream>
#include "BloomFilter.h"
#include <cstddef>
#include <string>
#include "Hash.h"

using namespace std;


int BloomFilter::Add(string str){
    vector<uint32_t> hash_result = hash.Murmurhash(str);
    for(int i=0;i<hash_result.size();i++)
    {
        int set_b = hash_result[i]%m;
        cout<<set_b<<endl; 
        bitarray.set(set_b);
    }
    return 0;
}     //向过滤器中添加

int BloomFilter::Lookup(string str){
    vector<uint32_t> hash_result = hash.Murmurhash(str);
    for(int i=0;i<hash_result.size();i++)
    {
        if(!bitarray[hash_result[i]%m]) 
        {
        cout<<"["<<str<<"]"<<"不在库中"<<endl;
        return 0; //查询的字符串不在库中
        }
    }
    cout<<"["<<str<<"]"<<"可能在库中"<<endl;
    return 1;   //查询某数据可能在库中
}    



