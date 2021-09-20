#include <iostream>
#include <string>

#include "../ft/bloom_filter.hpp"
#include "../ft/cuckoo_filter.hpp"
using namespace ft;
using std::cout;
using std::endl;
using std::string;

int main()
{
    bloom_filter bloom_ft(HashType::murmur, 10, 2);

    cout << "the number of cells is " << bloom_ft.get_cells() << endl;

    //test hasher
    cout << "test hash" << hashf(HashType::murmur, 0, "a") << endl;

    //test bloom_filter
    string str_list[] = {"AbC", "iJk", "XYZ", "a","a"};

    //add bloom_ft
    {
        for (const auto s : str_list)
        {
            bloom_ft.add(s);
        }
    }

    //loockup
    {
        for (const auto s : str_list)
        {
            auto result = bloom_ft.lookup(s);
            cout << s << " exist " << result << endl;
        }
    }

    //test bitvector
    cout << bloom_ft.get_bits() << endl;

    //test cuckoo_filter
    cuckoo_filter cuckoo_ft(HashType::murmur, 10, 8);

    //add

    for (const auto s : str_list)
    {
        cuckoo_ft.add(s);
    }

    //lookup
    for (const auto s : str_list)
    {
        auto result = cuckoo_ft.lookup(s);
        cout << s << " exist " << result << endl;
    }

    cout << cuckoo_ft.get_v() << endl;
}