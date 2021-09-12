# 实验三 Redis的安装和使用

 陈思芹
 2021.9.7


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
####
有序集合（sorted set）：
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
### 直接尝试使用redis

try redis，一个可直接试用redis的免安装在线平台: https://try.redis.io/

其他链接：

+ 官方文档（数据类型介绍）:https://redis.io/topics/data-types-intro
+ 菜鸟教程：https://www.runoob.com/redis/redis-data-types.html

### 遇到的问题：

### TODO：

- 深入理解Redis数据库的架构和实现机制
- 其他key-value型数据库学习
