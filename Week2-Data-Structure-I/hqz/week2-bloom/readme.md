### Bloom Filter 需求分析设计文档

-------------

#### 功能需求

​	主要有两点：1. 添加元素；2. 查询元素

<img src="https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922095442777.png" alt="image-20210922095442777" style="zoom:67%;" />



#### 功能描述

①Bloom过滤器本身作为一个类存在，初始化Bloom过滤器以后，根据用户指定的位长度生成位图。

②添加元素时，Bloom过滤器首先拉取hash数组hashTable，逐个对元素hash，返回位图bit位下标，对对应下标置1，返回操作结果码S_OK

③查询元素时，Bloom过滤器首先拉取hash数组hashTable，逐个对元素hash，并查询对应bit位是否为1，若为1，则继续hash，若为0，则说明该元素不在集合内，返回False；若检查完hashTable以后全为1则返回True。

*注：

①作为一个Demo，暂时仅支持逐个添加或者查询元素。

②简单起见，先仅支持String。

#### 数据结构

![image-20210922100310280](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922100310280.png)

​	主要拆分为两个类，Bitmap类负责底层数据存取，而Bloomfilter类负责实现主要过滤器功能，这样做的好处是解耦合，方便之后完善和添加功能。

**位图功能描述：**

​	底层通过vector\<int\>进行存储，初始化时，根据用户传入的位图大小调整。为简单起见，传入的参数表示2的左移位数，保证位图长度一定是2的倍数。

* 功能需求：

  ①bit位置1，int set_bit(unsigned int index)

  ②bit位清0，int reset_bit(unsigned int index)

  ③查询bit位，bool query(unsigned int index)

当使用位图时，外部直接传入作为下标的hash值，而bitmap内部可以根据用户设置的大小自动选取合适的下标位置，并将下标转换为bit位所在的int下标以及在int内的偏移。