### Bloom Filter 单元测试

-------------------

#### Bitmap类单元测试

​	首先对底层数据结构Bitmap进行单元测试：

​	①初始化，由于时间原因，还没有做到让bitmap任意可拓展，其可行大小在32bit，到2的32次bit之间。若用户超出范围则默认位2的20次bit。

![image-20210922110018753](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922110018753.png)

​	②基础功能测试：

![image-20210922110459850](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922110459850.png)

-------------------

#### Bloomfilter单元测试

①初始化：

![image-20210922110629574](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922110629574.png)

②功能测试：

![image-20210922110705909](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922110705909.png)

----------------

#### 最终结果

![image-20210922110728961](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20210922110728961.png)

