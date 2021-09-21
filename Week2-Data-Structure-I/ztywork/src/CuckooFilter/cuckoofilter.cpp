#include "cuckoofilter.h"

cuckoofilter::cuckoofilter(){
    memset(buckets,0,BUCKET_NUM*BUCKET_SIZE);
    element_num=0;
    kick_max=1000;
}

unsigned char cuckoofilter::fingerprint(std::string key){
    unsigned int hash_out;
    MurmurHash3_x86_32(key.c_str(),key.length(),521,&hash_out);
    return hash_out%256;
}

unsigned int cuckoofilter::position_now(std::string key){
    unsigned int hash_out;
    MurmurHash3_x86_32(key.c_str(),key.length(),378,&hash_out);
    return hash_out%BUCKET_NUM;
}

unsigned int cuckoofilter::position_dual(unsigned int now,unsigned char fp){
    unsigned int hash_out;
    std::string key = std::to_string(fp);
    MurmurHash3_x86_32(key.c_str(),key.length(),378,&hash_out);
    return (hash_out^now)%BUCKET_NUM;
}

bool cuckoofilter::contains(std::string key){
    unsigned char fp=fingerprint(key);
    unsigned int p1=position_now(key);
    unsigned int p2=position_dual(p1,fp);
    if(std::find(buckets[p1],buckets[p1]+BUCKET_SIZE,fp)!=buckets[p1]+BUCKET_SIZE)
        return true;
    if(std::find(buckets[p2],buckets[p2]+BUCKET_SIZE,fp)!=buckets[p2]+BUCKET_SIZE)
        return true;
    return false;
}

bool cuckoofilter::deletes(std::string key){
    if(!contains(key)) return false;
    unsigned char fp=fingerprint(key);
    unsigned int p1=position_now(key);
    unsigned int p2=position_dual(p1,fp);
    unsigned char *ptr=std::find(buckets[p1],buckets[p1]+BUCKET_SIZE,fp);
    if(ptr!=buckets[p1]+BUCKET_SIZE){
        memset(ptr,0,1);
        element_num--;
        return true;
    }
    ptr=std::find(buckets[p2],buckets[p2]+BUCKET_SIZE,fp);
    if(ptr!=buckets[p2]+BUCKET_SIZE){
        memset(ptr,0,1);
        element_num--;
        return true;
    }
    return false;
}


bool cuckoofilter::inserts(std::string key){
    if(contains(key)) return false;
    unsigned char fp=fingerprint(key);
    unsigned int p1=position_now(key);
    unsigned int p2=position_dual(p1,fp);
    unsigned char empty=0;
    unsigned char *ptr=std::find(buckets[p1],buckets[p1]+BUCKET_SIZE,empty);
    if(ptr!=buckets[p1]+BUCKET_SIZE){
        *ptr=fp;
        element_num++;
        return true;
    }
    ptr=std::find(buckets[p2],buckets[p2]+BUCKET_SIZE,empty);
    if(ptr!=buckets[p2]+BUCKET_SIZE){
        *ptr=fp;
        element_num++;
        return true;
    }
    unsigned int p=p1;
    unsigned int kick=0;
    while(kick<kick_max){
        unsigned char old_fp=buckets[p][0];
        buckets[p][0]=fp;
        fp=old_fp;
        p=position_dual(p,fp);
        ptr=std::find(buckets[p],buckets[p]+BUCKET_SIZE,empty);
        if(ptr!=buckets[p]+BUCKET_SIZE){
            *ptr=fp;
            element_num++;
            return true;
        }
        kick++;
    }
    return false;
}

unsigned int cuckoofilter::getElementNum(){
    return element_num;
}