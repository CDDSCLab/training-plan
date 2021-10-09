# Buffer Pool代码运行报告

2021.10.8

by Group2
陈思芹，郑前祎，侯磊，王昊龙

---

## 1. 陈思芹

### 测试结果

均顺利通过本地测试用例的测试：

LRUreplacer：

![](./images/localtest_lrureplacer.png)

LRUreplacer：

![](./images/localtest_bufferpool.png)

课程网站的测试达到150分，除内存安全外均通过。

![](./images/graderesult.png)

### 遇到的问题

##### LRUreplacer

1. 容器size的不匹配问题：测试用例采用了Unpin两次，程序将frame都加入了list

  ```
3: /files/bustub/test/buffer/lru_replacer_test.cpp:33: Failure
3: Expected equality of these values:
3:   6
3:   lru_replacer.Size()
3:     Which is: 7
```
解决：在Unpin函数中，加入对元素是否在list中的检查。如果在list则不做元素的添加。（一般不会出现Unpin两次这种情况，但是测试用例就加入了这样的情况）

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


3. 加锁以防止多线程调用，在各函数的具体实现前加上，如：
``` C++
Page *BufferPoolManager::FetchPageImpl(page_id_t page_id) {
  std::lock_guard<std::mutex> lock(latch_);
  ...
}
```

4. 误在FetchPage、FlushAllPages函数中调用FlushPage，造成错误。注意加锁后的函数不能相互调用彼此，不然会出现死锁情况。

5. 忽略了INVALID_PAGE_ID：在flushPage实现前应该检查page_id是否无效；在deletePage时应将被删除的页的page_id设置成无效。

6. 在deletePage时也要调用replacer->Pin()，将页面从替换帧列表中清除。

### 总结

通过动手编程实践，我对缓存池的原理有了较为深入了解，C++的编程能力也得到了提升。小组同学都很厉害（至少比我这个菜鸟强），通过和小组同学的交流讨论、不断请教，我遇到的许多问题都及时得到了解决，从他们身上也学到很多学习方法和编程、调试技巧。总体来说项目收获非常大，很有成就感。

---

## 2.郑前祎


### 实现及测试

本地测试：

 ![image-20211008173110540](https://raw.githubusercontent.com/zqyi/img/master/20211008180017.png)

 ![image-20211008173147272](https://raw.githubusercontent.com/zqyi/img/master/20211008180029.png)

在线测试：

![image-20211008173528428](https://raw.githubusercontent.com/zqyi/img/master/20211008180024.png)

![image-20211008180937037](https://raw.githubusercontent.com/zqyi/img/master/20211008180937.png)



### 过程中遇见的问题及解决

#### 问题一

LruReplacer无法通过测试。

错误原因：在对Unpin函数的处理上理解有误。正确的理解应该是调用Unpin时，如果Lru里没有该page_id，则将该page_id加入，如果已经有这个page_id则不进行处理。

解决：修改Unpin函数的实现。

#### 问题二

使用细粒度锁以实现更高的并发性时，会导致死锁。

错误原因：为了得到更高的并发性，选择对类中的私有成员变量单独加锁，则有三个锁：mu_page_table、mu_free_list、mu_page_。在对锁进行申请和释放时，容易造成死锁，并且其中函数嵌套调用时也难以处理锁的正确性。

解决：简化锁的实现，对函数进行加锁。

### 总结

本周的学习过程中，结合BufferPool的项目，学习了很多数据库的基本知识，并且在此基础上自己去理解和实现，有非常大的收获。在写项目的过程中，学习了很多C++的知识，也极大提高了自己的编程能力，为之后的项目打下了基础。
