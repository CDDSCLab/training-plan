#include <iostream>  
using namespace std;

#include<vector>  
#include "BloomFilter.h"
/*
 利用  bitmap和 hash实现
*/


void Test()
{   int abc;
    cout << "bitmap_size=";
    cin >> abc;
	BloomFilter<> bf(abc);
    double a;
    cout << "in_string_num=";
    cin >> a;
    string ss="";
    int s;
    char x;
    int k=rand()%10+1;//随机生成一个字符串的长度
    for(int b=0;b<a;b++)
        {
            for(int i=0;i<k;i++)
            {
            s=rand()%2;
            if(s==1)
                x=rand()%('Z'-'A'+1)+'A';
            else
                x=rand()%('z'-'a'+1)+'a';
            ss.push_back(x);
            }
            bf.Set(ss); 
            //cout << ss << endl;
            string ss="";
            
        }

    int t,y,d;
    double w=0;
    string tt="";
    cout << "test_string_num=";
    cin >> d;
    for(int b=0;b<d;b++)
        {
            for(int i=0;i<k;i++)
            {
            t=rand()%2;
            if(t==1)
                y=rand()%('Z'-'A'+1)+'A';
            else
                y=rand()%('z'-'a'+1)+'a';
            tt.push_back(y);
            }
            //cout << tt << endl;
            //cout << bf.IsIn(tt) << endl;
            if(bf.IsIn(tt))
                w++;
            string tt="";
        }

    double wr=w/d;
    cout.precision(4);
    cout << "wrong radio=" << wr;
}

int main()
{
	Test();
}