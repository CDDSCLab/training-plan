# B-Tree
可以参考 [dataStructure-Study](dataStructure_Study.md) 中的“Data Structure 2：B-Tree 的学习”部分
## Introduction

 - B树定义：在计算机科学中，B树（英语：B-tree）是一种自平衡的树，能够``保持数据有序``。这种数据结构能够让查找数据、顺序访问、插入数据及删除的动作，都在对数时间内完成。B树，概括来说是一个一般化的``二叉查找树（binary search tree）``一个节点可以拥有2个以上的子节点。与自平衡二叉查找树不同，B树适用于读写相对大的数据块的存储系统，例如磁盘。B树减少定位记录时所经历的中间过程，从而加快存取速度。B树这种数据结构可以用来描述外部存储。这种数据结构常被应用在数据库和文件系统的实现上。([维基百科](https://zh.wikipedia.org/wiki/B%E6%A0%91))
 - 一个 m 阶的 B 树是一个有以下属性的树：
    1. 每一个节点最多有 m 个子节点
    2. 每一个非叶子节点（除根节点）最少有 ⌈m/2⌉ 个子节点
    3. 如果根节点不是叶子节点，那么它至少有两个子节点
    4. 所有的叶子节点都在同一层
    5. 每个非叶子节点由 n 个 key 和 n+1 个指针组成，其中 ⌈m/2⌉-1 <= n <= m-1

## Implementation
使用 JAVA 实现 B-Tree 包含 5 个类，下图是简单的 UML 模型，包含每一个类中的关键变量和关键方法。

![](https://cdn.jsdelivr.net/gh/Beeter-yong/pictures/imgOne/BTreeImplement.png)

- Constants.java：此类是常量配置类，在 m 阶 B-Tree 中设置 m 的值，以及控制每一个节点中的 key 数 n 符合 ⌈m/2⌉-1 <= n <= m-1。
- SearchResult.java：B-Tree 中最常用的操作是搜找行为，对于查询结果可能出现两种情况，即是否找到该 key，因此该类用于结果的判断，以及查找到结果后返回结果的索引和没有查到结果返回待插入索引的位置。
- Entry.java：此类是存储元素的对象，其中有整型 key 作为关键字，value 可以用来存储字符串，也可以再添加其他类型，修改此类即可。
- BTNode.java：此类是以一个对象形式充当 B-Tree 的节点。所以包含该节点的父节点和子节点以及节点中包含的键值。并有相应的方法，包括设置父节点、孩子结点、查找 key 等行为。
- BTree.java：该类是树的数据结构，包含方法有实例化树、插入元素、寻找节点、打印树等。

## Todo List
在 test 包下的 BTreeTest.java 用于测试树的添加元素、打印元素、打印树和查找、移除元素等。