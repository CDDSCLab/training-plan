 # 实验零 环境配置
​    环境：archlinux

 * 宿主机安装Redis以及本地启动服务与连接

   ```bash
   $ sudo pacman -S  redis
   # 启动redis服务
   $ systemctl start redis
   # 直接就可以连上了
   $ redis-cli
   ```

 * 宿主机安装Mariadb以及本地启动服务与连接（Arch Linux 下使用的社区维护的MySQL,装了这个之后不能装MySql）

   ```bash
   $ sudo pacman -S mariadb
   # initialize 不初始化的话会报错哦哦
   $ mysql_install_db --user=mysql --basedir=/usr --datadir=/var/lib/
   $ mysql_secure_installation
   # 开启服务这个每个数据库都需要
   $ systemctl start mariadb
   # 默认root密码为空
   $ mariadb/mysql -u root -p
   ```

 * 宿主机安装MongoDB以及本地启动服务与连接

   ```bash
   $ sudo pacman S mongo
   # 在/ root目录下面新建了 /data/db 这个目录是默认路径,如果要改可以用--dbpath,把这个路径给mongo才能启动之 
   # 在root目录下建文件夹会是root root 用户和用户组的
   # 直接 mongod的时候出现了权限错误说/data/db是只读的,通过 ls -l 发现所属者和用户组都是root,可以通过sudo来执行,一劳永逸的办法是给文件夹改权限
   $ sudo chown -R 用户名:用户名所在组  /data
   # 在启动mongo服务就不会报错了  也可以使用systemctl start mongodb在后台开启mongod服务器
   $ mongod 
   # 打开shell连接服务器
   $ mongo
   ```

 * 宿主机安装janusGraph以及本地启动服务与连接

   ```bash
   # 从github Release下载 https://github.com/JanusGraph/janusgraph/releases下载full版本
   $ unzip janusgraph-0.5.2.zip
   # 启动
   $ cd janusgraph-0.5.2
   # 启动服务
   $ ./bin/gremlin-server.sh start 或者 ./bin/gremlin-server.sh  
   # 连接服务器
   $ ./bin/gremlin.sh
   gremlin> :remote connect tinkerpop.server conf/remote.yaml
   ==>Configured 172.17.0.2/172.17.0.2:8182
   # 注意 除了gremlin-server.sh  还可以用./janusgraph.sh start 再用gremlin连
   # 二者之间的区别 Info: https://docs.janusgraph.org/getting-started/installation/
   ```
   
 * 宿主机安装Docker `$ sudo pacman -S docker`

 * [Docker 基本使用命令](https://www.runoob.com/docker/docker-command-manual.html)

   ```bash
   # -i 为容器重新分配一个伪输入终端，通常与 -i 同时使用；
   # -t 以交互模式运行容器，通常与 -t 同时使用； 
   # -d 后台运行容器，并返回容器ID；
   # -p 指定端口映射，格式为：主机(宿主)端口:容器端口
   # --name  为容器指定一个名称；
   # --rm 在Docker容器退出时，默认容器内部的文件系统仍然被保留，以方便调试并保留用户数据。rm选项，这样在容器退出时就能够自动清理容器内部的文件系统。
   # --link 添加链接到另一个容器；
   # -volume 绑定一个卷 挂载之后docker用的 ref：https://www.cnblogs.com/51kata/p/5266626.html
   # -volumes-from
   # 使用镜像imageName以交互模式启动一个容器,在容器内执行/bin/bash命令,如果本地没有这个image会从Docker Hub拉取。
   $ docker run --name containerName -itd -p computerPort:containerPort imageName /bin/bash
   # 带-a显示全部container，不带-a显示正在运行的container
   $ docker container ls -a = docker ps -a
   # 显示image
   $ docker image ls 
   # 删除container 只能删除已经停止的
   $ docker container rm [CONTAINER...]
   # 删除image 只能删除已经没有被使用的image
   $ docker image rm [CONTAINER...]
   ```
   ```bash
   # 都是停止运行一个container前者更为温和用的是SIGTERM 后者用的是SIGKILL
   $ docker container stop [CONTAINER...]
   $ docker container kill [CONTAINER...]
   ```

   ```bash
   # 在运行的容器中执行命令
   docker exec -itd CONTAINER [ARG... /bin/bash]
   # 刪除<none>:<none>的所有 dangling images，指的是没有标签并且没有被容器使用的镜像。 https://blog.csdn.net/boling_cavalry/article/details/90727359
   docker image prune
   ```
   
* 这几种数据库的简单比较

  | 数据库名称 | 类型     | 查询语言                                                   | 实现语言                                         | 事务ACID的支持 | 特点                            | 默认端口 | 应用场景  |
  | ---------- | -------- | ---------------------------------------------------------- | ------------------------------------------------ | -------------- | ------------------------------- | -------- | --------- |
  | Redis      | KV       | [内置](https://redis.io/documentation)                     | [C](https://github.com/redis/redis)              | 弱             | 内存数据库快,EXPIRE数据过期功能 | 6379     | OLAP,OLTP |
  | MySQL      | SQL      | SQL                                                        | [C++](https://www.mysql.com/cn/)                 | 强             | SQL,用的广,数据来大会比较慢     | 3306     | OLTP      |
  | MongoDb    | Document | [内置](https://docs.mongodb.com/manual/reference/command/) | [C++](https://github.com/mongodb/mongo)          | 弱             | bson支持的数据格式比较丰富,mmap | 27017    | OLAP,OLTP |
  | JanusGraph | Graph    | [Gremlin](https://tinkerpop.apache.org/gremlin.html)       | [java](https://github.com/JanusGraph/janusgraph) | 强             | 直观                            | 8182     | OLAP,OLTP |

## TODO：

* DockerFIle的使用

* [-v 实现容器间、docker与宿主机之间的数据共享](https://www.cnblogs.com/51kata/p/5266626.html)

* Docker attach 

* Docker 性能


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
* 最大的收获是熟悉了redis一部分命令文档，以后有不懂的可以直接查redis文档了。

 ## TODO：

 	- 为什么有RPOPLPUSH 却没有LPOPLPUSH，队列先进先出所以是RPOPLPUSH。如果是栈的话，后进先出如果按这个逻辑来说应该要有一个RPOPLPUSH吧---->解决方法.在使用堆栈的时候把List反着用,RPUSH.
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
* 字符串可以用‘ ’ 也可以用用“ ”。 

 ## TODO：

 	- 关系表设计范式。
 	- 事物ACID。
 	- 一个SQL的执行过程。

 # 实验三 MongoDB的安装和使用
 ## Step1：Docker环境下安装MongoDB并连接远程Atlas
 * 情况1 2个docker容器一个当服务器，一个当客户端。
1. `$ docker run -id --name mongodb-test -p 27016:27017 mongo ` 开启服务

2. 容器机`$docker exec -it mongodb-test /bin/bash `打开bash再通过`$ mongo -h localhost -u root -p 输入` your_pass_word 连接好服务。

* 情况2 docker内环境连接，宿主机连的话一样的。
1. 在mongo官网新建一个免费的Atlas,mongo公司有能力云服务

2. `$ docker run -it --name mongo-test -v /home/icepig/M001:/data mongo    /bin/bash`进入docker mongo容器根据教材下载loadMovieDetailsDataset.zip 解压到/home/icepig/M001 因为docker要用到用到外面的空间,所以-v 挂载一下,挂载的话空间就共享了,可以设置多个挂载点.

   ```
   root帐号进入 /var/lib/docker/containers/Contianer-Id/config.v2.json 可以手动修改挂载点.
   "MountPoints":{"/data":{"Source":"/home/icepig/M001","Destination":"/data","RW":true,"Name":"","Driver":"","Type":"bind","Propagation":"rprivate","Spec":{"Type":"bind","Source":"/home/icepig/M001","Target":"/data"},"SkipMountpointCreation":false},
   ```

3. `cd /data` ` load(loadMovieDetailsDataset.js)` 加载数据到mongoDB

4. `$ mongo "mongodb+srv://cluster0.uj1f5.mongodb.net" --username m001  `连接远程Atlas，需要先通过`cat /etc/hosts`查看当前docker容器中的IP地址通过mongo网页加入到ip whitelist里面，才能正常连接成功。

 ## Step2：学习mongo的使用

* [实验记录](https://university.mongodb.com/mercury/M001/2020_July_7/chapter/Chapter_2_The_MongoDB_Query_Language_Atlas/lesson/595aae2236942e83e9a361a9/lecture)

```javascript
# question 1 How many documents in video.movieDetails match the filter {genres: "Comedy"}?
$ db.movieDetails.find({genres: "Comedy"}).length()
# question 2 插入一个字段并查找
$ db.movieDetails.insert({"title":""})
$ db.movieDetails.find({title: ""})
# question 3 How many movies in the movieDetails collection have exactly 2 award wins and 2 award nominations?
$ db.movieDetails.find({"awards.wins": 2, "awards.nominations": 2}).count()
# question 4 How many movies in the movieDetails collection are rated PG and have exactly 10 award nominations?
$ db.movieDetails.find({"rated": "PG", "awards.nominations": 10}).count()
# question 5 How many documents list just two writers: "Ethan Coen" and "Joel Coen", in that order?
$ db.movieDetails.find({"writers": ["Ethan Coen","Joel Coen"]})
# question 6 How many movies in the movieDetails collection list "Family" among its genres?
$ db.movieDetails.find({"genres":"Family"}).count()
$ db.movieDetails.find({genres:{$in:["Family"]}}).length()
# question 7 How many movies in the movieDetails collection list "Western" second among its genres?
$ db.movieDetails.find({"genres.1":"Western"}).count()
# question 8 How many documents in the 100YWeatherSmall.data collection do NOT contain the key atmosphericPressureChange?
$ db.data.find({atmosphericPressureChange: {$exists: false}}).count()
# question 9 shipwrecks collection match either of the following criteria: watlev equal to "always dry" or depth equal to 0.
$ db.shipwrecks.find({$or: [{depth: 0}, {watlev: "always dry"}]}).count()
# question 10 How many documents list: "AG1", "MD1", and "OA1" among the codes in their sections array.
$ db.data.find({sections: {$all: ["AG1", "MD1", "OA1"]}}).count()
# question 11 How many documents in this collection contain exactly two elements in the sections array field?
$ db.data.find({sections: {$size: 2}}).count()
# question 12 How many documents in the results.surveys collection contain a score of 7 for the product, "abc"?
$ db.surveys.find({results: {$elemMatch: {score: 7, product: "abc"}}}).count()
```

* 命令大全

```
1、Help查看命令提示
help
db.help();
db.yourColl.help();

2、切换/创建数据库
use raykaeso; 当创建一个集合(collection/table)的时候会自动创建当前数据库

3、查询所有数据库
show dbs;

4、删除当前使用数据库
db.dropDatabase();

5、从指定主机上克隆数据库
db.cloneDatabase(“127.0.0.1”); 将指定机器上的数据库的数据克隆到当前数据库

6、从指定的机器上复制指定数据库数据到某个数据库
db.copyDatabase(“mydb”, “temp”, “127.0.0.1”);将本机的mydb的数据复制到temp数据库中

7、修复当前数据库
db.repairDatabase();

8、查看当前使用的数据库
db.getName()/db;

9、显示当前db状态
db.stats();

10、当前db版本
db.version();

11、查看当前db的连接服务器机器地址
db.getMongo();

12、查询之前的错误信息和清除
db.getPrevError();
db.resetError();

二、MongoDB Collection聚集集合
1、创建一个聚集集合（table）
db.createCollection(“collName”, {size: 20, capped: 5, max: 100});//创建成功会显示{“ok”:1}
//判断集合是否为定容量db.collName.isCapped();

2、得到指定名称的聚集集合（table）
db.getCollection(“account”);

3、得到当前db的所有聚集集合
db.getCollectionNames();

4、显示当前db所有聚集索引的状态
db.printCollectionStats();

5、查询当前集合的数据条数
db.yourColl.count();

6、查看当前集合数据空间大小
db.yourColl.dataSize();

7、得到当前聚集集合所在的db
db.yourColl.getDB();

8、得到当前聚集的状态
db.coll.stats();

9、得到聚集集合总大小
db.coll.totalSize();

10、聚集集合储存空间大小
db.coll.storageSize();

11、聚集集合重命名
db.coll.renameCollection(“ray”); 将coll重命名为ray

12、删除当前聚集集合
db.coll.drop();

三、MongoDB用户相关
1、添加一个用户（创建）
db.createUser({user: 'username', pwd: 'xxxx', roles: [{role: 'readWrite', db: 'dbname'}]}); 添加用户、设置密码、是否只读

2、数据库认证、安全模式(登录)
db.auth(“ray”, “123456”);

3、显示当前所有用户
show users;

4、删除用户
db.removeUser(“userName”);

四、MongoDB聚集集合查询
1、查询所有记录
db.userInfo.find();
相当于：select* from userInfo;
默认每页显示20条记录，当显示不下的情况下，可以用it迭代命令查询下一页数据。注意：键入it命令不能带“;”
但是你可以设置每页显示数据的大小，用DBQuery.shellBatchSize= 50;这样每页就显示50条记录了。

2、查询去掉后的当前聚集集合中的某列的重复数据
db.userInfo.distinct(“name”);
会过滤掉name中的相同数据
相当于：select distict name from userInfo;

3、查询age = 22的记录
db.userInfo.find({“age”: 22});
相当于： select * from userInfo where age = 22;

4、条件查询的记录
MongoDB中条件操作符有：
(>) 大于 – $gt
(<) 小于 – $lt (>=) 大于等于 – $gte
(<= ) 小于等于 – $lte db.userInfo.find({age: {$gt: 22}}); 相当于：select * from userInfo where age>22;

db.userInfo.find({age: {$lt: 22}});
相当于：select * from userInfo where age<22; db.userInfo.find({age: {$gte: 25}}); 相当于：select * from userInfo where age >= 25;

db.userInfo.find({age: {$lte: 25}});
相当于：select * from userInfo where age <= 25; 5、and查询 db.userInfo.find({age: {$gte: 23, $lte: 26}}); 相当于：select * from userInfo wher age >=23 and age <=26
db.userInfo.find({name: ‘raykaeso’, age: 22});
相当于：select * from userInfo where name = ‘raykaeso’ and age = ‘22′;

6、字符模糊查询
db.userInfo.find({name: /mongo/});
//相当于%%
[code]select * from userInfo where name like ‘%mongo%';

7、查询指定列数据
db.userInfo.find({}, {name: 1, age: 1});
相当于：select name, age from userInfo;
当然name也可以用true或false

8、按条件查询指定列数据
db.userInfo.find({age: {$gt: 25}}, {name: 1, age: 1});
相当于：select name, age from userInfo where age <25;

9、排序
升序：db.userInfo.find().sort({age: 1});
降序：db.userInfo.find().sort({age: -1});

10、查询前5条数据
db.userInfo.find().limit(5);
相当于：select * from userInfo limit 5;

11、查询10条以后的数据
db.userInfo.find().skip(10);
相当于：select count(*) from userInfo as total;
select * from userInfo limit 10,total;

12、查询在5-10之间的数据
db.userInfo.find().limit(10).skip(5);
可用于分页，limit是pageSize，skip是第几页*pageSize
相当于：select * from userInfo limit 5,10;

13、or与 查询
db.userInfo.find({$or: [{age: 22}, {age: 25}]});
相当于：select * from userInfo where age = 22 or age = 25;

14、查询第一条数据
db.userInfo.findOne();
db.userInfo.find().limit(1);
相当于：select * from userInfo limit 1;

15、查询某个结果集的记录条数
db.userInfo.find({age: {$gte: 25}}).count();
相当于：select count(*) from userInfo where age >= 20;

五、MongoDB索引
1、创建索引
db.userInfo.ensureIndex({name: 1});
db.userInfo.ensureIndex({name: 1, ts: -1});

2、查询当前聚集集合所有索引
db.userInfo.getIndexes();

3、查看总索引记录大小
db.userInfo.totalIndexSize();

4、读取当前集合的所有index信息
db.users.reIndex();

5、删除指定索引
db.users.dropIndex(“name_1″);

6、删除所有索引索引
db.users.dropIndexes();

六、MongoDB修改、添加、删除集合数据
1、添加
db.users.save({name: ‘zhangsan’, age: 25, sex: true});
添加的数据的数据列，没有固定，根据添加的数据为准

2、修改
db.users.update({age: 25}, {$set: {name: ‘changeName’}}, false, true);
相当于：update users set name = ‘changeName’ where age = 25;
db.users.update({name: ‘Lisi’}, {$inc: {age: 50}}, false, true);
相当于：update users set age = age + 50 where name = ‘Lisi';
db.users.update({name: ‘Lisi’}, {$inc: {age: 50}, $set: {name: ‘hoho’}}, false, true);
相当于：update users set age = age + 50, name = ‘hoho’ where name = ‘Lisi';

3、删除
db.users.remove({age: 132});

4、查询修改删除
db.users.findAndModify({
query: {age: {$gte: 25}},
sort: {age: -1},
update: {$set: {name: ‘a2′}, $inc: {age: 2}},
remove: true
});  
```

 ## 特点：

* MongoDB的基本概念

| SQL术语/概念 | MongoDB术语/概念 | 解释/说明                           |
| :----------- | :--------------- | :---------------------------------- |
| database     | database         | 数据库                              |
| table        | collection       | 数据库表/集合                       |
| row          | document         | 数据记录行/文档                     |
| column       | field            | 数据字段/域                         |
| index        | index            | 索引                                |
| table joins  |                  | 表连接,MongoDB不支持                |
| primary key  | primary key      | 主键,MongoDB自动将_id字段设置为主键 |

 ## TODO：

 	- 学习和记录正则.

 # 实验四 JanusGraph的安装和使用
 ## Step1：根据[Janus Graph docker github开源项目](https://github.com/JanusGraph/janusgraph-docker)开启服务和连接服务
1. `docker run --rm --name janusgraph-default janusgraph/janusgraph:latest` 打开服务
2. `docker run --rm --link janusgraph-default:janusgraph -e GREMLIN_REMOTE_HOSTS=janusgraph 
       -it janusgraph/janusgraph:latest ./bin/gremlin.sh`一步到位直接执行了./bin/gremlin.sh 注意一定要有.应该是image 文件上做了挂载吧

 ## Step2：[体验Janus Graph](https://docs.janusgraph.org/getting-started/basic-usage/) [中文版本](https://juejin.im/entry/5cc02153f265da03502b3c9a)

![希腊众神图](/home/icepig/project/training-plan/Week1-Database-Introduction/DaoBingZhu-Week1/ExperimentReport.assets/godsGraph.jpg)

| 标示          | 含义                           |
| ------------- | ------------------------------ |
| 加粗的key     | 图中的索引键                   |
| 加粗带星的key | 图中的索引键值必须是唯一的     |
| 带下划线的key | 以顶点为中心的索引键           |
| 空心箭头的边  | 特定的边(不能重复)             |
| 尾部十字的边  | 单项边(只能在一个方向建立关系) |

```gremlin
# 远程连接
gremlin> :remote connect tinkerpop.server conf/remote.yaml
==>Configured localhost/127.0.0.1:8182
# open和load在图形上创建全局和顶点中心索引的集合。将所有顶点及其属性添加到图形中。将所有边线及其属性添加到图形中。
# open  -inmemory
gremlin> graph = JanusGraphFactory.open('conf/janusgraph-inmemory.properties')
==>standardjanusgraph[inmemory:[127.0.0.1]]
# 加载图对象 用GraphOfTheGodsFactory.loadWithoutMixedIndex而不是GraphOfTheGodsFactory.load,后者报错
gremlin> GraphOfTheGodsFactory.loadWithoutMixedIndex(graph, true)
# 遍历图存到变量g中
gremlin> g = graph.traversal()
==>graphtraversalsource[standardjanusgraph[cql:[127.0.0.1]], standard]
# 通过name属性上的唯一索引，可以检索到Saturn顶点 .next()
gremlin> saturn = g.V().has('name', 'saturn').next()
# valueMap() 点或边的所有属性
gremlin> g.V(saturn).valueMap()
==>[name:[saturn], age:[10000]]
# 通过边的名称查连接点
gremlin> g.V(saturn).in('father').in('father').values('name')
==>hercules
# .as('source')存临时变量,方便后面用;inV() 访问边的入顶点，入顶点是指边的目标顶点，也就是箭头指向的顶点；outV() 问边的出顶点，出顶点是指边的起始顶点；显示name这个属性
gremlin> g.E().has('place', geoWithin(Geoshape.circle(37.97, 23.72, 50))).as('source').inV().as('god2').select('source').outV().as('god1').select('god1', 'god2').by('name')
==>[god1:hercules, god2:hydra]
==>[god1:hercules, god2:nemean]
# repeat(   ).times( ) 
gremlin> hercules = g.V(saturn).repeat(__.in('father')).times(2).next()
# 根据边找其他点的属性
gremlin> g.V(hercules).out('father', 'mother').values('name')
==>jupiter
==>alcmene
# label() 
gremlin> g.V(hercules).out('father', 'mother').label()
==>god
==>human
# 根据某个属性进行条件选择 gt
gremlin> g.V(hercules).outE('battled').has('time', gt(1)).inV().values('name')
==>cerberus
==>hydra
# 先out('lives')就到了pluto住的地方Tartarus,再in('lives')就能够显示出谁住在这,可以在按条件选择
gremlin> g.V(pluto).out('lives').in('lives').values('name')
==>pluto
gremlin> g.V(pluto).out('lives').in('lives').where(is(neq(pluto))).values('name')
==>cerberus
gremlin> g.V(pluto).as('x').out('lives').in('lives').where(neq('x')).values('name')
==>cerberus
# Pluto的兄弟住在哪
gremlin> g.V(pluto).out('brother').as('god').out('lives').as('place').select('god', 'place').by('name')
==>[god:jupiter, place:sky]
==>[god:neptune, place:sea]
# Pluto的兄弟们根据这些地方的品质来选择他们居住的地方。
gremlin> g.E().has('reason', textContains('loves')).as('source').values('reason').as('reason').select('source').outV().values('name').as('god').select('source').inV().values('name').as('thing').select('god', 'reason', 'thing')
==>[god:neptune, reason:loves waves, thing:sea]
==>[god:jupiter, reason:loves fresh breezes, thing:sky]
```



 ## 遇到的问题：

1. 根据[Basic Usage](https://docs.janusgraph.org/getting-started/basic-usage/)命令顺序执行会遇到问题,文档其实有提示,注意提升阅读英文文档的能力.

   ```gremlin
   # 方法一 Basic Usage Info 提示 in-memory no index
   Using any configuration file other than conf/janusgraph-inmemory.properties requires that you have a dedicated backend configured and running. If you just want to quickly open a graph instance and explore some of the JanusGraph features, you could simply choose conf/janusgraph-inmemory.properties to open an in-memory backend.
   
   # open的时候换一个配置文件
   gremlin> graph = JanusGraphFactory.open('conf/janusgraph-cql.properties')
   
   # Insatllation Info提示
   The default configuration (gremlin-server.yaml) uses it's own inmemory backend instead of a dedicated database server. No search backend is used by default, so mixed indices aren't supported as search backend isn't specified (Make sure you are using GraphOfTheGodsFactory.loadWithoutMixedIndex(graph, true) instead of GraphOfTheGodsFactory.load(graph) if you follow Basic Usage example). For further information about storage backends, visit the corresponding section of the documentation.
   You are also encouraged to look into janusgraph.sh, which by defaults starts a more sophisticated server than gremlin-server.sh. Further documentation on server configuration can be found in the JanusGraph Server section. (This requires to download janusgraph-full-0.5.2.zip instead of the default janusgraph-0.5.2.zip.)
   
   # 加载的时候使用
   gremlin> GraphOfTheGodsFactory.loadWithoutMixedIndex(graph, true)
   # 方法二 使用完整版的janusgraph-full bin下面的 janusgraph.sh start启动服务再连接 然后根据步骤走就OK
   ```
   

 ## TODO：

![gremlin running](/home/icepig/project/training-plan/Week1-Database-Introduction/DaoBingZhu-Week1/ExperimentReport.assets/gremlin-running.png)

* Gremlin语法的学习与了解 [教程](http://tang.love/2018/11/15/gremlin_traversal_language/) [文档](http://tinkerpop-gremlin.cn/)
