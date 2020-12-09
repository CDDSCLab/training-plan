package com.ren.bloomFilter;

/**
 * 设置不同类型的 hash 函数
 */
public class Hash {
    //JAVA自带的Hash
    public static int javaHash(String data) {
        return (data.hashCode() > 0) ? data.hashCode() : -data.hashCode();
    }

    //加法Hash
    public static int addHash(String data){
        int hash, i;
        for (hash = data.length(), i = 0; i < data.length(); i++) {
            hash += data.charAt(i);
        }
        return hash > 0 ? hash : -hash;
    }

    //旋转Hash
    public static int RotatingHash(String data) {
        int hash, i;
        for (hash = data.length(), i = 0; i < data.length(); i++) {
            hash = (hash << 4) ^ (hash >> 28) ^ data.charAt(i);
        }
        return hash > 0 ? hash : -hash;
    }

    //一次一个Hash
    public static int oneByOneHash(String data) {
        int hash, i;
        for (hash = 0, i = 0; i < data.length(); ++i) {
            hash += data.charAt(i);
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        return hash > 0 ? hash : -hash;
    }

    //FNV算法
    public static int FNVHash(String data) {
        final int p = 16777619;
        int hash = (int) 2166136261L;
        for (int i = 0; i < data.length(); i++)
            hash = (hash ^ data.charAt(i)) * p;
        hash += hash << 13;
        hash ^= hash >> 7;
        hash += hash << 3;
        hash ^= hash >> 17;
        hash += hash << 5;
        return hash > 0 ? hash : -hash;
    }

    //JS算法
    public static int JSHash(String data) {
        int hash = 1315423911;
        for (int i = 0; i < data.length(); i++) {
            hash ^= ((hash << 5) + data.charAt(i) + (hash >> 2));
        }
        return (hash & 0x7FFFFFFF);
    }

    //AP算法
    public static int APHash(String data) {
        int hash = 0;
        for (int i = 0; i < data.length(); i++) {
            hash ^= ((i & 1) == 0) ? ((hash << 7) ^ data.charAt(i) ^ (hash >> 3))
                    : (~((hash << 11) ^ data.charAt(i) ^ (hash >> 5)));
        }
        return hash > 0 ? hash : -hash;
    }

    //ELF算法
    public static int ELFHash(String data){
        int hash = 0;
        int x = 0;
        for (int i = 0; i < data.length(); i++) {
            hash = (hash << 4) + data.charAt(i);
            if ((x = (int) (hash & 0xF0000000L)) != 0) {
                hash ^= (x >> 24);
                hash &= ~x;
            }
        }
        return hash > 0 ? hash : -hash;
    }

    public static void main(String[] args){
        String temp = "wangd";
        System.out.println(javaHash(temp));
        System.out.println(addHash(temp));
        System.out.println(RotatingHash(temp));
        System.out.println(oneByOneHash(temp));
        System.out.println(JSHash(temp));
        System.out.println(APHash(temp));
        System.out.println(ELFHash(temp));
        System.out.println(FNVHash(temp));
    }
}
