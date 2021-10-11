# Week4-5 Buffer Pool

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
