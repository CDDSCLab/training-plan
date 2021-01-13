package com.ren.bloomFilter;

import org.junit.Before;
import org.junit.Test;

public class BloomFilterTest {
    private BloomFilter bloomFilter;

    @Before
    public void start(){
        //传入预计输入数和希望得到的误报率
        bloomFilter = new BloomFilterImp(100000, 0.001);
        System.out.println("使用了多少个哈希函数：" + ((BloomFilterImp) bloomFilter).getK());
        System.out.println("创建的bit数组多大：" + ((BloomFilterImp) bloomFilter).getSIZE());
    }

    @Test
    public void test(){
        System.out.println(bloomFilter.contains("wang"));
        bloomFilter.put("wang");
        System.out.println(bloomFilter.contains("wang"));

        //计算误报率
        for (int i = 0; i < 100000; i++) {
            bloomFilter.put(i + "");
        }
        int num = 0;
        //查询一万个不存在的数
        for (int i = 100000; i < 200000; i++){
            if (bloomFilter.contains(i + "")){
                num++;
            }
        }
        System.out.println("误报数：" + num);
    }
}
