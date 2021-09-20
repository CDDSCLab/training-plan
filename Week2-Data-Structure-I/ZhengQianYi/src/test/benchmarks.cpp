#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "../ft/bloom_filter.hpp"
#include "../ft/cuckoo_filter.hpp"
#include "../ft/Rand_int.hpp"

using namespace ft;
using std::cout;
using std::endl;
using std::string;

int main()
{
    //随机数
    Rand_int rnd{0, 2147483647};

    std::vector<string> ss;

    size_t s_num = 15800;
    size_t test_num = 10000;

    cout << "the number of strings  is " << s_num << endl;

    for (int i = s_num; i > 0; i--)
    {
        int k = rnd() % 10 + 1; //随机生成一个字符串的长度
        string s = "";
        for (int i = 0; i < k; i++)
        {
            int r;
            char x;
            r = rnd() % 2;
            if (r == 1)
                x = rnd() % ('Z' - 'A' + 1) + 'A';
            else
                x = rnd() % ('z' - 'a' + 1) + 'a';
            s.push_back(x);
        }
        ss.push_back(s);
    }

    std::vector<string> test_ss;

    for (int i = test_num; i > 0; i--)
    {
        int k = rnd() % 10 + 102; //随机生成一个字符串的长度
        string s = "";
        for (int i = 0; i < k; i++)
        {
            int r;
            char x;
            r = rnd() % 2;
            if (r == 1)
                x = rnd() % ('Z' - 'A' + 1) + 'A';
            else
                x = rnd() % ('z' - 'a' + 1) + 'a';
            s.push_back(x);
        }
        test_ss.push_back(s);
    }

    // add

    bloom_filter bloom_ft(0.05, ss.size(), HashType::murmur);

    cout << "the number of cells    is " << bloom_ft.get_cells() << endl;

    cout << "the number of hasher   is " << bloom_ft.seeds_.size() << endl;
    auto start = std::chrono::steady_clock::now();

    for (const auto s : ss)
    {
        bloom_ft.add(s);
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;
    cout << "插入用时：" << (double)elapsed.count() * 0.001 << "ms" << endl;

    // lookup
    size_t count = 0;
    for (auto s : test_ss)
    {
        if (bloom_ft.lookup(s))
        {
            count++;
        }
    }

    cout << "误判率为：" << count / (double)test_num << endl;

    // Cuckoo Filter
    cuckoo_filter cuckoo_ft(HashType::murmur, s_num * 10, 32);
    cout << "the number of cells    is " << cuckoo_ft.get_cell() << endl;
    start = std::chrono::steady_clock::now();
    for (const auto s : ss)
    {
        cuckoo_ft.add(s);
    }
    end = std::chrono::steady_clock::now();
    elapsed = end - start;
    cout << "插入用时：" << (double)elapsed.count() * 0.001 << "ms" << endl;

    count = 0;
    // test_ss = ss;
    for (auto s : test_ss)
    {
        if (cuckoo_ft.lookup(s))
        {
            count++;
        }
    }

    cout << "误判率为：" << count / (double)test_num << endl;
}