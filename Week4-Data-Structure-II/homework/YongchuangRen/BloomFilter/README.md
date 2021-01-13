# JAVA 实现 Bloom Filter
## JAVA 的预备知识
### BitSet
- Java 平台的 BitSet 用于存放一个``位序列``，如果要高效的存放一个位序列，就可以使用位集 (BitSet)。由于位集将位包装在字节里，所以使用位集比使用 Boolean 对象的 List 更加高效和更加节省存储空间。
- BitSet 是位操作的对象，``值只有 0或 1 即 false 和 true``，内部维护了一个 long 数组，初始只有一个 long ，所以 BitSet 最小的 size 是 64，当随着存储的元素越来越多，BitSet 内部会动态扩充，一次扩充 64 位，最终内部是由 N 个 long 来存储。
- ``默认情况下，BitSet 的所有位都是 false 即 0``。
- 在没有外部同步的情况下，多个线程操作一个 BitSet 是不安全的。

### 哈希算法
- 哈希算法（Hash）又称摘要算法（Digest），它的作用是：对任意一组输入数据进行计算，得到一个固定长度的输出摘要。哈希算法最重要的特点就是：
    - 相同的输入一定得到相同的输出；
    - 不同的输入大概率得到不同的输出。
- Java 中的 String 应用的哈希算法（JDK8 源码）
    ```java
    public int hashCode() {
        int h = hash;
        if (h == 0 && value.length > 0) {
            char val[] = value;

            for (int i = 0; i < value.length; i++) {
                h = 31 * h + val[i];
            }
            hash = h;
        }
        return h;
    }
    ```
    - 该函数很简单，以 31 为权，每一位为字符的 ASCII 值进行运算，用自然溢出来等效取模，达到了目的：只要字符串的内容相同，返回的哈希码也相同。(下文 References 2 有详解)
    - 但是乘子 31 在此需要解释一下。选 31 作为乘子，是因为：
        - 31 是一个奇质数，如果选择一个偶数会在乘法运算中产生溢出，导致数值信息丢失，因为乘二相当于移位运算。选择质数的优势并不是特别的明显，但这是一个传统。
- 其他用于查询的哈希函数
    1. 加法Hash；
    2. 位运算Hash；
    3. 乘法Hash；
    4. 除法Hash；
    5. 查表Hash；
    6. 混合Hash；
- http://www.burtleburtle.net/bob/hash/doobs.html 这个页面提供了对几种流行Hash算法的评价。

## Bloom Filter 的实现
基于 JAVA 语言使用 IDEA 简单实现布隆过滤器，包含两个主要文件

![](https://cdn.jsdelivr.net/gh/Beeter-yong/pictures/imgOne/BloomFilterImpl.png)
- Hash.java：该类包含网络上收集的八个哈希函数
- BloomFilter.java：该类是布隆过滤器定义的接口，定义布隆过滤器的工功能
- BloomFilterImp.java：该类具体实现布隆过滤器数据结构
- BloomFilterTest.java：该类实例化了一个布隆过滤器，并进行了插入和查找测试。

## References
1. [Java中BitSet的使用及详解](https://blog.csdn.net/u012736409/article/details/53735429)
2. [java中的哈希算法和hashcode深入讲解](https://blog.csdn.net/reggergdsg/article/details/53819293)
3. [Java之——Hash算法大全](https://blog.csdn.net/l1028386804/article/details/54573106)
