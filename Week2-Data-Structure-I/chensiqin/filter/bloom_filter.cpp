#include "filter.h"
#include "hashs.h"
// #pragma once


bloom_filter::bloom_filter(int m, int k){
    bs_size=m;
    hash_size=k;
        if(k>8){
        hash_size=8;
    }
};

//additonal construction function
bloom_filter::bloom_filter(int n){
    double e=ERROR_RATE;
    int m=-n*log(e)/(log(2)*log(2));
    int k=m*log(2)/n;
    bs_size=m;
    hash_size=k;
    if(k>8){
        hash_size=8;
    }
};

int bloom_filter::insert(string key){
    hashchooser hc;
    for(int i=0;i<hash_size;i++){
        bs.set(hc.gethash(i,key,bs_size));
    }
    return 1;
}

int bloom_filter::lookup(string key){
    hashchooser hc;
    for(int i=0;i<hash_size;i++){
        bool isexist=bs.test(hc.gethash(i,key,bs_size));
        if(!isexist){
            return 0;
        }
    }
    return 1;

}

int bloom_filter::del(string key){
    return -1;
}

void bloom_filter::print(){
    cout<<"bs:"<<bs<<endl;
    cout<<"bs_size="<<bs_size<<endl;
    cout<<"hash_size="<<hash_size<<endl;
}