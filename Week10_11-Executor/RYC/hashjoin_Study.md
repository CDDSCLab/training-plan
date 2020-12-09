# HashJoin
## HashJoin 简介
关系型数据库中经常会需要涉及到 join 的操作，一般来说有三种方案处理 join：Nested Loop Join、Merge Join、Hash Join。本次学习主要针对 Hash Join。

Hash Join 一般分为两个阶段完成
1. Build 阶段：准备一个**哈希表**，选择待连接的一个表，将连接属性通过 hash 函数映射到哈希表中。
2. Probe 阶段：一旦哈希表确认后，扫描另外一张表，并对连接属性进行 hash 函数映射。

## 主要测试数据来源
https://github.com/pingcap/talent-plan/tree/master/tidb/join 中的 t 文件夹中三个文件

## 单线程 HashJoin
根据最基本的原理实现 HashJoin。其中实现中考虑只扫描一次 IO 便完成匹配。

![](https://raw.githubusercontent.com/Beeter-yong/pictures/master/imgTwo/tianruo_2020-9-27-637368347895467706.png)

- File.java：是对读取文件操作的一个封装，主要目的是避免在使用时多次 try/catch
- Enty.java：是 Hash Table 的基本单位类型
  - 使用 num 记录所存储的对象在集合（保存所有表的数据）中的位置，方便 Probe 阶段进行 hashJoin 合并
  - 使用 value 字符串存储 Build 阶段的 hash 属性值。
  - 使用 Entry 类型的 next，是为了构建 HashTable 中每个相同 hash 值得链表。
- HashJoin.java：是整个运算的主体
  - 在构造函数时，需要传入参数包括两个待 hashjoin 表的物理路径，以及两张表待 hash 的属性值是哪一列
  - hashjoin()：是启动函数，以此进行 Build 和 Probe 阶段。
  - hashLeft()：这个函数对应的是 Build 阶段，主要逻辑是一行行读取表中数据，并对取出的字符串进行分割，然后找到待 hash 的关键属性，hash 后存储在 Entry 类型的 HashTable 中。
  - hashRight()：这个函数对应的是 Probe 阶段，它会循环获取表行数据，并对待 hash 的关键属性进行 hash，在 hashTable 中找到对应的 hash 值位置，判断属性值是否相等，如果相等表示 hash 匹配成功进行，通过 Entry 的 num 找到第一张表的数据并 join 输出。

### BenchMark 结果
![](https://raw.githubusercontent.com/Beeter-yong/pictures/master/imgTwo/tianruo_2020-9-27-637368355613670403.png)

## 多线程 HashJoin
此部分实现主要参考资料是[Hahs join 算法原理](https://www.cnblogs.com/qlee/archive/2011/04/11/2012572.html)，但由于测试数据量很少，就不再考虑 IO 优化，但代码稍作更改可以很好的适应 IO 操作。

### 基本解析
为了多并发实现以及优化（在 Build 阶段使用小表，在 Probe 阶段使用大表），所以不可避免要先将两张表导入内存中，从而方便计算两张表的大小。

- 首先操作是将文件导入内存中，在扫描文件期间对文件进行分区处理，即取出每一个表行数据中的 hash 关键属性，并对其进行求余数计算，我将其处理成 4 份，即每一张表分成 4 个分区。所以我对关键属性进行 %4 计算。
  - 这个过程使用了两个线程并发读取两张表，个人认为在测试集中数据量过小可以这样处理，如果数据量过大反而不适合并发实现，因为从 IO 导入内存会使用缓存 cache，如果并发读取，很有可能频繁发生缓存覆盖导致效降低。
- 我们在第一步对每一张表的数据分成了 4 份，所以可以开启 4 个线程分别处理两张表对应的分区。例如第一张表的第二个分区与第二张表的第二个分区进行 hashjoin 即可。
- 在 hashjoin 的过程中，首先 Bulid 阶段与单线程相同，但我在 Probe 阶段同时开启两个线程处理，因为 Build 阶段是在 hashTable 中写数据，并且表数据量相对较少，所以使用单线程即可。而 Probe 阶段数据量相对较大，且只是对 HashTable 读取数据，便将分区分为两个部分，使用两个线程同时处理

### 具体实现
![](https://raw.githubusercontent.com/Beeter-yong/pictures/master/imgTwo/tianruo_2020-9-27-637368369128425437.png)

其中 FileWriter.java 并未编写，本打算将 hashJoin 数据保存到文件中，但数据量相对较少，控制台完全可以展示便不再存储在文件中

- FileReader.java：该类作用同单线程的 File.java 相同，代码相同。
- HashTable.java：该类代码与单线程的 hashjoin 基本相同，主要作用是对传入的两个集合分区进行 hashjoin
  - hashLeft()：该函数就是 Build 阶段
  - hashRight()：该函数是 Probe 阶段，但从中分割成两个线程并发处理。
- HashJoin.java：该类是整个项目的启动类，其中主要实现两个线程并发实现 IO 导入和分区，以及开启 4 个线程并发处理分区的 HashTable.java 中的 hashJoin

### BenchMark 测试
该测试表数据同单线程 HashJoin 相同

![](https://raw.githubusercontent.com/Beeter-yong/pictures/master/imgTwo/tianruo_2020-9-27-637368374672671319.png)

我认为多线程优势还是有的，但由于数据量太小不足以碾压单线程；

其次为什么多线程并发分成四个分区，以及每个 hashjoin 使用两个线程并发，主要原因在于测试电脑是 4 核处理器，根据 inter 的超线程技术可以同时处理 8 个线程，所以使用四个分区以及每个分区再开两个线程刚好同时运行 8 个线程




## 参考资料
[Hahs join 算法原理](https://www.cnblogs.com/qlee/archive/2011/04/11/2012572.html)