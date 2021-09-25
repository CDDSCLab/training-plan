#include <iostream>
#include "CuckooFilter.h"
#include <string>
#include<windows.h>
using namespace std;

int main() {
    cuckoofilter cuckoo;
    int a, number = 0;
    string s;
    double time = 0;
    LARGE_INTEGER nFreq;
    LARGE_INTEGER nBeginTime;
    LARGE_INTEGER nEndTime;
    cout << "---------------------测试-------------------" << endl;
    cout << "测试三万个数字" << endl;
    //插入30000个数字
    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nBeginTime);//开始计时 
    for (int i = 0; i < 30000; i++) {
        s = to_string(i);
        cuckoo.add(s);
    }
    QueryPerformanceCounter(&nEndTime);//停止计时  
    time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;//计算程序执行时间单位为s  
    cout << "插入的运行时间：" << time * 1000 << "ms" << endl;
    //查询三万个不存在的数
    for (int i = 30000; i < 60000; i++) {
        s = to_string(i);
        if (cuckoo.isContain(s)) {
            number++;
        }
    }
    cout << "误报数：" << number << endl;
    cout << "误报率：" << (double)number / 30000 << endl << endl;
    cout << "测试五万个数字" << endl;
    //插入50000个数字
    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nBeginTime);//开始计时 
    for (int i = 0; i < 50000; i++) {
        s = to_string(i);
        cuckoo.add(s);
    }
    QueryPerformanceCounter(&nEndTime);//停止计时  
    time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;//计算程序执行时间单位为s  
    cout << "插入的运行时间：" << time * 1000 << "ms" << endl;
    //查询五万个不存在的数
    for (int i = 50000; i < 100000; i++) {
        s = to_string(i);
        if (cuckoo.isContain(s)) {
            number++;
        }
    }
    cout << "误报数：" << number << endl;
    cout << "误报率：" << (double)number / 50000 << endl << endl;
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