#include <iostream>
#include "CuckooFilter.h"
#include "Hash.h"


using namespace std;

int CuckooFilter::Add(string str)
{
    uint32_t res_hash = hash.Murmurhash(str)[0];
    uint8_t fingerprint = res_hash;
    uint32_t idx1 , idx2 , idx3;
    idx1 = res_hash % bucket_size;
    idx2 = idx1 ^ hash.Murmurhash(to_string(fingerprint))[0]  % bucket_size;
    idx3 = idx2 ^ hash.Murmurhash(to_string(fingerprint))[0]  % bucket_size;
    if(!bucket[idx1])
    {
        bucket[idx1] = fingerprint ;
        cout<<"添加"<<str<<"成功("<<idx1<<")"<<endl;
        return 1;
    }
    else if(!bucket[idx2])
    {
        bucket[idx2] = fingerprint;
        cout<<"添加"<<str<<"成功("<<idx2<<")"<<endl;
        return 2;
    }
    uint32_t sel = rand()%2;
    uint32_t count = 0 ;
    if(sel==1)
    {
        uint8_t temp_fingerprint = bucket[idx1];
        bucket[idx1] = fingerprint;
        return Add_Conflict(idx1 , temp_fingerprint);
    }else 
    {
        uint8_t temp_fingerprint = bucket[idx2];
        bucket[idx2] = fingerprint;
        return Add_Conflict(idx2 , temp_fingerprint);
    }    
    return 0;
}

int CuckooFilter::Add_Conflict(uint32_t idx , uint8_t fingerprint)
{
    uint32_t idxx = idx ^ hash.Murmurhash(to_string(fingerprint))[0] % bucket_size;
    if(!bucket[idxx])
    {
        bucket[idxx] = fingerprint;
        cout<<"kick success"<<endl;
        return 1;
    }
    else 
    {
        conflict_count++;
        if(conflict_count>5)
        {
            cout<<"kick fail, loop"<<endl;
            return 0;
        }
        uint8_t temp_fingerprint = bucket[idxx];
        bucket[idxx] = fingerprint;
        return Add_Conflict(idxx,temp_fingerprint);
    }
}

int CuckooFilter::Lookup(string str)
{
    uint32_t res_hash = hash.Murmurhash(str)[0];
    uint8_t fingerprint = res_hash;
    uint32_t idx1 , idx2 ;
    idx1 = res_hash % bucket_size;
    idx2 = idx1 ^ hash.Murmurhash(to_string(fingerprint))[0]  % bucket_size;
    if(bucket[idx1] == fingerprint)
    {
        cout<<str<<"在数据库中"<<endl;
        return 1 ;

    }
    else if(bucket[idx2] == fingerprint)
    {
        cout<<str<<"在数据库中"<<endl;
        return 2 ;
    }
    else 
    {
        cout<<str<<"不在数据库中"<<endl;
        return 0 ;
    }
}

int CuckooFilter::Delete(string str)
{
    uint32_t res_hash = hash.Murmurhash(str)[0];
    uint8_t fingerprint = res_hash;
    uint32_t idx1 , idx2 ;
    idx1 = res_hash % bucket_size;
    idx2 = idx1 ^ hash.Murmurhash(to_string(fingerprint))[0]  % bucket_size;
    if(bucket[idx1] == fingerprint)
    {
        bucket[idx1] = 0;
        cout<<str<<"删除成功"<<endl;
        return 1 ;

    }
    else if(bucket[idx2] == fingerprint)
    {
        bucket[idx2] = 0;
        cout<<str<<"删除成功"<<endl;
        return 2 ;
    }
    else 
    {
        cout<<str<<"不在数据库中"<<endl;
        return 0 ;
    }
}