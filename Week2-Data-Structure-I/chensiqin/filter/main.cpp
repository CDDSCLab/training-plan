#include "filter.h"
#include "hashs.h"
// #include <string>
// #include <iostream>

//add to test

int main(){
    cout<<"---------bloom_filter--------------"<<endl;
    int bs_size=30;
    int hash_size=8;
    int n_data=8;

    bloom_filter bf(bs_size,hash_size);
    // bloom_filter bf(n_data);
    bf.insert("111");
    bf.print();

    
    bf.insert("223");
    bf.print();
    bf.insert("556");
    bf.print();


    int a=bf.lookup("111");
    int b=bf.lookup("223");
    int c=bf.lookup("333");
    int d=bf.lookup("444");

    bf.print();
    cout<<"a="<<a<<",b="<<b<<endl;
    cout<<"c="<<c<<",d="<<d<<endl;
    cout<<bf.lookup("556")<<endl;

    cout<<"---------cuckoo_filter--------------"<<endl;

    int bucket_size=20;
    cuckoo_filter cf(bucket_size);
    cf.print();
    cf.insert("111");
    cf.print();

    
    cf.insert("223");
    cf.print();
    cf.insert("556");
    cf.print();

    a=cf.lookup("111");
    b=cf.lookup("223");
    c=cf.lookup("353");
    d=cf.lookup("444");

    cf.print();
    cout<<"a="<<a<<",b="<<b<<endl;
    cout<<"c="<<c<<",d="<<d<<endl;
    cout<<bf.lookup("556")<<endl;

    cf.del("223");
    a=cf.lookup("111");
    b=cf.lookup("223");
    cf.print();
    cout<<"a="<<a<<",b="<<b<<endl;

    cout<<"---------counting_bloom_filter--------------"<<endl;
    // int bs_size=12;
    // int hash_size=4;
    // int n_data=5;

    // counting_bloom_filter cbf(n_data);
    counting_bloom_filter cbf(10,3)   ;
    
    cbf.insert("111");
    cbf.print();

    
    cbf.insert("223");
    cbf.print();
    cbf.insert("556");
    cbf.print();


    // int a=cbf.lookup("111");
    // int b=cbf.lookup("223");
    // int c=cbf.lookup("333");
    // int d=cbf.lookup("444");

    cbf.print();
    cout<<"a="<<a<<",b="<<b<<endl;
    cout<<"c="<<c<<",d="<<d<<endl;
    cout<<cbf.lookup("556")<<endl;

    getchar();

}