# 过滤器

## 1 理论

### Bloom Filter

#### 介绍

1. 原理：当一个元素被加入集合时，通过K个Hash函数将这些元素映射成一个位图阵列(位图)中的K个点，把它们置为1。当检索时，只需要看看这K个点是不是都是1就**大约**知道集合中有没有这个检索的元素。如果这些点有任何一个0，那么被检索元素**一定不在**，如果都是1，**可能**在。

2. 优点:

   a. 不需要存储key，节省空间；

   b. 布隆过滤器存储空间和插入 / 查询时间都是常数O(k)；

3. 缺陷:
   ​ 1）不能删除。
   ​ 2）存在误判率。在判断一个元素是否属于某个集合时，有可能把不属于这个集合的元素误认为会属于这个集合，因此布隆过滤器在能容忍低错误率的应用场合下，通过极少的错误来换取存储空间的极大节省。如下图，x、y、z通过3个哈希函数映射的结果，当需要检测w是否存在时，同样将w通过3个哈希函数映射出三个位置，发现全都置为1了，得出结论w是存在的。但其实不然，只是恰好w映射的位置结果已经全被占用了，此时即出现误判。

#### 构建流程

- 首先初始状态下，初始一个包含m位的的位数组，每一位都置为0
- 为了表达一个S={x1,x2,…,xn}这样一个n个元素的数组，使用k个Hash函数，将它们分别映射到[1,m]的范围中，对于任意一个元素x，第i个哈希函数映射的位置hi(x)会被置为1.如果一个位置多次被置为1，那么只有第一次会起作用
- 在判断y是否属于集合S时，对y使用k次哈希函数，如果所有的hi(y)的位置都是1，那么就认为y是集合中的元素，否则就认为y不是集合中的元素
  如下图，x、y、z通过3个哈希函数映射到对应位置置1，当需要检测w是否存在时，同样将w通过3个哈希函数映射出三个位置，3个位置全部置1则判断w可能存在；若不全为1，则w一定不存在，本例子中w即不存在。

#### 错误率

当集合S={x1,x2,…,xn}的所有元素都被k个哈希函数映射到m位的位数组中时，这个位数组中某一位还是0的概率是：

<img src="https://pic2.zhimg.com/v2-10950c27d6dd108acb6184bc892a6955_r.jpg" alt="preview" style="zoom: 50%;" />

其中`1/m`表示任意一个哈希函数选中这一位标1的概率，用`1`减去`1/m`即是哈希一次都没有被选中这一位的概率，然后把S映射到位数组中，需要做kn次哈希。

#### 最优哈希函数个数

k = (ln2) * (m/n)



### **Cuckoo Filter**

为了解决布隆过滤器中不能删除，且存在误判的缺点，本文引入了一种新的哈希算法——cuckoo filter，它既可以**确保该元素存在的必然性**，又可以在不违背此前提下**删除任意元素**，仅仅比bitmap**牺牲了微量空间效率**。

#### **Cuckoo hashing**

需要先理解cuckoo hashing

 Cuckoo的哈希函数是成对的（具体的实现可以根据需求设计），每一个元素都是两个，分别映射到两个位置，一个是记录的位置，另一个是备用位置，这个备用位置是处理碰撞时用的。

如下图，使用hashA 和hashB 计算对应key x的位置a和b ：

1. 当两个哈希位置有一个为空时，则插入该空位置；
2. 当两个哈希位置均不为空时，随机选择两者之一的位置上key y 踢出，并计算踢出的key y在另一个哈希值对应的位置，若为空直接插入，不为空踢出原元素插入，再对被踢出的元素重新计算，重复该过程，直到有空位置为止。直到被踢的次数达到一个上限，才确认哈希表已满，并执行rehash操作。

![v2-2d76f372e0a4b4549487779eb1ba9aa1_r](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915135717.jpg)

#### **Cuckoo Filter介绍**

下面介绍 cuckoo filter。相对的cuckoo hashing，cuckoo filter里不会存储原信息， 只存储指纹信息

1. 算法特点：因为在插入过程中可能因为反复踢出无限循环下去，这时就需要进行一次循环踢出的限制，超出限制则认为过滤器容量不足，需要进行扩容。另外布谷鸟过滤器在bucket上存储的是key的`指纹`，主要是考虑到不同的key长度不定，节约空间。`fingerprint` 一般由key做hash得来，长度较短，一般为8-12bit，也可根据实际数据量调整。

2. 寻找第二个bucket的方法：由于计算key的存储位置使用的时key本身的值，而存储时仅存储了fingerprint，因此当key被挤出，需要重新找位置时，通过指纹信息来找到第二个bucket

cuckoo filter采用只需要指纹信息就能找到第二个位置的方式， 技巧在这里：

   1.  loc1= HASH(key)
   2.  loc2= loc1 ⊕HASH(key′s fingerprint)
      其中 ⊕ 是异或运算
      也就是说， 知道了当前的loc1， 知道指纹信息， 就可以计算出另外一个loc2。

   > loc1做异或运算可以得到loc2，那么假如被踢出来的key已经是loc2了，其做异或运算会得到loc1还是其他位置呢？
   > **答案是loc1，**原因在于异或运算的性质。**存在性质：b^a^a=b**。
   > 因此当loc2做异或=loc1^HASH(key′s fingerprint)^HASH(key′s fingerprint)=loc1.
   > 由此，踢出来的loc做异或运算，总能找到另外一个位置。



## 2 实现

 类

![image-20210915194148550](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915194148.png)

其中各类的描述如下：

| 类名                                                 | 描述                               |
| ---------------------------------------------------- | ---------------------------------- |
| **C**[bitvector](classft_1_1bitvector.html)          | 布隆过滤器的底层bits存储           |
| **C**[bloom_filter](classft_1_1bloom__filter.html)   | 布隆过滤器实现                     |
| **C**[cuckoo_vector](classft_1_1cuckoo__vector.html) | 布谷鸟过滤器的底层存储，包括两个桶 |
| **C**[cuckoo_filter](classft_1_1cuckoo__filter.html) | 布谷鸟过滤器实现                   |
| **C**[filter](classft_1_1filter.html)                | 过滤器基类                         |
| **C**[Rand_int](classft_1_1Rand__int.html)           | 封装生成随机数                     |

### filter类

![image-20210915134517063](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915134517.png)

### bitvector类

<img src="https://raw.githubusercontent.com/zhengqianyi/img/master/20210915134240.png" alt="image-20210915134240286"  /> 

![image-20210915134529796](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915134529.png) 

### bloom_filter类

 ![image-20210915134305753](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915134305.png)

 ![image-20210915133955927](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915133955.png)

### cuckoo_vector类

 ![image-20210915134343469](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915134343.png)

### cuckoo_filter类

 ![image-20210915134425247](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915134425.png)

![image-20210915134445437](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915134445.png)

## 3 单元测试

进行hash函数测试

测试hash函数能否正确对string和int变量进行Hash

```bash
$ g++ hashf_test.cpp -o hashf_test -g
$ ./hashf_test
```

测试结果：

 ![image-20210915140106381](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915140106.png)

进行过滤器测试

测试过滤器add和lookup，并且输出过滤器内部存储

```bash
$ g++ unit_test_imp.cpp -o unit_test_imp -g
$ ./unit_test_imp
```

测试结果：

![image-20210915140415466](https://raw.githubusercontent.com/zhengqianyi/img/master/20210915140415.png) 

## 4 性能测试

测试代码位于```/test/benchmark.cpp```

随机生成字符串：

```c++
Rand_int rnd{0, 2147483647};

    std::vector<string> ss;

    for (int i = 1000; i > 0; i--)
    {
        int k = rnd() % 100 + 1; //随机生成一个字符串的长度
        string s = "";
        for (int i = 0; i < k; i++)
        {
            int r;
            char x;
            r = rnd() % 2;
            if (r == 1)
                x = rnd() % ('Z' - 'A' + 1) + 'A';
            else
                x = rnd() % ('z' - 'a' + 1) + 'a';
            s.push_back(x);
        }
        ss.push_back(s);
    }

    cout << ss.size() << endl;
```

将15800个字符串插入布隆过滤器，测试结果如下：

![image-20210920163944639](https://cdn.jsdelivr.net/gh/ZhengQianyi/img/20210920163951.png) 

将15800个字符串插入布谷鸟过滤器，测试结果如下：

![image-20210920164033316](https://cdn.jsdelivr.net/gh/ZhengQianyi/img/20210920164033.png) 

