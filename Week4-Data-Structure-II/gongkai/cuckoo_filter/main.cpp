#include <iostream>
#include "cuckoofilter.h"
using namespace std;
int main() {
    cuckoofilter c;
    c.insert("5");
    c.insert("6");
    c.insert("dfsdfsd");
    c.insert("1");
    c.insert("2");
    c.insert("3");
    c.insert("4");
    c.insert("7");
    c.insert("8");
    c.insert("8");
    c.printbucket();

    cout<<c.isInBucket("1")<<endl;
    cout<<c.isInBucket("2")<<endl;
    cout<<c.isInBucket("5")<<endl;
    cout<<c.isInBucket("6")<<endl;
    cout<<c.isInBucket("3")<<endl;
    cout<<c.isInBucket("4")<<endl;
    cout<<c.isInBucket("7")<<endl;
    cout<<c.isInBucket("dfsdfsd")<<endl;
    cout<<c.isInBucket("456")<<endl;
    cout<<c.isInBucket("789")<<endl;

    cout<<c.Delete("1")<<endl;
    cout<<c.Delete("2")<<endl;
    cout<<c.isInBucket("1")<<endl;
    cout<<c.isInBucket("2")<<endl;

    return 0;
}
