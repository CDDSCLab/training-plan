 # 实验零 环境配置
​    环境：archlinux

 * 安装Docker `$ sudo pacman -S docker`

 * 安装Redis `$ sudo pacman -S  redis`

 * 安装Mariadb（Arch Linux 下使用的社区维护的MySQL）

   ```sql
     $ sudo pacman -S mariadb
     # initialize
     $ mysql_install_db --user=mysql --basedir=/usr --datadir=/var/lib/
     $ mysql_secure_installation
     # 开启服务这个每个数据库都需要
     $ systemctl start mariadb
   ```

 * 安装MongoDB

 * 安装j

 * [Docker 基本使用命令](https://www.runoob.com/docker/docker-command-manual.html)

   ```bash
   # -i 为容器重新分配一个伪输入终端，通常与 -i 同时使用；
   # -t 以交互模式运行容器，通常与 -t 同时使用； 
   # -d 后台运行容器，并返回容器ID；
   # -p 指定端口映射，格式为：主机(宿主)端口:容器端口
   # --name  为容器指定一个名称；
   # 使用镜像imageName以交互模式启动一个容器,在容器内执行/bin/bash命令,如果本地没有这个image会从Docker Hub拉取。
   $ docker run --name containerName -itd -p computerPort:containerPort imageName /bin/bash
   ```

   ```bash
   # 带-a显示全部container，不带-a显示正在运行的container
   $ docker container ls -a = docker ps -a
   # 显示image
   $ docker image ls 
   # 删除container 只能删除已经停止的
   $ docker container rm [CONTAINER...]
   # 删除image 只能删除已经没有被使用的image
   $ docker image rm [CONTAINER...]
   # 都是停止运行一个container前者更为温和用的是SIGTERM 后者用的是SIGKILL
   $ docker container stop [CONTAINER...]
   $ docker container kill [CONTAINER...]
   ```

   ```bash
   # 在运行的容器中执行命令
   docker exec -itd CONTAINER [ARG... /bin/bash]
   ```
## TODO：

* 使用DockerFIle一口气配置好Docker环境上的四种数据库。

  ```dockerfile
  
  ```

# 实验一 [Redis的安装和使用](https://redis.io/)

 ## Step1：Docker环境下安装Redis并启动Redis服务

 1. 开启Redis服务

    `$ docker run -itd --name  redis-test -p 6378:6379 redis` docker开启redis服务器，将container的端口映射到本地[0.0.0.0](https://blog.csdn.net/ythunder/article/details/61931080):6378

 2. 客户端连接Redis服务：本地机，容器中

    * 本地机通过`$ redis-cli -h 127.0.0.1 -p 6378` 从127.0.0.1:6378连接到位于容器中127.0.0.1:6379的Redis服务。

    * `$ docker exec -it redis-test /bin/bash `容器中先开启bash再通过`$ redis-cli`直接连接到127.0.0.1：6379。
 ## Step2：通过redis-cli连接并进行实验

0.  本地机执行`> set mykey somevalue`,容器机可以通过`> get mykey `获取值`somevalue`说明redis支持多个客户端访问（后续只用一个客户端访问）。

1. Binary-safe strings.

   ```bash
   # nx key不存在的时候操作成功  xx key存在的时候操作成功
   > set mykey newval nx 
   (nil)
   > set mykey newval xx
   OK
   # 数字存储的时候也是按照Binary-safe strings,incr/decr/incrby/decrby等数字操作会把string解析成数字来进行操作;如果不是数字会返回错误
   > set counter 100
   OK
   > incr counter
   (integer) 101
   >incr mykey
   (error) ERR value is not an integer or out of range
   # mset/mget多个key同时操作
   > mset a 10 b 20 c 30
   OK
   > mget a b c
   1) "10"
   2) "20"
   3) "30"
   # exist判断是否存在 存在返回(integer) 1,失败返回(integer) 0;
   # del删除key       成功返回(integer) 1,失败返回(integer) 0;
   > exists mykey
   (integer) 1
   > del mykey
   (integer) 1
   > exists mykey
   (integer) 0
   # type返回key的类型，如果不存在返回none
   > set mykey x
   OK
   > type mykey
   string
   > del mykey
   (integer) 1
   > type mykey
   none
   # expires key 过期时间，单位可以是秒或者毫秒（pexpire=px） 存的是过期时间，即使Redis服务器停止时间也会流动;
   # 可以一步到位的设置，也可以分开设置，ttl（pttl）命令查看剩余时间。
   > set key some-value
   OK
   > expire key 5
   (integer) 1
   > get key (immediately)
   "some-value"
   > get key (after some time)
   (nil)
   > set key 100 ex 10
   OK
   > ttl key
   (integer) 9
   ```
   
2. Lists（双向链表实现） 插入比顺序表更快，查找更慢（用mset）。
   
   ```bash
   # rpush/lpush 向List后面和前面插入（一个或者多个-操作顺序是命令上的顺序）元素 返回当前List元素的数量
   > rpush mylist A
   (integer) 1
   > rpush mylist B
   (integer) 2
   > lpush mylist third second first 
   # lrange 范围获取 0第一个 -1最后一个 左闭右闭[];没有rrange
   > lrange mylist 0 -1
   1) "first"
   2) "A"
   3) "B"
   # lpop/rpop 当没东西的时候返回（nil）
   > lpop mylist
   "third"
   > lpop mylist # 执行5次第5次返回（nil）
   (nil)
   >exists mylist
   (integer)0
   # ltrim 范围选定和lrange一样，功能是在范围内的当作list新值，不在范围内的舍弃。O（n）时间复杂度
   >lpush mylist 5 4 3 2 1 
   (integer)5
   >ltrim mylist 0 2
   ok
   >lrange mylist 0 -1
   1) "1"
   2) "2"
   3) "3"
   # blpop/brpop key [key...] timeout (为0无限等待)阻塞版本的lpop/rpop
   >blpop myblocklist 0  # 打开另一个客户端写入数据2会通知当前客户端
   1) "myblocklist"
   2) "2"
   (55.96s)
   # RPOPLPUSH/BRPOPLPUSH for Reliable queue(当数据出队列之后放到一个process list当消费者真正的获取到数据之后再把process list的数据消除，比如如果网络出现问题，又没有保留process list数据就不能浮复现了)ref-https://redis.io/commands/rpoplpush
   >RPOPLPUSH mylist processList
   # LREM key count
   count > 0: Remove elements equal to element moving from head to tail.
   count < 0: Remove elements equal to element moving from tail to head.
   count = 0: Remove all elements equal to element.
   # LLEN key 返回长度
   >LLEN mylist
   (integer)3
   ```
   
3. Hashes

   ```bash
   # hset key field value; hget key field 
   > hset myhash user 200
   > hget myhash user 
   "200"
   # hmset key field value ...; hmget/hget key field...
   > hmset myhashs user 100 username icepig age 22
   > hget myhashs user
   "100"
   > hmget myhashs user username age
   1) "100"
   2) "icepig"
   3) "22"
   # hmset 可以不带key 
   > hmset user:300 username icepig300 age 22300
   OK
   > hmget user:300 username age
   1) "icepig300"
   2) "22300"
   # 上面这种hmget user:100 username age 不能获取到 myhashs的值
   > hmget user:100 username age
   1) (nil)
   2) (nil)
   # HINCRBY
   > hincrby myhashs user -1
   (integer)99
   > hincrby user:300 user -1 # user不能更改又新建了一个user初始值为0,-1=-1
   "-1"
   > hincrby user:300 user
   "-1"
   ```

4. Sets non-repeating/unique unordered

   ```bash
   # sadd key value... 添加到set里;smembers key 查看元素 
   > sadd myset 1 2 3
   (integer) 3
   > smembers myset
   1. 3
   2. 1
   3. 2
   # set 标签集合 key是标签 元素是这个标签下的值
   > sadd news:1000:tags 1 2 5 77
   (integer) 4
   # set 换个标签名字
   > sadd tag:1:news 1000
   (integer) 1
   > sadd tag:2:news 1000
   (integer) 1
   > sadd tag:5:news 1000
   (integer) 1
   > sadd tag:77:news 1000
   (integer) 1
   # sismember 判断是否存在
   > sismember myset 3
   (integer) 1
   > sismember myset 30
   (integer) 0
   # > smembers news:1000:tags
   1. 5
   2. 1
   3. 77
   4. 2
   # sinter 求不同集合之间的intersection交集 union:sunion并集 difference：sdiff差集 
   > sinter tag:1:news tag:2:news tag:10:news tag:27:news
   1) "1000"
   # sinterstore sunionstore sdiffstore两个set做inter/union/diff并赋值给第二个
   > sunionstore new tag:1:news
   (integer)1
   # 随机弹出一个spop 随机出一个但是不删除原来的srandmember
   > spop news:1000:tags
   "3"
   # scard set的长度
   > scard new
   (integer)3
   # smove sourceSet destSet value 从源set往目的set移动元素
   > SADD myset "one"
   (integer) 1
   is> SADD myset "two"
   (integer) 1
   > SADD myotherset "three"
   (integer) 1
   > SMOVE myset myotherset "two"
   (integer) 1
   > SMEMBERS myset
   1) "one"
   > SMEMBERS myotherset
   1) "three"
   2) "two"
   # sscan 
   > sscan myotherset 0
   1)"0"
   2)	1) "three"
   	2) "two"
   ```

5. Sorted sets 

   ```bash
   # zadd key score value ...
   > zadd people 1 icepig
   (integer) 1
   > zadd people 2 icepig2
   (integer) 1
   > zadd people 3 icepig3
   (integer) 1
   # zrange key startIndex endIndex 从小到大 zrevrange
   > zrange people 0 -1
   1) "icepig"
   2) "icepig2"
   3) "icepig3"
   # zrangebyscore key -inf score  inf无穷大，-inf +inf  “（5 10”相当于（5,10」
   > zrangebyscore people (1 2
   1) "icepig2"
   # zrank key value 返回value的rank 从0开始
   > zrank people icepig2
   (integer) 1
   # lexicographically 字典序从a开始 ZRANGEBYLEX（根据字典返回值）, ZREVRANGEBYLEX（反序返回）, ZREMRANGEBYLEX（移除remove） and ZLEXCOUNT（计数）. （ 开「闭
   > ZADD myzset 1 "one"
   (integer) 1
   > ZADD myzset 2 "two"
   (integer) 1
   > ZADD myzset 3 "three"
   (integer) 1
   > ZREMRANGEBYSCORE myzset -inf (2
   (integer) 1
   > ZRANGE myzset 0 -1 WITHSCORES
   1) "two"
   2) "2"
   3) "three"
   4) "3"
   ```

6. Bitmaps

   ```bash
   # setbit key index 1/0 get key index 长度自动增长，没有置1过的返回为0
   > setbit key 10 1
   (integer) 1
   > getbit key 10
   (integer) 1
   > getbit key 11
   (integer) 0
   # bitop and/or/xor/not destkey key1 key2
   >set key1 "foobar"
   >set key2 "abcdef"
   >bitop and dest key1 key2
   >get dest
   "`bc`ab"
   # bitcount key 1的个数
   >bitcount dest
   (integer) 17
   # bitpos 找到第一个为1/0的index
   > bitpos dest 0
   (integer) 0
   > bitpos dest 1
   (integer) 1
   ```

7. HyperLogLogs 基数计数

   ```bash
   > pfadd hll a b c d
   (integer) 1
   > pfcount hll
   (integer) 4
   ```

 ## 特点：

* Redis keys are binary safe, this means that you can use any binary sequence as a key, from a string like "foo" to the content of a JPEG file. The empty string is also a valid key.The maximum allowed key size is 512 MB.
* Automatic creation and removal of keys applies to all the Redis data types composed of multiple elements -- Streams, Sets, Sorted Sets and Hashes.
* 最大的收获是熟悉了redis一部分命令文档，以后有不懂的可以直接差redis文档了。

 ## TODO：

 	- 为什么有RPOPLPUSH 却没有LPOPLPUSH，队列先进先出所以是RPOPLPUSH。如果是栈的话，后进先出如果按这个逻辑来说应该要有一个RPOPLPUSH吧
 	- 如果有时间重新看看[《Redis设计与实现》](http://redisbook.com/)了解一下具体的每个data type是哪种数据结构实现的。
 	- 基数估计HLL[基数估计原理](http://www.rainybowe.com/blog/2017/07/13/%E7%A5%9E%E5%A5%87%E7%9A%84HyperLogLog%E7%AE%97%E6%B3%95/index.html?utm_source=tuicool&utm_medium=referral)

 # 实验二 MySQL的安装和使用
 ## Step1：Docker环境下安装MySQL并启动MySQL服务
1. `$ docker run --name mysql-test -p 3305:3306 -e MYSQL_ROOT_PASSWORD=your_pass_word mysql` 开启服务
2.  容器机`$docker exec -it mysql-test /bin/bash `打开bash再通过`$ mysql -h localhost -u root -p 输入` your_pass_word 连接好服务。

 ## Step2：建立一个student表和student_info表，并查询一个学生对应的student_info信息
```SQL
# 创建数据库并切换到目标数据库
create database h_test;
use h_test;
# 创建学生表 （学号，姓名，性别，团队号，电话）
CREATE TABLE STUDENT(
 SID     BIGINT  UNSIGNED PRIMARY KEY  AUTO_INCREMENT,
 NAME    VARCHAR(40),
 SEX     VARCHAR(4)  DEFAULT 'male' CHECK(SEX IN ('male','female')),
 TID     INT UNSIGNED,
 TEL     INT UNSIGNED
)engine=InnoDB default charset=utf8 auto_increment=1;
# 创建学生信息表（SID 高考英语、数学、语文、综合成绩）
CREATE TABLE STUDENT_INFO(
SID BIGINT UNSIGNED PRIMARY KEY  AUTO_INCREMENT,
ENGLISH_SCORE INT UNSIGNED NOT NULL,
MATH_SCORE INT UNSIGNED NOT NULL,
CHINESE_SCORE INT UNSIGNED NOT NULL,
COMPREHENSIVE_SCORE INT UNSIGNED NOT NULL
)engine=InnoDB default charset=utf8 auto_increment=1;
# 添加学生信息表对学生表学号的外键
ALTER TABLE STUDENT_INFO
ADD CONSTRAINT INFO_STUDENT FOREIGN KEY(SID) 
REFERENCES STUDENT(SID) ON DELETE  CASCADE ON UPDATE CASCADE;
# 学生表插入三条数据
INSERT INTO STUDENT(SID,NAME,SEX,TID,TEL) VALUES (2020060101001,'icepig01','male',01,1234567891);
INSERT INTO STUDENT (SID,NAME,SEX,TID,TEL)
VALUES(2020060101002,'icepig02','male',01,1234567891);
INSERT INTO STUDENT （NAME,SEX,TID,TEL）
VALUES('icepig03','male',01,1234567891);
# 学生信息表插入三条数据
INSERT INTO STUDENT_INFO (SID,ENGLISH_SCORE,MATH_SCORE,CHINESE_SCORE,COMPREHENSIVE_SCORE)
VALUES(2020060101001,120,120,110,250);
INSERT INTO STUDENT_INFO (SID,ENGLISH_SCORE,MATH_SCORE,CHINESE_SCORE,COMPREHENSIVE_SCORE)
VALUES(2020060101002,130,130,110,270);
INSERT INTO STUDENT_INFO (SID,ENGLISH_SCORE,MATH_SCORE,CHINESE_SCORE,COMPREHENSIVE_SCORE)
VALUES(2020060101003,135,135,110,270);
# 显示建表效果
SELECT * FROM STUDENT;
+---------------+----------+------+------+------------+
| SID           | NAME     | SEX  | TID  | TEL        |
+---------------+----------+------+------+------------+
| 2020060101001 | icepig01 | male |    1 | 1234567891 |
| 2020060101002 | icepig02 | male |    1 | 1234567891 |
| 2020060101003 | icepig03 | male |    1 | 1234567891 |
+---------------+----------+------+------+------------+
SELECT * FROM STUDENT_INFO;
+---------------+---------------+------------+---------------+---------------------+
| SID           | ENGLISH_SCORE | MATH_SCORE | CHINESE_SCORE | COMPREHENSIVE_SCORE |
+---------------+---------------+------------+---------------+---------------------+
| 2020060101001 |           120 |        120 |           110 |                 250 |
| 2020060101002 |           130 |        130 |           110 |                 270 |
| 2020060101003 |           135 |        135 |           110 |                 270 |
+---------------+---------------+------------+---------------+---------------------+
# 学生信息表增加总分列，并设置为其他三列的值
ALTER TABLE STUDENT_INFO ADD COLUMN TOTAL_SCORE INT;
UPDATE STUDENT_INFO SET TOTAL_SCORE = ENGLISH_SCORE+MATH_SCORE+CHINESE_SCORE+COMPREHENSIVE_SCORE;
# 通过JOIN 查看两表数据 USING 去除重复字段。
SELECT * FROM STUDENT JOIN STUDENT_INFO USING (SID);
+---------------+----------+------+------+------------+---------------+------------+---------------+---------------------+-------------+
| SID           | NAME     | SEX  | TID  | TEL        | ENGLISH_SCORE | MATH_SCORE | CHINESE_SCORE | COMPREHENSIVE_SCORE | TOTAL_SCORE |
+---------------+----------+------+------+------------+---------------+------------+---------------+---------------------+-------------+
| 2020060101001 | icepig01 | male |    1 | 1234567891 |           120 |        120 |           110 |                 250 |         600 |
| 2020060101002 | icepig02 | male |    1 | 1234567891 |           130 |        130 |           110 |                 270 |         640 |
| 2020060101003 | icepig03 | male |    1 | 1234567891 |           135 |        135 |           110 |                 270 |         650 |
+---------------+----------+------+------+------------+---------------+------------+---------------+---------------------+-------------+
# 清库跑路
drop table STUDENT_INFO;
drop table STUDENT;
drop database h_test;
```

 ## 特点：

* 命令不区分大小写，database name table name field name 区分大小写。
* 字符串可以用‘ ’ 也应恶意用“ ”。 

 ## TODO：

 	- 关系表设计范式。
 	- 事物ACID。
 	- 一个SQL的执行过程。

 # 实验三 MongoDB的安装和使用
 ## Step1：Docker环境下安装MongoDB并启动MongoDB服务
1. `$ docker run -id --name mongodb-test -p 27016:27017 mongo ` 开启服务
2.  容器机`$docker exec -it mongodb-test /bin/bash `打开bash再通过`$ mongo -h localhost -u root -p 输入` your_pass_word 连接好服务。

 ## Step2：
xxx
 ## 特点：

xxx

 ## TODO：

 	- 
 	- 

 # 实验四 
 ## Step1：
xxx
 ## Step2：
xxx
 ## 遇到的问题：

xxx

 ## TODO：

 	- 
 	- 

   ```

   ```