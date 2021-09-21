#include<iostream>
#include<string>
using namespace std;
int main()
{
    unsigned char t=123;
    std::string key=std::to_string(t);
    cout<<key<<endl;

    return 0;
}