#include <iostream>
#include "CuckooFilter.h"
#include "Hash.h"
#include "CuckooFilter.cpp"
#include "Hash.cpp"
#include <vector>
#include "MurMurHash.h"
#include "MurMurHash.cpp"
#include <time.h>

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
    CuckooFilter cuckoofilter(4096);
    vector<string> rdstring = Rd_generate_string(1000);
    float count = 0;
    start = clock();
    for(int i = 0 ;i<rdstring.size();i++)
    {
        if(!cuckoofilter.Add(rdstring[i]))
        {
            count+=1;
        }
    }
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout<<"误报率:"<<float(count/1000)<<endl;
    cout<<"耗时:"<<duration<<"s"<<endl;
    return 0;
}