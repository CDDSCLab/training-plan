package com.ren.cuckooFilter;

import java.util.Arrays;

public class Keys {
    public Byte[] key;

    public Keys(int bucket_size) {
        key = new Byte[bucket_size];
        Arrays.fill(key, (byte)0);
    }
}
