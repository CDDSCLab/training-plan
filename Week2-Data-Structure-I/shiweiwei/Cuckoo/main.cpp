#include <iostream>
#include "CuckooFilter.h"
#include <string>
using namespace std;

int main() {
    cuckoofilter cuckoo;
    int a, number = 0;
    string s;
    cout << "---------------------测试-------------------" << endl;
    //插入4000个数字
    for (int i = 0; i < 4000; i++) {
        s = to_string(i);
        cuckoo.add(s);
    }
    //查询四千个不存在的数
    for (int i = 4000; i < 8000; i++) {
        s = to_string(i);
        if (cuckoo.isContain(s)) {
            number++;
        }
    }
    cout << "误报数：" << number << endl;
    cout << "请选择操作：1.插入 2.查询 3.删除 其他键退出" << endl;
    cin >> a;
    while (a > 0 && a < 4) {
        if (a == 1) {
            cout << "请输入要插入的字符串" << endl;
            cin >> s;
            if (cuckoo.add(s))
                cout << "插入成功" << endl;
            else
                cout << "插入失败" << endl;
        }
        else if (a == 2) {
            cout << "请输入要查询的字符串" << endl;
            cin >> s;
            if (cuckoo.isContain(s))
                cout << "查询成功" << endl;
            else
                cout << "查询失败" << endl;
        }
        else if (a == 3) {
            cout << "请输入要删除的字符串" << endl;
            cin >> s;
            if (cuckoo.Delete(s))
                cout << "删除成功" << endl;
            else
                cout << "删除失败" << endl;
        }
        else {
            cout << "请输入数字1，2，3" << endl;
        }
        cout << "请选择操作：1.插入 2.查询 3.删除 其他键退出" << endl;
        cin >> a;
    }
}