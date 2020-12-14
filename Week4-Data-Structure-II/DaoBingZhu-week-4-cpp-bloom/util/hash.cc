//
// Created by IcePig on 2020-07-27.
/*
 * 每次按照四字节长度读取字节流中的数据 w，并使用普通的哈希函数计算哈希值。计算过程中使用
 * uint32_t 的自然溢出特性。四字节读取则为了加速，最终可能剩下 3/2/1 个多余的字节，使用
 * switch 语句补充计算，以实现最好的性能。这里 FALLTHROUGH_INTENDED 宏并无实际作用，
 * 仅仅作为一种“我确定我这里想跳过”的标志。do {} while(0) 对代码无影响，这种写法也会出现
 * 在一些多行的宏定义里（见链接）。LevelDB 中哈希表和布隆过滤器会使用到该哈希函数。
 * DecodeFixed32那个是为了通用大小端才要的，这里直接改造成小段。
 */
#ifndef FALLTHROUGH_INTENDED
#define FALLTHROUGH_INTENDED do { } while (0)
#endif

#include "hash.h"

uint32_t Hash(const char *data, size_t n, uint32_t seed) {
    // Similar to murmur hash
    const uint32_t m = 0xc6a4a793;
    const uint32_t r = 24;
    const char *limit = data + n;
    uint32_t h = seed ^(n * m);

    // Pick up four bytes at a time
    while (data + 4 <= limit) {
        // 取出4字节
        // this is for little endian and big endian

        //  uint32_t w = DecodeFixed32(data);

        //  little endian only
        uint32_t w = *reinterpret_cast<const uint32_t * >(data);
        data += 4;
        h += w;
        h *= m;
        h ^= (h >> 16);
    }

    // Pick up remaining bytes
    switch (limit - data) {
        case 3:
            h += static_cast<unsigned char>(data[2]) << 16;
            FALLTHROUGH_INTENDED;// 人为FALLTHROUGH
        case 2:
            h += static_cast<unsigned char>(data[1]) << 8;
            FALLTHROUGH_INTENDED;// 人为FALLTHROUGH
        case 1:
            h += static_cast<unsigned char>(data[0]);
            h *= m;
            h ^= (h >> r);
            break;
    }
    return h;
}

