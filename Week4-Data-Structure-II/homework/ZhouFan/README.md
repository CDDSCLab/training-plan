# Bloom Filter-java实现 

开发环境：eclipse 

###  Bloom Filter相关    

- 组成    

  一个二进制向量与k个hash函数

- 优点

  - 空间效率高(存放的不是完整数据，存放的是二进制）
  - 查询快(查询和插入的时间复杂度都为常数)
  - hash函数相互之间没有关系，方便由硬件并行实现  
  - 可以表示全集，其他任何数据结构都不能(布隆过滤器将位数组设为1，就可以表示全集，表示全部元素都存在) 

- 缺点


  - 误算率( 本来不存在集合中的元素，却被认为存在与集合中)
  - 不能删除元素

###    Counting BloomFilter相关

- 目的

  - 解决BloomFilter不能删除的问题  

- 方法   

  - 将位数组中的每一位由 bit 扩展为 n-bit 计数器，实际上，基本的布隆过滤器可以看作是只有一位的计数器的Counting filters  

- 代码

  - private BitSet bitSet=new BitSet(*BloomLength);(BloomFilter）

  - private Map<Integer, Integer> bloomFilterCountMap = new HashMap<>();（Counting BloomFilter)

    BitSet为位数组，每个值只有两种情况(0或1即true or false)

    Map<Integer, Integer>为key-value集合，每个key对应一个value

     

  ### Counting BloomFilter相关实现 

  这里只写了大概思路，无代码

-  插入：将这个元素计算的k个hash函数的对应位置加1
-  删除：将这个元素计算的k个hash函数的对应位置减1
-  查找：这个元素计算的k个hash函数的对应位置是否全都大于0  
-  测试：测试30万数据，当k=2时，冲突79908个数据，冲突率=79908/300000=0.2。当k=16时，冲突31171个数据，冲突率=31171/300000=0.1。其实冲突率还是很高的，问题在于hash函数选得不好。

#    CuckooFilter-java实现 

### CuckooFilter相关

​       主要提出是为了解决不能删除的问题，并且提升了空间性能

 ###  基本结构   

​       一个桶，2个hash函数，8个候选巢，最多存28个元素，每一行的hashcode相同，每个巢存的是该数

​      据的指纹，指纹为一个字节

 ### CuckooFilter相关实现 

- 插入：当候选巢为空时，直接插入该数据的指纹
- 再插入：当所有候选巢都不为空时，这时需要强占一个候选巢，被强占的元素会继续寻找空的候选巢，如此循环下去，当达到一个最大循环上限时，就返回false
- 删除：因为巢存的是指纹，所以去8个候选巢里找到该元素的指纹，然后删除  
- 查找：去8个候选巢里查找该元素的指纹，如存在则查找成功，反之，查找失败
- 扩容：当哈希表被填满，这时需要扩容，方法是重新申请一个大的空间，然后将原来的哈希表数据复制到新的表中，之后重新进行插入  



