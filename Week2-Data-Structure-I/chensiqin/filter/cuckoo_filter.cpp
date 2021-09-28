// #pragma once
#include "filter.h"

#include "hashs.h"
 // #include <iostream>



cuckoo_filter::cuckoo_filter(int m) {
    bucket_size = CUCKOO_BUCKET_SIZE;
    // bucket_num = std::pow(2, m); //限制桶的数量为指数
    bucket_num = CUCKOO_BUCKET_NUM;
    std::memset(bucket, 0, sizeof bucket);


    // bucket.resize(bucket_num);
}

int cuckoo_filter::insert(string key) {
    hashchooser hc;
    unsigned int fingerprint = hc.fingerprint(key);
    unsigned int i1 = hc.cuckoo_hash(key, bucket_num);
    unsigned int i2 = i1 ^ hc.cuckoo_hash(intToString(fingerprint), bucket_num);

    // cout << "-----try insert:" << key << "------" << endl;
    // cout << "i1=" << i1 << endl;
    // cout << "i2=" << i2 << endl;
    // cout << "fingerprint:" << fingerprint << endl;


    //if there is an empty entry, put the fingerprint on it.
    for (int i = 0; i < bucket_size; i++) {
        if (bucket[i1][i] == 0) {
            bucket[i1][i] = fingerprint;
            return 1;
        }
    }
    for (int i = 0; i < bucket_size; i++) {
        if (bucket[i2][i] == 0) {
            bucket[i2][i] = fingerprint;
            return 1;
        }
    }

    //if there is no empty, random choose another one
    unsigned int newfingerprint = fingerprint;
    for (int k = 0; k < MAX_CUCKOO_KICKS; k++) {

        srand((unsigned int) time(NULL));
        int random = rand() % 2;
        int kick_th = rand() % bucket_size;
        unsigned int index = (random == 0 ? i1 : i2);

        //对于桶中的每个元素，寻找一个可替换到其他空的位置的元素temp做替换

        for (int i = 0; i < bucket_size; i++) {
            unsigned int temp = bucket[index][i];
            unsigned int temp_loc = index ^ hc.cuckoo_hash(intToString(temp), bucket_num);
            for (int j = 0; j < bucket_size; j++) {
                if (bucket[temp_loc][j] == 0) //如果另一个桶有空位，就把元素temp放这，并替换进关键元素
                {
                    bucket[temp_loc][j] = temp;
                    bucket[index][i] = newfingerprint;
                    return 1;
                }
            }

        }
        //全部都没空，则发生循环替换，取两个桶的一个元素，作为新的fingerprint
        newfingerprint = bucket[index][kick_th];
        i1 = index;
        i2 = index ^ hc.cuckoo_hash(intToString(newfingerprint), bucket_num);
    }


    //if all is full,reture false
    cout << "insert false!" << endl;
    return 0;
};

int cuckoo_filter::lookup(string key) {
    hashchooser hc;
    unsigned int fingerprint = hc.fingerprint(key);
    unsigned int i1 = hc.cuckoo_hash(key, bucket_num);
    unsigned int i2 = i1 ^ hc.cuckoo_hash(intToString(fingerprint), bucket_num);


    cout << "----try lookup:" << key << "----" << endl;
    cout << "i1=" << i1 << endl;
    cout << "i2=" << i2 << endl;
    cout << "fingerprint:" << fingerprint << endl;

    for (int i = 0; i < bucket_size; i++) {
        cout << bucket[i1][i] << endl;
        if (bucket[i1][i] == fingerprint) {
            cout << "lookup: " << key << ", location:bucket" << i1 << endl;
            return 1;
        } else if (bucket[i2][i] == fingerprint) {
            cout << "lookup: " << key << ", location:bucket" << i2 << endl;
            return 1;
        }
    }


    cout << "lookup fail!" << endl;
    return 0;
};

int cuckoo_filter::del(string key) {
    hashchooser hc;
    unsigned int fingerprint = hc.fingerprint(key);
    unsigned int i1 = hc.cuckoo_hash(key, bucket_num);
    unsigned int i2 = i1 ^ hc.cuckoo_hash(intToString(fingerprint), bucket_num);


    cout << "----try del:" << key << "----" << endl;
    cout << "i1=" << i1 << endl;
    cout << "i2=" << i2 << endl;
    cout << "fingerprint:" << fingerprint << endl;

    for (int i = 0; i < bucket_size; i++) {
        if (bucket[i1][i] == fingerprint) {
            bucket[i1][i] = 0;
            return 1;
        } else if (bucket[i2][i] == fingerprint) {
            bucket[i2][i] = 0;
            return 1;
        }
    }

    cout << "del false!" << endl;
    return 0;
};

void cuckoo_filter::print() {
    // cout<<"bucket:"<<bucket<<endl;
    cout << "bucket_num=" << bucket_num << endl;
    cout << "bucket_size=" << bucket_size << endl;
    for (int i = 0; i < bucket_num; i++) {
        cout << "bucket[" << i << "]: ";
        for (int j = 0; j < bucket_size; j++) {
            cout << bucket[i][j] << " ";
        }
        cout << endl;

    }
};