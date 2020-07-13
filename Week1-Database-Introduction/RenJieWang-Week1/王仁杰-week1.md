

 # 实验一 MySQL的安装和使用

 ## Step1：Ubuntu上MySQL的安装

```bash
#更新软件列表
sudo apt-get update
#安装MySQL服务
sudo apt-get install mysql-sever
#使用service命令开启MySQL服务 打开位于/etc/init.d/目录下的服务
service mysql start
#启动MySQL安全配置向导，设置root用户的密码及其他
sudo mysql_secure_installaction
#MySQL登录 
sudo mysql -uroot -p
```

 ## Step2：创建数据库执行sql语句

```sql
//建立数据库test
create database test;
//查看MySQL中的所有数据库
show databases;
//选中要操作的数据库
use test;
//创建表student、teacher和class
//注意区分单引号和重音符的区别。
 create table student(
    -> Sid int(11) primary key,
    -> Sname varchar(20) not null,
    -> Sage int(11) not null
 	-> );
create table `teacher`(
    -> `Tid` int(11) primary key,
    -> `Tname` varchar(20) not null,
    -> `Tsex` varchar(10) default 'male'
    -> );
create table class(
    -> Sid int(11),
    -> constraint sc_fk foreign key(Sid) references student(Sid),
    -> Tid int(11),
    -> constraint tc_fk foreign key(Tid) references teacher(Tid)
	-> );
//向表中插入数据
 insert into student (Sid,Sname,Sage) values
    -> (0,'小明',15),(1,'小王',15);
insert into teacher(Tid,Tname,Tsex) values
    -> (0,'王老师','male');
insert into class(Cid,Sid,Tid) values (0,0),(1,0);
//查找王老师教的所有学生
select Sname from student,teacher,class
    -> where class.Sid=student.Sid and
    -> class.Tid=teacher.Tid and
    -> Tname='王老师';
```

 ## 遇到的问题：

1. MySQL不知支持中文,三种方法

   1. 建表时使用`character set=utf8`

   2. 建表后使用`alter table tableName convert to character set utf8`

   3. 在MySQL配置文件/etc/mysql/my.cnf中添加

      ```bash
      [mysqld]
      character-set-server=utf8
      [client]
      default-character-set=utf8
      ```

 ## TODO：

 	- 跟踪sql语句的执行过程
 	- 

 # 实验二 Redis的安装与使用

## Step1: Ubuntu上Redis的安装

```bash
#安装
sudo apt-get update
sudo apt-get install redis-server
#启动服务器
redis-server
#启动客户端
redis-cli
```

## Step2：使用

Redis有五种数据类型：字符串、哈希、列表、集合、有序集合。

### 字符串

* <font color='green'>SET key value  </font> 设定key的value
* <font color="green">GET key</font> 获取key的value
* <font color="green">DEL key</font> 删除key   
* <font color="green">EXISTS key</font>  key是否存在
* <font color="green">GETSET key</font>  设定旧值的同时返回新值
* <font color="green">STRLEN key</font> 返回对应字符串的长度  
* <font color="green">APPEND key append_value</font>  在字符串后追加字符串
* <font color="green">INCR/DECR  key</font>  当字符串为数字式+1/-1 

### 哈希

hash是一个string类型的field和value的映射表

* <font color="green">HMSET key  [field value]+ </font>  同时将多个field-value对设置到key中
* <font color="green">HSET key field value </font> 单独一个field-value对设置到key中 
* <font color="green">HGETALL key</font> 获取key的所有field和value对，以字符串组的形式
* <font color="green">HGET key field</font>  获取key中field对应的value
* <font color="green">HEDL key [field]+</font> 删除key中的 一个或多个field-value对
* <font color="green">HEXISTS key field</font> 查看key中是否存在field字段
*  <font color="green">HKEYS/HVALS key</font> 获取key中所有的field/value 

### 列表

list是一个字符串列表，最后push的字符串排在最前面，index为0

* <font color="green">LPUSH key [value]+</font> 向列表中添加字符串，越后面的排序越前
* <font color="green">LPOP key</font>  弹出第一个元素
*  <font color="green">LINDEX key index</font>  按照index获取元素，0为第一个元素
* <font color="green">LLEN key</font> 获取列表长度
* <font color="green">LSET key index value</font>  按照index修改元素的值 

### 集合

set是一个无序的字符串集合，集合成员唯一，不能有重复的数据

* <font color="green">SADD key [member]+</font> 在集合中添加成员 
* <font color="green">SMEMBERS key</font> 返回集合中的所有成员
* <font color="green">SISMEMBER  key member</font> 判断member是否是集合的成员
* <font color="green">SREM key [member]+</font> 移除集合中的一个或多个成员

### 有序集合

​	在集合的基础上为成员关联一个double型的数（这个数可以重复），对成员进行排序，有序集合使用哈希表实现，复杂度为O(1)

* <font color="green">ZADD key [score member]+ </font> 为有序集合添加数据
* <font color="green">ZRANGE  key start stop [withscores]?</font>  通过索引区间查找成员

# 实验三： mongoDB的安装与使用

## Step1：MongoDB安装

打开[mongoDB下载官网]( https://www.mongodb.com/try/download/enterprise )选择要下载mongoDB的版本和系统，然后可以选择直接下载或者Copy LInk，然后在Ubuntu使用wget命令下载。

下载完成后：

```bash
#解压
tar -zxvf fileName
#移动文件
MV fileName/  /usr/local/mongodb
#导入PATH，有三种方法
#1.export命令 只存在于当前shell
export PATH=<file-install-directory>/bin:$PATH
exprot PATH=$PATH:<file-install-directory>/bin
#2.修改 /etc/profile 添加上面的语句之一，重启生效
#3.修改 用户目录下的隐藏文件 .bashrc 添加语句，对当前用户，重启生效

#启动mongodb服务
mongod
#使用mongo shell进入本地mongodb
mongo
```

使用mongo shell连接atlas cluster

* 连接自己的cluster，首先在mongo官网申请免费的atlas cluster，配置好白名单，连接用户后，使用mongo shell连接atlas cluster。

  ```bash
  mongo "mongodb+srv://ailntps.xlogk.mongodb.net" --username <username>
  ```

* 连接MongoDB官网的用于学习的atlas cluster

  ```bash
  mongo "mongodb://cluster0-shard-00-00-jxeqq.mongodb.net:27017,cluster0-shard-00-01-jxeqq.mongodb.net:27017,cluster0-shard-00-02-jxeqq.mongodb.net:27017/test?replicaSet=Cluster0-shard-0" --authenticationDatabase admin --ssl --username m001-student --password m001-mongodb-basics
  
  #或者
  mongo "mongodb+srv://cluster0-jxeqq.mongodb.net/test" --username m001-student -password m001-mongodb-basics
  ```

使用Compass连接MongoDB

在[mongo官网]( https://www.mongodb.com/try/download/compass )下载自己系统的compass安装程序，安装好后使用connection string连接

```bash
mongodb+srv://m001-student:m001-mongodb-basics@cluster0-jxeqq.mongodb.net/test
```

## Step2 MongoDB的使用

MongoDB按照逻辑结构可以分为database、collection和document，这是一种层次结构，呈包含关系

* 创建/删除数据库

  ```shell
  #创建数据库
  use databaseName
  #在选中数据库的情况下
  db.dropDatabase();
  #查看数据库
  show dbs
  ```

* 创建/删除集合（collection）

  ```shell
  #创建 options类型为document/JSON
  db.createCollection(name,options)
  #删除
  db.collection_name.drop()
  ```

* 插入

  ```shell
  #两种插入方法
  db.collection_name.insertOne(
  	<document>
  )
  db.collection_name.insertMany(
  	[<document> <document> ......<document>]
  )
  ```

* 查询

  ```shell
  #查询 projection为投影，默认返回所有键值，设定时采用{key1:1,key2:1}表示选定
  db.collection_name.find(query,projection)
  
  #query中为document或Array时
  "Document.key:value"
  "Array.0:value" "Array.1:value"
  Array:[vaule1,vaule2]
  
  #pretty()表示格式化输出
  #查询使用的修饰符
  # or
  $or:[{key1:value1},{key2:value2}]
  # <
  $lt:value
  # <=
  $lte:value
  # >
  $gt:value
  # >=
  $gte:value
  # !=
  $ne:value
  # type
  $type:typename
  ```

* 更新

  ```shell
  #更新，query为条件，update为更新，multi false时只更新查到的第一条数据，true时全部更新
  db.collection_name.update(
  	<query>,
  	<update>,
  	{
  		upsert:<bool>
  		multi:<bool>
  	}
  )
  #更新修饰符
  #修改数据
  $set:{key:value}
  ```

* 删除

  ```shell
  db.collection_name.remove(
  	<querty>
  	{
  		justOne:<bool>
  	}
  )
  ```

# 实验四 Janusgraph实验

## Step1 Janusgraph安装

* 使用docker进行安装的两种方式

  ```shell
  #docker命令 -i交互运行 -d后台运行 -p指定端口 -t重新分配伪输入终端与-i一起使用 -name 指定容器名
  # -e设置环境变量 --link添加链接到另一个容器 --rm容器退出时不保留数据，默认保留
  #第一种，docker启动Janusgraph的服务器，使用8182端口
  $ docker run -it -p 8182:8182 janusgraph/janusgraph
  #使用下载好的JanusGraph文件的console连接服务器
  $ cd install_directory
  $ ./bin/gremlin.sh
  #启动console后连接8182端口
  ==> Configured localhost/127.0.0.1:8182
  
  #第二种方式
  #重命名服务器 :latest最新容器
  $ docker run --name janusgraph-default janusgraph/janusgraph:latest
  #建立第二个容器作为Console 链接到服务器，并执行 ./bin/gremlin.sh命令
  $ docker run --rm --link janusgraph-default:janusgraph -e \   		            GREMLIN_REMOTE_HOSTS=janusgraph \
   -it janusgraph/janusgraph:latest ./bin/gremlin.sh
  #连接8182端口
  ==> Configured localhost/127.0.0.1:8182
  ```

* 本地安装，下载安装文件后

  ```shell
  #打开服务器
  $ ./bin/gremlin-server.sh start
  #启动Console
  $ ./bin/gremlin.sh
  #启动console后连接8182端口
  ==> Configured localhost/127.0.0.1:8182
  ```

## Step2 JanusGraph使用

* docker加载“诸神关系图”

  ```shell
  gremlin> graph = JanusGraphFactory.open('conf/janusgraph-inmemory.properties')
  ==>standardjanusgraph[cql:[127.0.0.1]]
  gremlin> GraphOfTheGodsFactory.loadWithoutMixedIndex(graph, true)
  ==>null
  gremlin> g = graph.traversal()
  ==>graphtraversalsource[standardjanusgraph[cql:[127.0.0.1]], standard]
  ```

* JanusGraph使用，首先通过index key索引键（这里是name）找到一个顶点（saturn）

  vertex.values('key')取顶点的属性

  vertex.label()取顶点的标签

  通过边前往其他顶点：

  vertex.in('key')指向该节点的边键值为key的顶点

  vertex.out('key')该节点指向的边键值为key的顶点

  ```shell
  #通过名字查找到土星saturn
  gremlin> saturn = g.V().has('name', 'saturn').next()
  #查看土星的value
  gremlin> g.V(saturn).valueMap()
  ==>[name:[saturn], age:[10000]]
  #查看土星的孙子的名字
  gremlin> g.V(saturn).in('father').in('father').values('name')
  ==>hercules
  #建立Hercules顶点
  gremlin> hercules = g.V(saturn).repeat(__.in('father')).times(2).next()
  ==>v[1536]
  #查看Hercules的父母名字
  gremlin> g.V(hercules).out('father', 'mother').values('name')
  ==>jupiter
  ==>alcmene
  #查看Hercules的父母种族
  gremlin> g.V(hercules).out('father', 'mother').label()
  ==>god
  ==>human
  #查看和Hercules打过架的
  gremlin> g.V(hercules).out('battled').valueMap()
  ==>[name:[nemean]]
  ==>[name:[hydra]]
  ==>[name:[cerberus]]
  
  ```

  