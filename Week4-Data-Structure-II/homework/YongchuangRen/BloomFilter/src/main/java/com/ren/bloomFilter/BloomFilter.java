package com.ren.bloomFilter;

public interface BloomFilter {
    //增加元素
    public void put(String data);
    //查询元素
    public boolean contains(String data);
}
