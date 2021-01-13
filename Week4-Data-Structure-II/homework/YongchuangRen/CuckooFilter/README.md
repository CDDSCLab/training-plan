# JAVA 实现 Cuckoo Filter
基于 JAVA 语言使用 IDEA 简单实现布谷鸟过滤器，包含三个主要文件

![](https://cdn.jsdelivr.net/gh/Beeter-yong/pictures/imgOne/cuckooFilterImp.png)
- Hash.java：该类存放两个哈希函数，一个作用是生成插入数据对象的一字节指纹，另一作用是定位数据位置哈希函数
- Keys.java：是一个用于存储数据的类，里面定义一个数组，需要传参来确定数组的初始化大小。
- CuckooFilter.java：是一个布谷鸟过滤器数据结构的实现类，包含元素的插入、查找和删除。