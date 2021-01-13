package com.ren.cuckooFilter;

/**
 * 两个哈希函数，分别作用于哈希指纹、哈希位置
 */
public class Hash {

    /**
     * 哈希位置
     * @param data
     * @return
     */
    public static int javaHash(String data){
        int hash = data.hashCode();
        return hash > 0 ? hash : -hash;
    }

    /**
     * 将字符串转换成指纹
     * @param data
     * @return
     */
    public static Byte fingerprint(String data){
        int hash = 0;
        int i;
        for (i = 0; i < data.length(); ++i)
            hash = 33 * hash + data.charAt(i);
        byte finger = (byte) hash;
        //返回值是负数 -1 -- -128
        if (finger >= 0){
            return (byte)(finger - 128);
        }
        return (byte)hash;
    }

}
