## Week4-5 Buffer Pool

### 一、理论背景知识

#### Buffer Pool 的作用

​	Buffer Pool主要负责数据在disk和memory之间的传输，一般而言，商用的数据库系统都设计了自己的buffer pool来负责数据在内存和存储设备上的传输。Buffer Pool的总体设计目标是：

1. 优化空间局部性，使得磁盘是尽可能的顺序读/写
2. 时间控制：尽量减少硬盘读写等待，这需要控制page的读入时机和evict时机

​	使用自己设计的Buffer Pool后，大部分数据库系统在使用文件系统接口时都会使用O_DIRECT标记来跳过文件系统的page cache。原因是：

1. 如果同时存在buffer pool+page cache，在内存中就存在冗余的page copy
2. 不同的OS采用不同的策略，将导致数据库在不同的OS上有不同的性能表现

#### Buffer Pool 的结构

<img src="https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/24953980-cfc406fc2ed6202c.png" alt="img" style="zoom: 50%;" />

​	如图所示，buffer pool是数据库系统向OS申请的一块内存空间，数据库系统将这个空间以frame为单位进行划分管理。frame对应文件中的page，其大小是相等的。数据库将通过系统调用，将数据从存储设备拷贝到frame中，frame中的字节和存储设备上的字节是完全一样的。

​	当需要访问存储介质上的数据时，数据库其他模块可向Buffer Pool申请访问page。数据库系统通过一个page table来映射page id到buffer pool中的frame位置，如果没有在page table中找到需要的page，则说明page miss。如果在Buffer Pool已满的情况下需要访问新的page，则需要执行页面置换策略。

#### Buffer Pool 置换策略

内存页面置换算法主要有以下几种：
* **先进先出置换算法（FIFO）**

   算法思想：每次选择淘汰的页面是最早进入内存的页面。

* **最近最久未使用置换算法（LRU）**

  算法思想：每次淘汰的页面是最近最久未使用的页面。

* **时钟置换算法（CLOCK）**

  算法思想：为每个页面设置一个访问位，再将内存中的页面都通过链接指针链接成一个循环队列，以类似时钟扫描的方式对页面进行置换操作。

在本项目中，要求实现的是**LRU置换算法**。

LRU（Least recently used，最近最少使用）算法根据数据的历史访问记录来进行淘汰数据，其核心思想是“如果数据最近被访问过，那么将来被访问的几率也更高”。

LRU最常见的实现是使用一个链表保存缓存数据，如图所示

<img src="https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/5682416-3a5d7333c349fd44.png" alt="img" style="zoom: 67%;" />

​	LRU置换器每次将最近访问的数据和新加入的数据加入缓冲链表，每次需要淘汰时则选取链表尾部的页面，这是最为朴素的一种LRU实现。这种实现主要存在两个问题：

（1）预读失效；

（2）缓冲池污染；

​	预读失效是指，由于预读(Read-Ahead)操作，提前把页放入了缓冲池，但最终数据库并没有从页中读取数据，称为预读失效。要优化预读失效，思路是：（1）让预读失败的页，停留在缓冲池LRU里的时间尽可能短；（2）让真正被读取的页，才挪到缓冲池LRU的头部。其具体解决方法为，将LRU分为两个部分，新生代和老生代。新页（例如被预读的页）加入缓冲池时，只加入到老生代头部，如果数据真正被读取（预读成功），才会加入到新生代的头部，如果数据没有被读取，则会比新生代里的“热数据页”更早被淘汰出缓冲池。

![image-20211010221704096](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20211010221704096.png)

​	缓冲池污染是指，当某一个数据访问请求，要批量扫描大量数据时，可能导致把缓冲池的所有页都替换出去，导致大量热数据被换出，数据库性能急剧下降，这种情况叫缓冲池污染。缓冲池污染的解决方法为，加入一个“老生代停留时间窗口”的机制，插入老生代头部的页，即使立刻被访问，并不会立刻放入新生代头部，只有满足“被访问”并且“在老生代停留时间”大于一定时间，才会被放入新生代头部。

![image-20211010222014123](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20211010222014123.png)

![image-20211010222054157](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20211010222054157.png)

## 二、代码实现部分

### 实现要求

#### LRU Replacer

​	实现一个页面置换器，利用LRU置换策略决定一个需要换出的页面，并将该页面的frame传给上层（buffer pool）。如果一个页面被钉住了（Pinned），说明此页面正在被使用，需要将其移出置换列表。

​	LRU Replacer主要需要实现以下接口：

-  `Victim(T*)` ：对于被 `Replacer` 跟踪的页面，选出最近访问次数最少的进行删除，将其内容（frame id）存储在输出参数中，并返回 `TRUE`。如果 `Replacer` 为空则返回 `False`。
-  `Pin(T)` ：此方法在页面被固定到 `BufferPoolManager` 中的 `frame` 之后被调用。它应该从`LRUReplacer` 中删除包含固定页面的 `frame`。
-  `Unpin(T)` ：当一个页面的 `pin` 计数降为0以后调用此方法，将包含此页面的 `frame` 加入 `LRUReplacer` 跟踪的 `unpin` 页面中。
-  `Size()` ：此方法返回在 `LRUReplacer` 中所包含的框架数量。

#### Buffer Pool Manager

​	实现具体的缓冲池管理器，对内存中的页面进行管理，包括创捷页面，换入换出页面等。换出页面时，利用前面所实现的置换器选择页面进行换出。实现此管理器不需要关注具体操作磁盘数据的细节，项目已经提供了相应接口（Disk Manager）。

​	Buffer Pool Manager主要需要实现以下接口：

-  `FetchPageImpl(page_id)` ：从磁盘上获取页面，将其复制到内存中；
-  `NewPageImpl(page_id)` ：申请一块新的页面；
-  `UnpinPageImpl(page_id, is_dirty)` ：页面使用完毕时调用，将一个页面的`pin`计数减一，若计数为零，则放入可置换队列当中；
-  `FlushPageImpl(page_id)` ：将脏页面刷盘；
-  `DeletePageImpl(page_id)` ：删除一个页面；
-  `FlushAllPagesImpl()` ：将所有页面刷盘。

-------------------------

### 实现细节

![BufferPool (1)](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/BufferPool (1).png)

#### LRU Replacer

​	采用一个**双向环形链表**，配合**Hash Map**来进行底层数据的管理。双向环形链表可以确保访问队头（插入新页面）和队尾（选择要换出的页面）的时间复杂度均为O(1)，而Hash Map负责记录所有列表中 `frame` 的指针，确保在执行 `Pin` 操作时可以快速找到目标页面，时间复杂度同样为O(1)。

#### Buffer Pool Manager

Buffer Pool Manager主要内置了如下几个组件：

* 空闲页面列表（free list）
* LRU置换器（LRU Replacer）
* 磁盘管理器（Disk Manager）- 项目提供

* 日志管理器（Log Manager）- 项目提供

当Buffer Pool需要将新的页面放入内存时，主要执行如下步骤：

![image-20211010232737441](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20211010232737441.png)

当Buffer Pool需要换出页面时，会首先检查该页面是否被标记为已修改（is_dirty），若为脏页面，则先刷盘。而当Buffer Pool删除页面时，将空闲frame存回空间列表。

### 测试结果

#### LRU Replacer

![image-20211010233152444](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20211010233152444.png)

#### Buffer Pool Manager

![image-20211010233212516](https://flaggyellows-bucket-for-typora.oss-cn-shanghai.aliyuncs.com/img/typora/image-20211010233212516.png)

