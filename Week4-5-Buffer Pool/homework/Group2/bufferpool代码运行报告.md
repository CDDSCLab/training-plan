# Buffer Pool代码运行报告

2021.10.8

by Group2
陈思芹，郑前祎，侯磊，王昊龙

## 1. 陈思芹

### 测试结果

均顺利通过本地测试用例的测试：

LRUreplacer：

![](./images_csq/localtest_lrureplacer.png)

LRUreplacer：

![](./images_csq/localtest_bufferpool.png)

### 测试用例遇到的问题

##### LRUreplacer

1. 容器size的不匹配问题：Unpin两次的时候，把frame都加入了list

  ```
3: /files/bustub/test/buffer/lru_replacer_test.cpp:33: Failure
3: Expected equality of these values:
3:   6
3:   lru_replacer.Size()
3:     Which is: 7
```
解决：在Unpin函数中，加入对元素是否在list中的检查。如果在list则不做元素的添加。（可能是程序错误导致的）

##### buffer_pool_manager

1. 内存错误

  ```
==13391==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000
```
解决：此报错一般是指针引用出现了问题。这里将判断指针为空的方式从```if(！point)```改为```if(point!=nullptr)```。

2. 忘记将被替换的页面从页表中删除。使用如下语句删除旧页和增加新页：

  ``` C++
page_table_.erase(page_id_replace);
page_table_.insert({page_id_new,frame_id_replace});
```

3. 在替换页面后，得到了一个frame_id。使用如下代码从page中取出该Page进行新的初始化：
  ``` C++
auto page_new=pages_+frame_id_replace;
page_new->ResetMemory();
```
