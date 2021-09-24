### Cuckoo Filter 需求分析

---------------------

#### 底层数据结构

​	一个大小为2的16次的桶列表，每一个桶能放两个指纹。其中前一个存放首选位，后一个存放次选位。指纹长度为4字节。

​	每个桶的结构：

![image-20210922121850145](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922121850145.png)

#### 功能需求

1. 添加元素
2. 查找元素
3. 删除元素

<img src="https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922121701274.png" alt="image-20210922121701274" style="zoom:67%;" />

#### 思考

①布谷鸟过滤器中为元素计算的指纹过程和哈希有什么区别？

计算指纹的过程应当是不可逆的。假如有人恶意构造一个元素不同，但是指纹相同的列表，则可以快速使布谷鸟过滤器失去过滤功能。因此应该使指纹无法被逆运算，可采用MD5，SHA等算法。

#### 算法

f = fingerprint(x);
i1 = hash(x);
i2 = i1 ⊕ hash(f);

Insert：

```c++
Algorithm 1: Insert(x)
f = fingerprint(x);
i1 = hash(x);
i2 = i1 ⊕ hash(f);
if bucket[i1] or bucket[i2] has an empty entry then
	add f to that bucket;
	return Done;
// must relocate existing items;
i = randomly pick i1 or i2;
for n = 0; n < MaxNumKicks; n++ do
	randomly select an entry e from bucket[i];
	swap f and the fingerprint stored in entry e;
	i = i ⊕ hash(f);
	if bucket[i] has an empty entry then
		add f to bucket[i];
		return Done;
// Hashtable is considered full;
return Failure;
```

Lookup：

```c++
Algorithm 2: Lookup(x)
f = fingerprint(x);
i1 = hash(x);
i2 = i1 ⊕ hash(f);
if bucket[i1] or bucket[i2] has f then
	return True;
return False; 
```

Delete：

```c++
Algorithm 3: Delete(x)
f = fingerprint(x);
i1 = hash(x);
i2 = i1 ⊕ hash(f);
if bucket[i1] or bucket[i2] has f then
	remove a copy of f from this bucket;
	return True;
return False;
```

#### Bucket 类需求分析

Bucket类为布谷鸟过滤器提供底层数据存储支持。

##### 功能需求

①检查目标位置是否可用（提供下标和偏移）

②存储元素指纹（提供下标、偏移和指纹）

③获取目标位元素指纹（提供下标和偏移，返回指纹）

④删除目标位元素指纹（提供下标和偏移）

#### 类图

![image-20210922122847213](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922122847213.png)
