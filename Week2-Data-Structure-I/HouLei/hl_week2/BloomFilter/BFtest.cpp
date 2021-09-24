#include <iostream>
#include "BloomFilter.h"
#include "Hash.h"
#include "BloomFilter.cpp"
#include "Hash.cpp"
#include <vector>
#include "MurMurHash.h"
#include "MurMurHash.cpp"

using namespace std;

vector<string> Rd_generate_string(uint32_t num)
{
    int i=0;
    vector<string> rd_string;
    for (;i<num;i++)
    {
        int k=rand()%100+1;//随机生成一个字符串的长度
        string ss="";
        for(int i=0;i<k;i++)
        {
            int s;
            char x;
            s=rand()%2;
            if(s==1)
                x=rand()%('Z'-'A'+1)+'A';
            else
                x=rand()%('z'-'a'+1)+'a';
            ss.push_back(x);
        }
        rd_string.push_back(ss);
    }
    return rd_string;
};


int main()
{
    clock_t start, finish;
    double duration;    
    BloomFilter bloomfilter(1000,4000);
    vector<string> rdstring = Rd_generate_string(1000);
    int i=0;
    float count=0,p_rate;
    start = clock();
    for(;i<rdstring.size();i++)
    {
        if(bloomfilter.Lookup(rdstring[i]))
        {
            count=count+1;
        }else bloomfilter.Add(rdstring[i]);
    }
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    p_rate=count/rdstring.size();
    cout<<"误报率:"<<p_rate<<endl;
    cout<<"耗时:"<<duration<<"s"<<endl;
    return 0;
}

