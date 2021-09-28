#include "filter.h"
#include "hashs.h"
// #pragma once



counting_bloom_filter::counting_bloom_filter(int m, int k){
    bs_size=m;
    bscount.resize(m);
    hash_size=k;
};

//additonal construction function
counting_bloom_filter::counting_bloom_filter(int n){
    double e=ERROR_RATE;
    int m=-n*log(e)/(log(2)*log(2));
    int k=m*log(2)/n;
    bs_size=m;
    bscount.resize(m);
    hash_size=k;
    if(k>8){
        hash_size=8;
    }
};

int counting_bloom_filter::insert(string key){
    hashchooser hc;
    for(int i=0;i<hash_size;i++){
        bscount[hc.gethash(i,key,bs_size)]++;
    }
    // cout<<"insert "<<key<<" successfully"<<endl;
    return 1;
}

int counting_bloom_filter::lookup(string key){
    hashchooser hc;
    for(int i=0;i<hash_size;i++){
        int existcount=bscount[hc.gethash(i,key,bs_size)];
        if(existcount<=0){
            // cout<<"fail to lookup "<<key<<"!"<<endl;
            return 0;
        }
    }
    // cout<<"find "<<key<<" successfully"<<endl;
    return 1;

}

int counting_bloom_filter::del(string key){
    hashchooser hc;
    if(lookup(key)){
        for(int i=0;i<hash_size;i++){
            bscount[hc.gethash(i,key,bs_size)]--;
        }
        return 1;
    }
    // cout<<"fail to delete "<<key<<"!"<<endl;
    return 0;

}

void counting_bloom_filter::print(){
    cout<<"bs_size="<<bs_size<<endl;
    cout<<"hash_size="<<hash_size<<endl;
    for(int i=0;i<bs_size;i++){
        cout<<bscount[i]<<" ";
    }
    cout<<endl<<endl;

}