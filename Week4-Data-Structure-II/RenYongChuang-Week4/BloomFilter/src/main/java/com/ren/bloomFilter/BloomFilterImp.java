package com.ren.bloomFilter;


import java.util.BitSet;

public class BloomFilterImp implements BloomFilter {
    private int SIZE;   //创建数组大小
    private int K;      //哈希函数个数

    private BitSet bitSet;

    private BloomFilterImp(int SIZE) {
        this.SIZE = SIZE;

    }

    /**
     * 实例化布隆过滤器
     * @param n 预计存储元素个数
     * @param p 预想的误报率
     * @return
     */
    public BloomFilterImp (int n, double p) {
        //计算最佳数组大小
        int m = (int) (-(n * Math.log(p))/Math.pow(Math.log(2), 2));
        //计算最佳哈希函数个数
        int k = (int) (m * Math.log(2)/n);

        this.K = (k <= 8)? k : 8;
        this.SIZE = m;
        this.bitSet = new BitSet(SIZE);
    }

    public void put(String data) {
        for (int i = 0; i < K; i++){
            bitSet.set(getHash(data, i));
        }
    }

    public boolean contains(String data) {
        boolean have = true;
        for (int i = 0; i < K; i++){
            have &= bitSet.get(getHash(data, i));
        }
        return have;
    }

    /**
     * 计算哈希值
     * @param data
     * @param i 使用哪种哈希函数
     * @return
     */
    private int getHash(String data, int i){
        int hash;
        switch (i){
            case 0:
                hash = Hash.javaHash(data);
                break;
            case 1:
                hash = Hash.addHash(data);
                break;
            case 2:
                hash = Hash.RotatingHash(data);
                break;
            case 3:
                hash = Hash.oneByOneHash(data);
                break;
            case 4:
                hash = Hash.FNVHash(data);
                break;
            case 5:
                hash = Hash.APHash(data);
                break;
            case 6:
                hash = Hash.JSHash(data);
                break;
            default:
                hash = Hash.ELFHash(data);
        }
        return hash % SIZE;
    }

    public int getSIZE() {
        return SIZE;
    }

    public BloomFilterImp setSIZE(int SIZE) {
        this.SIZE = SIZE;
        return this;
    }

    public int getK() {
        return K;
    }

    public BloomFilterImp setK(int k) {
        K = k;
        return this;
    }
}
