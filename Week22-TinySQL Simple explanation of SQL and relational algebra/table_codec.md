# Table Codec

## 项目介绍

[项目链接](https://github.com/talent-plan/tinysql/blob/course/courses/proj1-part2-README-zh_CN.md)

### 项目背景

TiDB 是 SQL over NoSQL 的数据库, 底层存储采用的是分布式 KV 存储引擎 TiKV, TiDB 通过将 SQL 转换为 KV 操作, 从而实现 SQL over NoSQL 的功能.

本次 Project 的目的是理解 TiDB 中的 Table Codec, 以及 TiDB 中的数据存储方式, 并在 TinySQL 上实现一个简单的 Table Codec.

### TinySQL 的数据存储方式

存储数据需要的性质:

1. 从单表操作的角度看, 同一张表的数据应该存放在一起, 避免在处理某一张表时读取到其他表上的数据.
2. 对于同一张表内的数据, 如果类似 person = "xx" 的等值查询较多，那么一个类似 hash 表的排列是比较优的, 也可以采用有序数组; 如果类似 number >= "xx" 的查询较多, 那么一个类似有序数组的排列是比较优的.
3. 对于同一行上的数据, 如果同一行上的数据总是需要同时被读取, 那么将这些数据存储在一起是比较优的. 在 TinySQL 中, 我们选择将同一行上的数据存放在一起.

总的来看, 一个类似有序数组的排列可能是最简单的方式, 它可以用一个统一的方式满足几乎所有要求.

Tuple 的编码方式:

``` shell
    Key： tablePrefix_tableID_recordPrefixSep_rowID
    Value: [col1, col2, col3, col4]
```

唯一索引元素的编码方式:

``` shell
    Key: tablePrefix_tableID_indexPrefixSep_indexID_indexColumnsValue
    Value: rowID
```

非唯一索引元素的编码方式:

``` shell
    Key： tablePrefix_tableID_recordPrefixSep_rowID
    Value: [col1, col2, col3, col4]
```

## 代码实现问题

### 在 Decode 时需要考虑 key 不合法的情况

在 Decode 时需要考虑以下三种不合法的情况:

1. Key 的长度是否合法

    ``` C++
        if len(key) < RecordRowKeyLen {
            return 0, 0, errInvalidRecordKey.GenWithStack("invalid record key - %q", key)
        }
    ```

2. Key 中是否包含特定字符串常量 tablePrefix

    ``` C++
        if !startWithTablePrefix(key) {
            return 0, 0, errInvalidRecordKey.GenWithStack("invalid record key - %q", key)
        }
    ```

3. Key 中是否包含特定字符串常量 recordPrefixSep

    ``` C++
        if !startWithRecordPrefix(key) {
            return 0, 0, errInvalidRecordKey.GenWithStack("invalid record key - %q", key)
        }
    ```

### 为什么在编码 `tableID` 和 `handle` 等 `int64` 类型为 `[]byte` 类型时, 需要使用 `EncodeIntToCmpUint()` 函数变为 `uint64` 类型?

``` Go
func EncodeInt(b []byte, v int64) []byte {
    var data [8]byte
    U := EncodeIntToCmpUint(v)
    binary.BigEndian.PutUint64(data[:], u)
    return append(b, data[:]....)
}

func EncodeIntToCmpUint(v int64) uint64 {
    return uint64(v) ^ signMask
}
```

主要原因: 为了保证编码后的 `[]byte` 类型的 key 有序. 如果直接将有符号整数类型编码为字节数组, 那么在比较操作中可能会出现错误的结果.

比如现在有两个数 a=-1, b=1, 编码之前 a<b; 但是编码之后 a=[255 255 255 255 255 255 255 255], b=[0 0 0 0 0 0 0 1], a>b, 违背了编码之前和编码之后比较关系不变的原则.

## 理论思考讨论

### 从 Join 的角度考虑, 数据应该怎么映射?

一般来说, 关系型数据的 Join 实现有三种: Hash Join, Sort Merge Join, Nested Loop Join.

Hash Join: 在较小的表上建立 Hash Table, 然后对较大的表进行遍历, 从 Hash 索引中查找匹配的数据.

Sort Merge Join: 将两个表的数据分别排序, 然后对两个表进行 Merge Join 操作. 如果列上有索引, 则可以直接进行 Merge Join 操作.

Nested Loop Join: 从一张表中读取一行数据 (outer table), 然后访问另一张表 (inner table, 通常有索引, 比 outer table 大) 进行匹配, 类似一个嵌套的循环.

总的来说, 我们可以在 Join 列上建立索引, 对 Join 进行优化.

### TiDB 为什么要采用 SQL over NoSQL 的架构? 与使用对象存储作为底层存储相比, 有什么优势?

TiDB, CockroachDB, YugabyteDB 等 NewSQL 数据库都采用了 SQL over NoSQL 的架构, 底层存储采用了 分布式 KV 存储引擎.

![tidb](/Users/moon/CLionProjects/training-plan/Week22-TinySQL Simple explanation of SQL and relational algebra/tidb.png)

PolarDB, Snowflake 等云原生数据库采用了对象存储作为底层存储.

![snowflake](/Users/moon/CLionProjects/training-plan/Week22-TinySQL Simple explanation of SQL and relational algebra/snowflake.png)

SQL over NoSQL 的架构相比于对象存储的架构有哪些优势呢?

1. SQL over NoSQL 架构能很方便地支持谓词下推, 但目前某些对象存储也支持谓词下推.
2. SQL over NoSQL 架构通常采用三副本的方式进行数据冗余, 对象存储通常采用纠删码. 纠删码的空间放大相比于三副本的方式更小 (EMC对象存储系统ECS 12+4 和 10+2 冗余度分别为 1.33 和 1.2
阿里云盘古集群chunk存储 8+3 冗余度=1.375), 能够以更低的成本实现近似三副本的可靠性. 但纠删码编码和解码计算时需要消耗 CPU 资源, 三副本的方式则不需要. 因为采用了纠删码, 对象存储支持的操作类型也十分有限, 数据写入后通常不能修改, 不能 append, 需要在计算层做大量缓存. 而且, 对象存储的读写延迟通常比较高, 通常在 10ms 以上.

### TiDB 采用了 将同表数据存储在一起, 同表中的行按有序排列的方式, 其他采用 KV 引擎的数据库是如何选择的?

在新版本的 TiDB 中, Tuple 编码时将 rowID 替换为了 primary key.

// 同表内也可以按列进行存储?

### 表内的 Row 和 Index 是怎样排列在 TiKV 中的?

在本项目 Row 和 Index 的编码方案中, 一个 Table 内部所有的 Row 都有相同的前缀, 一个 Index 的数据也都有相同的前缀. 这些具有相同前缀的数据在 TiKV 的 Key 空间内是排列在一起的.

TiDB 采用了名为 MemComparable 的方案, 保证编码前后 Key 的比较关系不变, 从而保证编码前后 Key 的有序性. 采用这种编码后, 某个表的所有 Row 数据就会按照 RowID 的顺序排列在 TiKV 的 Key 空间中, 某个 Index 的数据也会按照 Index 的 ColumnValue 顺序排列在 Key 空间内. (具体案例可以参照 为什么在编码 `tableID` 和 `handle` 等 `int64` 类型为 `[]byte` 类型时, 需要使用 `EncodeIntToCmpUint()` 函数变为 `uint64` 类型?)

### TiDB 为什么选择 LSM-Tree 作为底层存储引擎?

1. LSM-Tree 将所有的随机写都转换为了顺序写, 写性能更好 (读写性能权衡上偏向写).
2. LSM-Tree 对压缩更友好, 相比之下, 占据的存储空间更小.
3. LSM-Tree 相比 B+Tree 更易于实现, 更易于做并发控制.
4. RocksDB 作为 LSM-Tree 的实现, 有很多优秀的特性, 能很好地适应现代硬件 (如 SSD).

B+Tree 通常只能以 Page 为单位进行压缩, 一个 Page 通常为 4KB, 8KB 或 16KB, 压缩效率不高. 而 LSM-Tree 通常以 SSTable 为单位进行压缩, 压缩效率更高.

[Closing the B+-tree vs. LSM-tree Write Amplification
Gap on Modern Storage Hardware with Built-in
Transparent Compression](https://www.usenix.org/system/files/fast22-qiao.pdf) 这篇论文介绍了利用 内置透明压缩技术的新硬件, 将 B+Tree 的写放大降低到和 LSM-Tree 相同的水平.

参考: [什么是透明压缩? 具备透明压缩的 SSD 有什么不同?](https://zhuanlan.zhihu.com/p/470052587)
