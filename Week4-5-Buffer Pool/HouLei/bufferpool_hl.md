## Buffer Pool学习笔记

Project地址：https://15445.courses.cs.cmu.edu/fall2020/project1/

主要需要实现两个部分：

LRU置换和Buffer Pool Manager

### LRU置换实现

LRU：最近最少使用置换算法，即选择最近最久未使用的页面予以淘汰。

实现：创建一个队列，依次把最近使用过的页面加入队头，每次需要置换时从队尾取出。使用list存储即可，为了减少复杂度增加一个Map。

简单实现 就是为了后面buffer pool manager做准备

Unpin()指的是从buffer pool中取消 在lru_replacer中添加一个元素 (Pin()相反)

Victim()取出队尾节点，并在map中删除它的映射关系

### Buffer Pool Manager

主要有三个部分组成:

free_list：存放当前空闲的frame的队列

page_table：保存page->frame的映射

pages：所有的页面(?为什么不叫frames)

- frame是buffer pool实际拥有的空间，每一个frame在整个程序进程中可能维护不同的page(来自磁盘的不同页)

- 使用pages + frame_id 访问指定page

- 具体函数的实现在CMU的代码注释里给出了详细的步骤，主要是要理解好页面状态的关系。

- 一个frame会有3个状态：Pin/Unpin/free_list，在Pin/Unpin时都要在page_table中维护映射关系，因为Unpin只是指页面暂时没有进程访问，但仍然在Buffer pool中。


### 遇到的问题

- 一开始测试的过程比较困难，只能在给出的GoogleTest中修改用例来实现，熟悉之后还好。
- 实现的过程中把frame和page的概念搞混淆了，应该先仔细阅读一下文档理清思路再开始。

### 其他

并发安全：

之前没怎么写过，简单的使用了std::mutex，在每个函数的头尾进行加锁解锁，肯定有不需要锁的部分，影响了效率，后续可以进行优化。

