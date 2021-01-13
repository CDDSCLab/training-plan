package com.ren.cuckooFilter;

import java.util.Arrays;

public class CuckooFilter {
    private int bucket_capacity;    //过滤器的bucket数量
    private int bucket_size;    //过滤器的bucket大小(可以存放几个相同哈希的key)
    private int max_kicks;      //最大的‘踢’的次数限制
    private int counts;     //过滤器存放的key的数量
    private Keys[] keys;    //过滤器数组。

    /**
     * 构造一个布谷鸟过滤器
     * @param bucket_capacity 过滤器大小
     * @param bucket_size   过滤器每个空位放置元素数
     * @param max_kicks     过滤器的扩容阈值
     */
    public CuckooFilter(int bucket_capacity, int bucket_size, int max_kicks) {
        this.bucket_capacity = bucket_capacity;
        this.bucket_size = bucket_size;
        this.max_kicks = max_kicks;
        keys = new Keys[bucket_capacity];
        //为每一个bucket设置key空间
        for (int i = 0; i < bucket_capacity; i++){
            keys[i] = new Keys(bucket_size);
        }
        this.counts = 0;
    }

    /**
     * 添加一个元素
     * @param data
     * @return
     */
    public boolean addKey(String data){
        byte finger = Hash.fingerprint(data);   //将字符串转换为字节型指纹
        //求得两个哈希值
        int hash1 = Hash.javaHash(data) % bucket_capacity;
        int hash2 = (hash1 ^ (Hash.javaHash(-finger+"") % bucket_capacity)) % bucket_capacity;
        //寻找哈希值的bucket对应位置为空并插入指纹
        int index = keyEntryLocation(hash1);
        if (index != -1){
            keys[hash1].key[index] = finger;
            this.counts++;
            return true;
        }
        index = keyEntryLocation(hash2);
        if (index != -1){
            keys[hash2].key[index] = finger;
            this.counts++;
            return true;
        }
        //如果两个位置没有空位，随机选择一个“踢出”
        byte keyFinger = 0;
        int hash = (Math.random() % 2 == 0) ? hash1 : hash2;
        for (int i = 0; i < this.max_kicks; i++){
            index = (int) (Math.random() % bucket_size);    //随机选择一个bucket内的要踢出的key
            keyFinger = keys[hash].key[index];
            keys[hash].key[index] = finger;

            //为被踢的key找新窝
            hash = (hash ^ (Hash.javaHash(-keyFinger + "") % bucket_capacity)) % bucket_capacity;
            index = keyEntryLocation(hash);
            if (index != -1){
                keys[hash].key[index] = keyFinger;
                this.counts++;
                return true;
            }//如果没有找到新巢就继续“踢”
        }
        //现在踢的次数到达了踢的阈值，我们需要扩容操作
        if (!cuckooFilterExpand())
            return false;
        //扩容后将“被踢的”key放入新扩容的巣
        keys[hash].key[bucket_size] = keyFinger;
        this.counts++;
        return true;
    }

    /**
     * 查找key是否存在
     * @param data
     * @return
     */
    public boolean lookUp(String data){
        byte finger = Hash.fingerprint(data);
        //求得两个哈希值
        int hash1 = Hash.javaHash(data) % bucket_capacity;
        int hash2 = (hash1 ^ (Hash.javaHash(-finger+"") % bucket_capacity)) % bucket_capacity;
        int index = 0;
//        index = keyEntryLocation(hash1);
//        if (index != -1 && keys[hash1].key[index] != 0){
//            return true;
//        }
//        index = keyEntryLocation(hash2);
//        if (index != -1 && keys[hash2].key[index] != 0){
//            return true;
//        }
        if (keyLocation(hash1,finger) != -1 || keyLocation(hash2, finger) != -1){
            return true;
        }
        return false;
    }

    /**
     * 删除key
     * @param data
     * @return
     */
    public boolean delete(String data){
        byte finger = Hash.fingerprint(data);
        //求得两个哈希值
        int hash1 = Hash.javaHash(data) % bucket_capacity;
        int hash2 = (hash1 ^ (Hash.javaHash(-finger+"") % bucket_capacity)) % bucket_capacity;
        int index = 0;
        index = keyLocation(hash1, finger);
        if (index != -1){
            keys[hash1].key[index] = 0;
            this.counts--;
            return true;
        }
        index = keyLocation(hash2, finger);
        if (index != -1){
            keys[hash2].key[index] = 0;
            this.counts--;
            return true;
        }
        return false;
    }

    /**
     * 布谷鸟过滤器扩容
     * @return
     */
    private boolean cuckooFilterExpand(){
        for (int i = 0; i < bucket_capacity; i++){
            Keys newKey = new Keys(this.bucket_size + 1);
            newKey.key = Arrays.copyOf(keys[i].key, this.bucket_size);
            keys[i] = newKey;
        }
        this.bucket_size++;
        return true;
    }

    /**
     * 查找hash对应的bucket是空的位置
     * @param hash 查找哪一个bucket
     * @return  返回空位置，没有空位置返回-1
     */
    private int keyEntryLocation(int hash){
        for (int i = 0; i < bucket_size; i++){
            if (keys[hash].key[i] == 0){
                return i;
            }
        }
        return -1;  //已经没有空位置了
    }

    /**
     * 查找指纹位置
     * @param hash
     * @param finger
     * @return
     */
    private int keyLocation(int hash, byte finger){
        for (int i = 0; i < bucket_size; i++){
            if (keys[hash].key[i] == finger){
                return i;
            }
        }
        return -1;
    }

}
