# 项目问题

1. Bitmap 操作

2. 关于0结构数组或1结构数组的应用

```cpp
char occupied_[(BUCKET_ARRAY_SIZE - 1) / 8 + 1]{0};
char readable_[(BUCKET_ARRAY_SIZE - 1) / 8 + 1]{0};
MappingType array_[0];
```

<img src="./Hash Table Index-1.assets/image7.jpeg" alt="image7" style="zoom:50%;" />

这是一个Zero-length array（零长度数组）。它位于结构体/类的最后，本身不占用空间。在声明结构体的时候分配内存，去除掉其他元素的部分就是零长度数组可以访问的部分。而类的实例化是不能定义大小的。但是，我们的类比较特殊，它的实际意义是一个页面，而页面的大小是固定的。从测试代码可以看到该类实例化是这样的

```cpp
auto directory_page = reinterpret_cast<HashTableDirectoryPage *>(bpm->NewPage(&directory_page_id, nullptr)->GetData());
```

3. 为什么`HashTableDirectoryPage`的`bucket_page_ids_`数组和`local_depths_`数组的长度设置为512？

一般来说，长度为2的倍数，如果再打1024，就会超出page的大小

4. 在比较两个键值对时，为什么key需要有比较器，value直接使用"=="？

key 的类型不同，vaule 的值相同

# 扩展问题

hash 函数的雪崩效应（看情况，有时候需要，有时候选择放弃）