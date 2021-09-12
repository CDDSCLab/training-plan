# 实验三 Redis基础知识和实验过程_陈思芹.md

## 三.Redis 基础知识和实验过程

### 概念：

1. 以key-value的方式存储，亦可支持list，set，zset，hash等数据结构。

2. 运行在内存中，读写速度极快。支持数据持久化，即将数据保存回磁盘。

3. 是一个数据结构服务器。value可以存储更为复杂的数据结构。Redis有5种基本的数据类型：字符串（strings）、哈希（hashes）、列表（list）、集合（sets）、有序集合（sorted sets），每种数据类型都有对应的set/get命令。

4. 单机数据库，数据库大小不能超过内存大小。一个redis实例默认支持16个数据库，以数字编号命名，更像是一个命名空间。

5. 可为key设置一个生存期。过期的key会自动销毁。

6. 其他数据类型：位图（bitmap）用于进行位操作，极大节省空间。HyperLogLog用于做基数估计。Geo用于存储地理位置信息。Stream用于消息队列。

### 使用docker搭建redis环境

#### 1. 拉取redis镜像：
```
 $ docker pull redis
```
#### 2. 运行容器：
```
 $ docker run -itd --name some-redis -p 6379:6379 redis
```
 参数说明：

 + some-redis为容器名。


#### 3. 连接容器：
```
$ docker exec -it some-redis /bin/bash
```
#### 4. 连接redis服务并测试数据库：

```
>/data# redis-cli
> 127.0.0.1:6379> set test 1
> OK
```

### 链接

+ try redis，一个可直接试用redis的免安装在线平台: https://try.redis.io/

+ 官方文档（数据类型介绍）:https://redis.io/topics/data-types-intro
+ 菜鸟教程：https://www.runoob.com/redis/redis-data-types.html

### 运行示例：

#### 哈希（hash）：
```
127.0.0.1:6379>  HMSET runoobkey name "redis tutorial" description "redis basic commands for caching" likes 20 visitors 23000
OK
127.0.0.1:6379>  HGETALL runoobkey
1) "name"
2) "redis tutorial"
3) "description"
4) "redis basic commands for caching"
5) "likes"
6) "20"
7) "visitors"
8) "23000"
```

#### 列表（list）：
```
127.0.0.1:6379> del runoobkey
(integer) 1
127.0.0.1:6379> lpush runoobkey redis
(integer) 1
127.0.0.1:6379> lpush runoobkey mongodb
(integer) 2
127.0.0.1:6379> rpush runoobkey mysql
(integer) 3
127.0.0.1:6379> lrange runoobkey 0 10
1) "mongodb"
2) "redis"
3) "mysql"
127.0.0.1:6379> lpop runoobkey
"mongodb"
127.0.0.1:6379> lrange runoobkey 0 -1
1) "redis"
2) "mysql"
127.0.0.1:6379> llen runoobkey
(integer) 2
127.0.0.1:6379>
```

#### 集合（set）：
```
127.0.0.1:6379> del runoobkey
(integer) 1
127.0.0.1:6379> sadd runoobkey redis
(integer) 1
127.0.0.1:6379> sadd runoobkey 1 2 3
(integer) 3
127.0.0.1:6379> sadd runoobkey mongodb
(integer) 1
127.0.0.1:6379> smembers runoobkey
1) "3"
2) "1"
3) "redis"
4) "2"
5) "mongodb"
127.0.0.1:6379> sismember runoobkey 3
(integer) 1
```
#### 有序集合（sorted set）：
```
127.0.0.1:6379> zadd hackers 1940 "Alan Kay"
(integer) 1
127.0.0.1:6379> zadd hackers 1957 "Sophie Wilson"
(integer) 1
127.0.0.1:6379> zadd hackers 1953 "Richard Stallman"
(integer) 1
127.0.0.1:6379> zadd hackers 1949 "Anita Borg"
(integer) 1
127.0.0.1:6379> zadd hackers 1965 "Yukihiro Matsumoto"
(integer) 1
127.0.0.1:6379> zadd hackers 1914 "Hedy Lamarr"
(integer) 1
127.0.0.1:6379>  zadd hackers 1916 "Claude Shannon"
(integer) 1
127.0.0.1:6379>  zrange hackers 0 -1
1) "Hedy Lamarr"
2) "Claude Shannon"
3) "Alan Kay"
4) "Anita Borg"
5) "Richard Stallman"
6) "Sophie Wilson"
7) "Yukihiro Matsumoto"
127.0.0.1:6379> zrangebyscore hackers -inf 1950
1) "Hedy Lamarr"
2) "Claude Shannon"
3) "Alan Kay"
4) "Anita Borg"
127.0.0.1:6379> zremrangebyscore hackers 1940 1960
(integer) 4
```
#### 命令总结：

###### 1. ```redis-cli``` 进入redis客户端

###### 2. 对于字符串（string）：
是 redis 最基本的类型，是二进制安全的。可以包含任何数据。比如jpg图片或者序列化的对象。
+ ```SET key value``` 设置或更新指定key的值，
+ ```GET key``` 获取指定key的值，
+ ```DEL key``` 在key存在的时候删除key和其关联值，
+ ```MSET key1 value1 key2 value2...``` 设置或更新多个值，
+ ```MGET key1 key2...``` 获取多个key所对应值，

+ ```EXIST key``` 返回key是否存在。

###### 3. 对于哈希（hash）：
是一个string类型的字段（field）和值（value）的映射表。
+ ```HMSET key field1 value1 [field2 value2]...``` 设置hash表，
+ ```HGET key field``` 获取指定field的值，

+ ```HGETALL key``` 获取指定key的所有字段和值。

###### 4. 对于列表（list）：
是简单的字符串列表，按照插入顺序排序。可以添加元素到列表的头部（左边）或者尾部（右边）。
+ ```LPUSH key element...``` 在列表左边添加元素，
+ ```RPUSH key element...``` 在列表右边添加元素，
+ ```LRANGE key index1 index2``` 返回索引之间的元素，index可为负，-1为最后一个元素，-2为倒数第二个元素。
+ ```LPOP/RPOP key``` 弹出一个列表元素，
+ ```BLPOP/BRPOP key``` 弹出元素的阻塞版本，当列表为空时会阻塞列表，直到有新元素添加至列表或超时，

+ ```LLEN key``` 获取列表长度。

应用例子：社交网络中获取最新的10篇文章或照片；生产者/消费者场景。

###### 5. 对于集合（set）：
是String类型的无序集合，集合中的元素是唯一的。通过哈希表实现，添加，删除，查找的复杂度都是O(1)。
+ ```SADD key ele1 ele2...``` 向集合添加元素，返回1为添加成功，0为集合的重复元素，
+ ```SMEMBERS key``` 返回集合所有元素，
+ ```SISMEMBER key element``` 查找某个元素是否在集合中，
+ ```SCARD key``` 获取集合的元素个数，
+ ```SPOP key``` 随机获取并移除一个元素,

+ ```SRANDMEMBER key n``` 随机返回集合中的n个元素。

应用例子：给新闻文章添加一组类型标签，扑克牌游戏。

###### 6. 对于有序集合（sorted set）：
和集合类似，不同的是每个元素都会关联一个double类型的分数，来为元素进行从小到大的排序。
+ ```ZADD key score ele``` 向有序集合添加元素及分数，
+ ```ZRANGE key index1 index2``` 返回元素，按索引（分数）排序，
+ ```ZRANGEBYSORCE key min max``` 返回分数在某个区间的元素，
+ ```ZRANGK key element``` 返回元素索引，

+ ```ZCARD key``` 返回集合的元素个数。

新特性：分数相同时按字母顺序排序。

### 遇到的问题：

### TODO：

 	-深入理解Redis数据库的架构和实现机制
 	-其他key-value型数据库学习
