
#include <string>
#include <sstream>
#include <fstream>

#include "filter/hashs.h"
#include "filter/filter.h"

using std::string;
using std::cout;
using std::cin;
using std::ifstream;
using std::vector;


int main() {
    //使用1000个不同的随机字符串做测试
    ifstream in_insert("randString1_1000.txt");
    ifstream in_lookup("randString2_1000.txt");
    string line;

    cout << "---------bloom_filter--------------" << endl;
    int bs_size = 20000;
    int hash_size = 8;
    bloom_filter bf(bs_size, hash_size);

    while (getline(in_insert, line)) {
        bf.insert(line);
    }

    int result;
    int count_error = 0;
    while (getline(in_lookup, line)) {
        result = bf.lookup(line);
        if (result == 1) {
            count_error++;
        }
    }

    cout << "test string: " << 1000 << endl;
    cout << "error count: " << count_error << endl;
    cout << "error rate: " << double(count_error/1000.0)*100 << "%" << endl;


    // cout << "---------cuckoo_filter--------------" << endl;
    // int bucket_size = 80;

    // cuckoo_filter cf(bucket_size);

    // while (getline(in_insert, line)) {
    //     cf.insert(line);
    // }

    // count_error = 0;
    // while (getline(in_lookup, line)) {
    //     result = cf.lookup(line);
    //     if (result == 1) {
    //         count_error++;
    //     }
    // }

    // cout << "test string: " << 1000 << endl;
    // cout << "error count: " << count_error << endl;
    // cout << "error rate: " << double(count_error/1000) << "%" << endl;

    getchar();
}