# SkipList
[![Build Status](https://travis-ci.org/MottoX/SkipList.svg?branch=master)](https://travis-ci.org/MottoX/SkipList)

A Java implementation of [skip list](https://en.wikipedia.org/wiki/Skip_list).

[跳表](https://zh.wikipedia.org/wiki/%E8%B7%B3%E8%B7%83%E5%88%97%E8%A1%A8)的 Java 语言实现。

## Introduction
>Skip lists are data structures that use probabilistic balancing rather than strictly enforced balancing. As a 
result, the algorithms for insertion and deletion in skip lists are much simpler and significantly faster than equivalent algorithms for balanced trees.

跳表是90年代由 William Pugh 发明的一种概率平衡数据结构。跳表的平衡是一种基于随机的非严格平衡。它完全基于链表实现，而不涉及树的结构，和普通的平衡树相比，
跳表的新增和删除操作实现相对简单很多。跳表的核心思想就是将扁平的列表进行分层，每个节点在插入的时候会生成一个随机的层数，基于此随机的层数与跳表中相邻的数据连接。
跳表的增删改查操作具有 O(log N) 的均摊复杂度。

## Implementation
The [SkipListMap](src/main/java/com/github/dataStructure/SkipListMap.java) uses skip list as the underlying essential data structure and 
implements `java.util.Map`.
Two extra sentinel nodes(head and tail) are used for the sake of convenience. The `SkipListMap` is generic and supports
particular comparision function by passing a custom `java.util.Comparator` to the constructor.

[SkipListMap](src/main/java/com/github/dataStructure/SkipListMap.java) 是基于跳表的实现的 map。
基本思路与跳表[原著论文]( skiplist.pdf )一致，在实现时为了方便起见，加入了 head 和 tail 两个哨兵节点。

## Todo List
* Implement `java.util.NavigableMap` interface.
* Add more test cases.

## Reference
* [Skip lists: a probabilistic alternative to balanced trees](http://dl.acm.org/citation.cfm?id=78977)
* [Redis ZSET implementation](https://github.com/antirez/redis/blob/4.0/src/t_zset.c)
* *java.util.concurrent.ConcurrentSkipListMap* By Doug Lea

## License
[MIT License](LICENSE)
