#include <iostream>
#include "CuckooFilter.h"
#include "Hash.h"
#include "CuckooFilter.cpp"
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
    CuckooFilter cuckoofilter(4096);
    vector<string> rdstring = Rd_generate_string(10);
    for(int i = 0 ;i<rdstring.size();i++)
    {
        cuckoofilter.Add(rdstring[i]);
    }
    cuckoofilter.Lookup("vou");
    cuckoofilter.Lookup("houlei");
    cuckoofilter.Delete("vou");
    cuckoofilter.Lookup("vou");
    return 0;
}