### Bloom Filter 性能测试

性能测试代码如下所示：

![image-20210924110611301](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210924110611301.png)

主要测试两项指标：运行时间和假阳性率；其中输入参数num表示输入元素的个数。

![image-20210924110550543](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210924110550543.png)

实验结果如下：

![image-20210924110535017](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210924110535017.png)

经测试，布隆过滤器总体性能较好，在元素个数达到30万时，（元素个数*哈希数）超过了底层位图的比特位数，误判率仍维持在较可接受的水平。

----------

#### 问题&后续目标

完善的布隆过滤器应当根据预期的元素个数动态调整位图大小以及哈希个数，这一点在设计过程中没有考虑到，后续应当添加这个功能，只需在构造函数中添加预期元素个数的参数，并动态调整哈希函数的种子列表大小即可。

