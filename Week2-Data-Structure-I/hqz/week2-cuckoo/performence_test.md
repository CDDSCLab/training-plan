### Cuckoo Filter 性能测试

性能测试代码如下所示：

![image-20210924111849064](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210924111849064.png)

主要测试三项指标：运行时间、插入失败次数和假阳性率；其中输入参数num表示输入元素的个数。

![image-20210924111935922](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210924111935922.png)

实验结果如下：

![image-20210924112000839](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210924112000839.png)

​	为了和布隆过滤器作比较，布谷鸟大小采用的大小是：（布隆过滤器比特位 / 布谷鸟指纹大小）。与预期不同的是，即使是占用小于百分之三的情况，布谷鸟过滤器的性能也差于布隆过滤器。这种情况很可能是因为我的布谷鸟实现计算指纹采用了MD5算法，其复杂性超过了布隆过滤器对哈希的计算，导致性能变差。布谷鸟的实现还需要进一步优化。

