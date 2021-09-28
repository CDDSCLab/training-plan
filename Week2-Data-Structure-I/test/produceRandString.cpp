
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <ctime>
#include <algorithm>

using std::string;

string uniqueName(int length) {
    auto randchar = []() -> char
    {
        const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

int main()
{
    std::fstream ofile("./randString2_1000.txt",std::ios::out);
    if(!ofile){
        std::cerr<<"File open or create error!"<<std::endl;
        exit(1);
    }

    srand(std::time(0));

    for(int i=0;i<1000;i++){

        ofile<<uniqueName(10+rand()%40)<<std::endl;
    }
    ofile.close();
    return 0;
}