#include "cuckoo.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
using namespace cuckoo;

string random_uuid_6()
{
	char result[7]={0};
	char * result_tmp=&result[5];
	uint32_t factor_encry=2863314058;
	static uint32_t flag=0;  
	char chs[]= { '0', '1', '2', '3', '4', '5', '6', '7', '8',  
		'9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
		'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 
		'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',  
		'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',  
		'Z','+','-'};
	++flag;
	uint32_t factor=(int)(pow(2.0,0)+pow(2.0,1)+pow(2.0,2)+pow(2.0,3)+pow(2.0,4)+pow(2.0,5));
	for(int i=0;i<6;++i)
	{
		uint32_t data=pow(2.0,i*6)*factor;
		uint32_t flag_encry=factor_encry ^ flag;
		uint32_t tmp=data & flag_encry;
		tmp=tmp >> i*6;
		*result_tmp--=chs[tmp];
	}
	return string(result);
}

void test(int num){
    std::cout << "--------" << num << " elements --------" << std::endl;
    std::vector<string> testSet, retestSet;
    for(int i = 0; i < num; i++) testSet.push_back(random_uuid_6());
    for(int i = 0; i < num; i++) retestSet.push_back(random_uuid_6());
    Cuckoofilter cf;
    int fail_count = 0;
    clock_t start_time=clock();
    for(auto &s : testSet){
        int suc = cf.insert(s);
        if(suc == E_OUTOFKICK) fail_count++;
    }
    clock_t end_time=clock();
    std::cout << "Add " << num << " elements to Bloom filter took: " <<(double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << fail_count << " elements failed to insert." << std::endl;
    start_time=clock();
    bool b;
    fail_count = 0;
    for(auto &s : testSet){
        b = cf.lookup(s);
        if(!b) fail_count++;
    }
    end_time=clock();
    std::cout << fail_count << " elements not found." << std::endl;
    std::cout << "Query " << num << " elements to Bloom filter took: " <<(double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << std::endl;
    int wcnt = 0;
    for(auto &s : retestSet){
        b = cf.lookup(s);
        if(b) wcnt++;
    }
    std::cout << "False positive rate: " << wcnt / (double)num * 100 << "%" << std::endl;
}

int main(int argc, char **argv){
    test(500);
    test(1000);
    test(10000);
    test(50000);
    test(100000);
    test(300000);
    return 0;
}