#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "filter/hashs.h"
#include "filter/filter.h"

#define TEST_TIME 50

using std::string;
using std::cout;
using std::cin;
using std::ifstream;
using std::vector;

class Timer{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> d_start;

public:
    //构造函数构造一个启动计时器
    Timer(){
        d_start=std::chrono::high_resolution_clock::now();
    }
    //析构时停止计时，并打印时间
    // ~Timer(){
    //     // stop();
    // }

    void stop(double &ms){
        auto d_end=std::chrono::high_resolution_clock::now();
        auto _start=std::chrono::time_point_cast<std::chrono::microseconds>(d_start)
                    .time_since_epoch()
                    .count();
        auto _end=std::chrono::time_point_cast<std::chrono::microseconds>(d_end)
                    .time_since_epoch()
                    .count();
        auto duration=_end-_start;
        ms=duration*0.001;
        // std::cout<<"use time:  "<<ms<<"ms\n";
    }
};

int main(){
    //使用10000个不同的随机字符串做测试
    ifstream in("randString_10000.txt");
    vector<string> str_vector;
    string line;
	while (getline(in, line)){
        str_vector.push_back(line);
	}

    double time_sum=0;

    cout<<"---------bloom_filter--------------"<<endl;
    int bs_size=20000;
    int hash_size=8;
    for (int i=0;i<TEST_TIME;i++){
        bloom_filter bf(bs_size,hash_size);

        double time;
        {
            Timer timer;
            for (int i=0;i<str_vector.size();i++){
                bf.insert(str_vector[i]);
            }
            timer.stop(time);
        }
        time_sum+=time;

    }
    cout<<"test string: "<<10000<<endl;
    cout<<"test time: "<<TEST_TIME<<endl;
    cout<<"avg use time: "<<time_sum/TEST_TIME<<"ms"<<endl;


    cout<<"---------cuckoo_filter--------------"<<endl;
    int bucket_size=80;
    time_sum=0;

    for (int i=0;i<TEST_TIME;i++){
        cuckoo_filter cf(bucket_size);
        double time;
        {
            Timer timer;
            for (int i=0;i<str_vector.size();i++){
                cf.insert(str_vector[i]);
            }
            timer.stop(time);
        }
        time_sum+=time;
    }
    cout<<"test string: "<<10000<<endl;
    cout<<"test time: "<<TEST_TIME<<endl;
    cout<<"avg use time: "<<time_sum/TEST_TIME<<"ms"<<endl;

    getchar();
}