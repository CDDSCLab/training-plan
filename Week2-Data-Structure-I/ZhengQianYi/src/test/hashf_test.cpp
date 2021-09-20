#include <iostream>
#include <string>

#include "../ft/hasher.hpp"

using namespace ft;
using std::cout;
using std::endl;
using std::string;

int main()
{

    size_t a = 347589338;
        cout << a << endl;
    cout << "test hash " << hashf(HashType::murmur, 0, a) << endl;

    string s{"abc"};
    cout << s << endl;

    cout << "test hash " << hashf(HashType::murmur, 0, s) << endl;
}