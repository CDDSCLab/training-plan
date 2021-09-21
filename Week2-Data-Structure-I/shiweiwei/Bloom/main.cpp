#include <iostream>
#include "BloomFilter.h"
#include "hash.h"
#include <string> 
using namespace std;

int main(){
    bloomfilter bloom;
    int a, number = 0;
    string s;
    cout << "----------------测试-----------------" << endl;
    //插入1000个数字
    for (int i = 0; i < 1000; i++) {
        s = to_string(i);
        bloom.add(s);
    }
    //查询一千个不存在的数
    for (int i = 1000; i < 2000; i++) {
        s = to_string(i);
        if (bloom.isContain(s)) {
            number++;
        }
    }
    cout << "误报数：" << number << endl;
    cout << "请选择操作：1.插入 2.查询 其他键退出" << endl;
    cin >> a;
    while (a > 0 && a < 3) {
        if (a == 1) {
            cout << "请输入要插入的字符串" << endl;
            cin >> s;
            bloom.add(s);
            cout << "插入成功" << endl;
        }
        else if (a == 2) {
            cout << "请输入要查询的字符串" << endl;
            cin >> s;
            if(bloom.isContain(s))
                cout << "查询成功" << endl;
            else
                cout << "查询失败" << endl;
        }
        else {
            cout << "请输入数字1或者数字2" << endl;
        }
        cout << "请选择操作：1.插入 2.查询 其他键退出" << endl;
        cin >> a;
    }
}