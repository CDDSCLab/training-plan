#include<iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include"bloomfilter.h"

BloomFilter bloom;

int main()
{
    clock_t start, finish;
    double  duration;
    start = clock();

    std::fstream ifile("./testString.txt");
    if(!ifile){
        std::cerr<<"File open error!"<<std::endl;
        exit(1);
    } 
    std::string key;
    while(ifile>>key){
        bloom.add(key);   
    }
    ifile.close();

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout<<"element num: "<<bloom.getElementNum()<<std::endl;
    printf( "%f seconds\n", duration );
    return 0;
}
