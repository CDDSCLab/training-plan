# 实验一 MySQL的安装和使用

## Step1：安装与增加root用户

 1. mysql的安装

    ```bash
    apt install mysql-server
    ```

    这里会自动安装mysql的最新版本

    ![mysql安装成功](https://i.loli.net/2020/07/07/6zwRG5cNdXHWuVp.png)
    
    可以看到安装成功
    
 1.  mysql新增root用户

    百度说可以直接使用`mysql -u root -p`回车进入mysql,但是我尝试了是不行的。

    ```bash
    mysql_secure_installation
    ```

    使用以上命令为root用户更改密码

    ```
    Securing the MySQL server deployment.
    
    Enter password for user root: 
    
    VALIDATE PASSWORD COMPONENT can be used to test passwords
    and improve security. It checks the strength of password
    and allows the users to set only those passwords which are
    secure enough. Would you like to setup VALIDATE PASSWORD component?
    
    Press y|Y for Yes, any other key for No: n
    Using existing password for root.
    Change the password for root ? ((Press y|Y for Yes, any other key for No) : y
    
    New password: 
    
    Re-enter new password: 
    By default, a MySQL installation has an anonymous user,
    allowing anyone to log into MySQL without having to have
    a user account created for them. This is intended only for
    testing, and to make the installation go a bit smoother.
    You should remove them before moving into a production
    environment.
    
    Remove anonymous users? (Press y|Y for Yes, any other key for No) : y
    Success.
    
    
    Normally, root should only be allowed to connect from
    'localhost'. This ensures that someone cannot guess at
    the root password from the network.
    
    Disallow root login remotely? (Press y|Y for Yes, any other key for No) : n
    
     ... skipping.
    By default, MySQL comes with a database named 'test' that
    anyone can access. This is also intended only for testing,
    and should be removed before moving into a production
    environment.
    
    
    Remove test database and access to it? (Press y|Y for Yes, any other key for No) : y
     - Dropping test database...
    Success.
    
     - Removing privileges on test database...
    Success.
    
    Reloading the privilege tables will ensure that all changes
    made so far will take effect immediately.
    
    Reload privilege tables now? (Press y|Y for Yes, any other key for No) : y
    Success.
    
    All done! 
        
    ```

    修改后输入密码成功进入mysql

    ![mysql进入成功](https://i.loli.net/2020/07/07/Rk8XjlEYospZzDw.png)

## Step2：Mysql基本命令使用

```sql
CREATE DATABASE IF NOT EXISTS lab
	DEFAULT CHARACTER SET UTF8
	COLLATE UTF8_GENERAL_CI;#指定排序

USE lab;

DROP TABLE IF EXISTS student;

CREATE TABLE student 
(
	id int(11),
	name varchar(20),
	age int(11),
	PRIMARY KEY(id)
)
 ENGINE=InnoDB
 DEFAULT CHARSET =utf8;


DROP TABLE IF EXISTS student_info;

CREATE TABLE student_info
(
	id int(11),
	identity char(18),
	class varchar(20),
	address varchar(200),
	PRIMARY KEY(id),
	KEY fk_id (id),
	CONSTRAINT fk_id FOREIGN KEY (id) REFERENCES student(id)
	ON DELETE CASCADE ON UPDATE CASCADE

)
 ENGINE=InnoDB
 DEFAULT CHARSET =utf8;
INSERT INTO student (id,name,age) values (11111,'fanqie',22);
INSERT INTO student_info (identity,id,class,address) values ('320555192303341918',11111,'一班','电子科技大学');
select student.id,class,identity,address,name,age from student ,student_info as info where student.id=info.id AND student.id=11111;
```

## 遇到的问题：

## TODO：

* 对于底层架构进行理解
* 对于索引的建立使用进行理解

# 实验二 Mongodb的安装和使用

## Step 1 Mongodb介绍与安装

1. 存储结构介绍
   1. ObjectId
          Mongodb自身提供ObjectId,如果在创建时不自己提供id，它会根据所在服务器进程和时间生成id,相比于Mysql提供的自增id，避免在大量访问的时候必须顺序插入来保证id的唯一的缺点。

   2. 数据结构层次

      Mongodb中数据库的存储分为三个层次，database,colletction和document。每一个数据库会有自己的文件夹，数据的文件内部被分为多个块，集合存储着文档。mongodb中最大的名称空间为集合，mongodb每一个索引对应着一个名称空间。在内部存储中集合和索引的存储是相似的。Mongodb底层数据结构为B树。

2. Mongodb安装

```bash
	sudo apt install mongodb
```

在ubuntu下会进行自动的安装，并且设置自启动



## Step 2 Mongodb的基本使用

1. Mongodb数据类型

   1. Json

      Json是一种存储交换数据格式的文件，它可以包含字典，字符串，数组等数据。在Mongodb中，可以直接将Json数据转化为对应的document存入mongodb之中。

   2. Mongodb数据类型

      1. document:也称Object,字典类型
      
      2. Array:数组类型，可以方便的对于数组进行过滤操作
   
 1. CDUR

     1. 插入

         1. InsertOne
            
            ```js
            MongoDB Enterprise atlas-ynjgr4-shard-0:PRIMARY> db.movies.insertOne({"title":"Lion King","year": 1847,"imdb":"tt0084725"})
            {
            	"acknowledged" : true,
            	"insertedId" : ObjectId("5f05d8f7c43b33918dc8a5ad")
            }
            MongoDB Enterprise atlas-ynjgr4-shard-0:PRIMARY> db.movies.insertOne({"_id":"fadfjaodfjadoif","title":"Lion King","year": 1847,"imdb":"tt0084725"})
            { "acknowledged" : true, "insertedId" : "fadfjaodfjadoif" }
            MongoDB Enterprise atlas-ynjgr4-shard-0:PRIMARY> db.movies.find()
            { "_id" : ObjectId("5f05d6121dd443c20b103fb7"), "title" : "Star Trek II:The Wrath of ", "year" : 1982, "imdb" : "tt0084726" }
            { "_id" : ObjectId("5f05d8f7c43b33918dc8a5ad"), "title" : "Lion King", "year" : 1847, "imdb" : "tt0084725" }
            { "_id" : "fadfjaodfjadoif", "title" : "Lion King", "year" : 1847, "imdb" : "tt0084725" }
            
            ```
            
            2.InsertMany
            
            ```js
            MongoDB Enterprise atlas-ynjgr4-shard-0:PRIMARY> db.movies.insertMany(
            	[
            		{
            			"_id": "tt0084726",
            			"title": "Star Trek II: The Wrath of Khan",
            			"year": 1982,
            			"type": "movie"
            		},
            		{
            			"_id": "tt0796366",
            			"title": "Star Trek",
            			"year":2009,
            			"type": "movie"
            		},
            	]
            
            )
            { "acknowledged" : true, "insertedIds" : [ "tt0084726", "tt0796366" ] }
            ```
           
            insertMany默认是顺序的，即当执行到一个错误的输入时，便不再执行，但是也可以指定为无序的。
            
            ```js
            MongoDB Enterprise atlas-ynjgr4-shard-0:PRIMARY> db.movies.insertMany(
            	[
            		{
            			"_id": "tt0084726",
            			"title": "Star Trek II: The Wrath of Khan",
            			"year": 1982,
            			"type": "movie"
            		},
            		{
            			"_id": "tt0796366",
            			"title": "Star Trek",
            			"year":2009,
            			"type": "movie"
            		},
            		{
            			"_id": "tt0084726",
            			"title": "Star Trek II: The Wrath of Khan",
            			"year": 1982,
            			"type": "movie"
            		},
            		{
            			"_id": "tt1408101",
            			"title": "Star Trek Into Darkness",
            			"year":2013,
            			"type": "movie"
            		}
            		
            	],
            	{
            		ordered: false
            	}
            
            )
            MongoDB Enterprise atlas-ynjgr4-shard-0:PRIMARY> db.movies.find()
            { "_id" : "tt0084726", "title" : "Star Trek II: The Wrath of Khan", "year" : 1982, "type" : "movie" }
            { "_id" : "tt0796366", "title" : "Star Trek", "year" : 2009, "type" : "movie" }
            { "_id" : "tt1408101", "title" : "Star Trek Into Darkness", "year" : 2013, "type" : "movie" }
            ```
            
            虽然这里面有两个与之前的输入重复，一个与输入重复，但是第四个依然成功的存储入了数据库之中。即指定无序以后在遇到错误时不会停下等待
            
         2. 寻找

            1. find()

               ```js
               MongoDB Enterprise atlas-ynjgr4-shard-0:PRIMARY> db.movies.find({year:2013}).pretty()
               {
               	"_id" : "tt1408101",
               	"title" : "Star Trek Into Darkness",
               	"year" : 2013,
               	"type" : "movie"
               }
               ```

               find()意味着寻找，在find里面的{}可以指定寻找参数

               多层级的寻找可以用全路径指定Object寻找

               ```js
               db.movieDetails.find({"awards.wins":2}).pretty()
               ```

               数组的寻找如下

               ```bash
               db.movieDetails.find({actors:["Andy Garcia"]})#寻找只有Andy Garcia的
               db.movieDetails.find({actors:"Andy Garcia"})#寻找其中一个是Andy Garcia的
               db.movieDetails.find({"actors.0":"Andy Garcia"})#寻找第一个为Andy Garcia的
               ```

         3. 更新

            ```bash
            db.movieDetails.updateOne({
            	title:"The Martian"
            },{
            	$set:{
            		poster:"http://ia.medis-imdb.com"}
            })
            db.movieDetails.updateOne({
            	title:"The Martian"
            },{
            	$set:{
            		"awards":{
            		"wins":8,
            		"nominatioins":14,
            		"text":"Nominated for 3 Gold"}
            })
            db.movieDetails.updateOne({
            	title:"The Martian"
            },{
            	$push:{
            		reviews:{
            		rating:4.5
            		date:ISODate("2016-01-12T09:00:00Z"),
            		reviewer:"Spencer H.",
            		text:reviewText1
            		}
            })
            ```

            updateOne用于更新一个，第一个参数制定需要更新的对象，第二个参数指定需要更新对象中的属性。如果匹配到多个值，默认更新第一个匹配到值

            ```txt
            $set：设置值为指定的值
            $unset:删除指定的值
            $push:将一个数据变量推入到指定的Array内
            $pushAll:将所有的变量推入到指定的Array内
            $pull:清空所有的Array变量
            $pullAll:清空所有指定的Array变量
            ```

            updateMany的操作与update一致，只是会更新所有遇到的值。

            在某些情况下，我们需要如果不存在则插入，存在则更新。将第三个参数中的upsert设为true便可以达成目的

            ```js
            db.movieDetails.updateOne({
            	title:"The Martian"
            },{
            	$set:{
            		"awards":{
            		"wins":8,
            		"nominatioins":14,
            		"text":"Nominated for 3 Gold"}
            }，{
            upsert:true
            })
            ```

            此外还可以通过replaceOne进行数据的更改

            ```js
            > let filter={title:"Star Trek II: The Wrath of Khan"}
            > let doc =db.movieDetails.findOne(filter);
            > doc.poster;
            http://ia.media-imdb.com/images/M/MV5BMTcwNjc5NjA4N15BMl5BanBnXkFtZTcwNDk5MzI4OA@@._V1_SX300.jpg
            > doc.poster="http://www.imdb.com/title/tt1329164/mediaviewer/rm2619416575";
            http://www.imdb.com/title/tt1329164/mediaviewer/rm2619416575
            > doc.genres;
            [ "Action", "Adventure", "Drama" ]
            > doc.genres.push("TV Series");
            4
            > db.movieDetails.replaceOne(filter,doc);
            { "acknowledged" : true, "matchedCount" : 1, "modifiedCount" : 1 }
            
            ```

            replaceOne第一个参数为所匹配的值，第二个参数为所替换的值

         4. 删除

            ```js
            db.moiveDetails.deleteOne({title:"Star Trek II: The Wrath of Khan"})
            db.moiveDetails.deleteMany({"awards.win":8})
            ```

            与insertOne,insertMany类型，删除也可以删除一个或者多个
         
     2. 高级函数

         1. 比较操作

             | Name |      Description      |
             | :--: | :-------------------: |
             | $eq  |         等于          |
             | $gt  |         大于          |
             | $gte |       大于等于        |
             | $in  | 匹配具有在Array内的值 |
             | $lt  |         小于          |
             | $lte |       小于等于        |
             | $ne  |        不等于         |
             | $nin | 匹配不具有Array内的值 |

         2. 元素操作

             1. exist

                 ```js
                 db.movieDetails.find({"type":$exists: true})
                 db.movieDetails.find({"type":$exists: false})
                 ```

                 判断这个属性在document里面是否存在，true为存在，false为不存在

             2. type

                 ```js
                 db.movieDetails.find({"type":$type :"string"})
                 ```

                 判断属性域的值是否为指定的值类型

         3. 逻辑操作

             1. $or

                匹配满足其中一个表达式的值

                ```js
                db.moiveDetails.find( { $or: [ { "awards.wins": { $lt: 20 } }, { type: "moive" } ] } )
                ```

                匹配得奖小于20或者类型是moive的类型的值

             2. $and

                匹配满足所有表达式的值

                ```js
                 db.moiveDetails.find( { $and: [ { "awards.wins": { $lt: 20 } }, { type: "moive" } ] } )
                ```

                匹配得奖小于20且类型是moive的类型的值

             3. $not

                匹配且满足表达式要求的值和不匹配表达式的值

                ```js
                db.moiveDetails.find( { $not: [ { "awards.wins": { $lt: 20 } } ] } )
                ```

                匹配得奖少于20或者不存在awards.wins域的值

             4. $nor

                匹配所有不满足表达式其中的任何一个值的值

                ```js
                db.moiveDetails.find( { $nor [ { "awards.wins": { $lt: 20 } }, { type: "moive" } ] } )
                ```

                匹配得奖不小于20且类型不为moive的值

         4. 数组操作

             1. $all

                 匹配属性的数组所有包含的指定值

                 ```js
                 db.moiveDetails.find(writers:{$all:["Gene Roddenberry", "Harve Bennett"]})
                 ```

                 匹配作者中里面有Gene Roddenberry, Harve Bennett的值。

             2. $size

                 匹配属性的数组和指定的数组长度一致的值

                 ```js
                 db.moiveDetails.find(writers:{$size:2})
                 ```

                 匹配作者为两个的值

             3. $elemMatch

                 匹配字典内的值

                 ```js
                 db.movieDetails.find({boxOffice:{$elemMatch:	{"country":"Germany","revenue":{$gt:16}}}})
                 ```

                 匹配在数组中的元素同时满足这两个条件的值，也就是在数组中的元素满足国家为"Germany","revenue"大于16的值
                 
## 遇到的问题：

远程服务器连接不上，后来连接上也是十分的卡，经常掉线，之后就全部在本地测试了

  ## TODO：
* 对于名称空间进行理解

* 学习如何实现支持多条读写指令的事务

     

# 实验三 JanusGraph的安装和使用

## Step 1 JanusGraph安装

这里使用的是docker进行的安装，

```bash
docker run --name janusgraph-default janusgraph/janusgraph:latest
```

这会自动安装并且启动janugraph server,

```bash
docker run --rm --link janusgraph-default:janusgraph -e GREMLIN_REMOTE_HOSTS=janusgraph \
    -it janusgraph/janusgraph:latest ./bin/gremlin.sh
```

将client连接到server上

## Step 2基础使用

1. 加载图数据

```java
gremlin> graph = JanusGraphFactory.open('conf/janusgraph-inmemory.properties')
==>standardjanusgraph[inmemory:[127.0.0.1]]
gremlin> GraphOfTheGodsFactory.loadWithoutMixedIndex(graph, true)
==>null
gremlin> g=graph.traversal()
==>graphtraversalsource[standardjanusgraph[inmemory:[127.0.0.1]], standard]
```

2. 节点数据的获取

   ```sh
   gremlin> saturn = g.V().has('name', 'saturn').next()#获取节点数据并且获取name为staurn的所有节点进行next遍历
   ==>v[4176]
   gremlin>  g.V(saturn).valueMap()#获取键值对
   ==>[name:[saturn],age:[10000]]
   gremlin> g.V(saturn).in('father').in('father').values('name')#获取父节点的父节点的值
   ==>hercules
   ```

3. 边数据的获取

   ```bash
   gremlin> g.E().has('place', geoWithin(Geoshape.circle(37.97, 23.72, 50))).as('source').inV().as('god2').select('source').outV().as('god1').select('god1', 'god2').by('name')
   14:34:17 WARN  org.janusgraph.graphdb.transaction.StandardJanusGraphTx  - Query requires iterating over all vertices [(place geoWithin BUFFER (POINT (23.72 37.97), 0.44966))]. For better performance, use indexes
   ==>[god1:hercules,god2:nemean]
   ==>[god1:hercules,god2:hydra]
   ```

   获取有边从(37,97)指向(23.72,50)的两个节点数据

4. 基本遍历

   ```bash
   gremlin> hercules = g.V(saturn).repeat(__.in('father')).times(2).next()#获取孙子节点
   ==>v[4312]
   ```

   _.in("father")表示自己的孩子节点，times(2)便是重复两次

   ```bash
   gremlin> g.V(hercules).out('father', 'mother')
   ==>v[4128]
   ==>v[8248]
   gremlin> g.V(hercules).out('father', 'mother').values('name')
   ==>jupiter
   ==>alcmene
   gremlin>  g.V(hercules).out('father', 'mother').label()
   ==>god
   ==>human
   gremlin> hercules.label()
   ==>demigod
   ```

   out表示取出几点，参数可以为一个或者多个，values表示属性值，label为标签值

   ```bash
   gremlin> g.V(hercules).out('battled')
   ==>v[4112]
   ==>v[4240]
   ==>v[8408]
   gremlin> g.V(hercules).out('battled').valueMap()
   ==>[name:[nemean]]
   ==>[name:[cerberus]]
   ==>[name:[hydra]]
   gremlin> g.V(hercules).outE('battled').has('time', gt(1)).inV().values('name')
   ==>cerberus
   ==>hydra
   ```

   out()函数会取出所得函数值，outE满足限定的条件的值，这里表示取出大于战争次数为1的值。

## 遇到的问题

在示例数据加载时遇到了许多问题

## TodoList

1. 深入学习图数据库的用法
2. 学习图数据库后端结构。

# 实验四 Redis的安装和使用

## Step1 Redis安装

redis在ubuntu下进行安装使用

```bash
sudo apt-get install redis-server
```

## Step2 Redis的初步使用

1. 启动命令行

   ```sh
    redis-cli -h 127.0.0.1 -p 6379
   ```

2. 字符串类型

   1. SET/GET命令

      redis的key为String类型，同时String也是redis的基本类型之一，一般来说redis的String类型长度不能超过512MB

      ```sh
      	set key value [expiration EX seconds|PX milliseconds] [NX|XX]
      ```

      set命令格式如上，key为关键字，value为值，expiration为过期时间，nx,xx分别是在key存在时不更改和存在时不更改。

      ```sh
      get key
      ```

      get用于获取对应的key值

   	```sh
   	127.0.0.1:6379> set mykey somevalue
   	OK
   	127.0.0.1:6379> get mykey
   	"somevalue"
   	127.0.0.1:6379> set mykey newvalue
   	OK
   	127.0.0.1:6379> get mykey
   	"newvalue"
   	127.0.0.1:6379> set mykey newvalue nx#存在更改失败
   	(nil)
   	127.0.0.1:6379> set mykey newvalue xx#存在更改成功
   	OK
   	127.0.0.1:6379> set newkey newvalue xx#不存在更改失败
   	(nil)
   	127.0.0.1:6379> set newkey newvalue nx#不存在更改成功
   	OK
   	```

   3. INCR/DECR/INCRBY/DECRBY

      ```sh
      127.0.0.1:6379> set counter 100
      OK
      127.0.0.1:6379> incr counter
      (integer) 101
      127.0.0.1:6379> incrby counter 50
      (integer) 151
      127.0.0.1:6379> DECR counter
      (integer) 150
      127.0.0.1:6379> DECRBY counter 50
      (integer) 100
      127.0.0.1:6379> incr mykey
      (error) ERR value is not an integer or out of range 
      ```

      以上四个命令都是对于Integer类型的值进行操作，对于非Integer类型的值操作会报错，以上几个命令都是原子性的
      
   3. MSET/MGET

      ```sh
      127.0.0.1:6379> mset a 10 b 20 c 30
      OK
      127.0.0.1:6379>  mget a b c
      1) "10"
      2) "20"
      3) "30"
      ```

      MSET和MGET用于批量化的赋值和获取值

   4. EXIST/DEL/TYPE

      ```sh
      127.0.0.1:6379> exists mykey newkey
      (integer) 2
      127.0.0.1:6379> del mykey
      (integer) 1
      127.0.0.1:6379> exists mykey newkey
      (integer) 1
      ```

      exists返回值为所查询的key存在的数目，del为删除成功的数目

      ```sh
      127.0.0.1:6379> type newkey
      string
      127.0.0.1:6379> del newkey
      (integer) 1
      127.0.0.1:6379> type newkey
      none
      ```

      type返回的是数据的类型，查询不存在的值返回的类型为none。

   5. 过期时间

      ```sh
      127.0.0.1:6379> set key som-value
      OK
      127.0.0.1:6379> expire key 5
      (integer) 1
      127.0.0.1:6379> get key
      "som-value"
      127.0.0.1:6379> get key
      (nil)
      127.0.0.1:6379> ttl key
      (integer) -2
      127.0.0.1:6379> set key 100 ex 10
      OK
      127.0.0.1:6379> ttl key
      (integer) 7
      127.0.0.1:6379> set key 100 px 10000000
      OK
      127.0.0.1:6379> ttl key
      (integer) 9998
      127.0.0.1:6379> pttl key
      (integer) 9913147
      ```

      ttl返回的是离过期还有多少，单位为秒，对于已经过期或者不存在的key返回的为-2,如果需要以毫秒进行获取可以使用pttl进行。

3. List类型

   redis类型的List时线性表类型，对于基于下标的访问速度较慢但是插入较快

   1. LPUSH/RPUSH

      LPUSH意味着将数据放在最左边（放在头），RPUSH则意味着将数据放在最右边（放在尾）。

      ```sh
      127.0.0.1:6379> rpush mylist A
      (integer) 1
      127.0.0.1:6379> rpush mylist B
      (integer) 2
      127.0.0.1:6379> lpush mylist first
      (integer) 3
      127.0.0.1:6379> lrange mylist 0 -1
      1) "first"
      2) "A"
      3) "B"
      ```

      LRANGE 用于显示List内的内容，格式如下

      ```sh
      lrange key start stop
      ```

      LPUSH和RPUSH也可以一次增加多个值

      ```sh
      127.0.0.1:6379> rpush mylist 1 2 3 4 5 "foo bar"
      (integer) 9
      127.0.0.1:6379> lpush mylist 1 2 3 4 5 "foo bar"
      (integer) 15
      ```

   2. RPOP/LPOP

      ```sh
      127.0.0.1:6379> rpush mylist a b c
      (integer) 3
      127.0.0.1:6379> rpop mylist
      "c"
      127.0.0.1:6379> rpop mylist
      "b"
      127.0.0.1:6379> rpop mylist
      "a"
      127.0.0.1:6379> rpop mylist
      (nil)
      ```

      RPOP用于删除列表中的值，当列表中的值数目为0时，返回null。

   3. LTRIM

      ```sh
      127.0.0.1:6379> rpush mylist 1 2 3 4 5
      (integer) 5
      127.0.0.1:6379> ltrim mylist 0 2
      OK
      127.0.0.1:6379> lrange mylist 0 -1
      1) "1"
      2) "2"
      3) "3"
      ```

      LTRIM用于提取出子数组。

   4. BRPOP

      ````sh
      127.0.0.1:6379> brpop tasks 5
      (nil)
      (5.05s)
      127.0.0.1:6379> lpush tasks tasks frist
      (integer) 2
      127.0.0.1:6379> brpop tasks 5
      1) "tasks"
      2) "tasks"
      127.0.0.1:6379> lrange tasks 0 -1
      1) "frist"
      ````

      BRPOP用于提取数组中的数据，BRPOP提取的数据是阻塞的，最后一个数组指定阻塞多久，如果到了指定的时间仍没有数据的话，返回null。

      与RPOP不同，BRPOP是按照阻塞顺序执行，且执行完成后，会返回一个key和value。

4. RedisHashes

   1. 赋值与取值

      ```sh
      hmset key field value [field value ...]
      hmget key field [field ...]
      ```

      hmset第一个field为key，其他的field为value,hmget也是以第一个field为key，其他为value。如果后续field指定了不存在的field则为null。

      ```sh
      127.0.0.1:6379> hmset user:1000 username antirez birthyear 1977 verified 1 
      OK
      127.0.0.1:6379> hgetall user:1000 
      1) "username"
      2) "antirez"
      3) "birthyear"
      4) "1977"
      5) "verified"
      6) "1"
      127.0.0.1:6379> hget user:1000 username
      "antirez"
      127.0.0.1:6379> hmget user:1000 username xxx
      1) "antirez"
      2) (nil)
      ```

      hgetall获取key的全部value,hget获取一个指定的value,hmget获取多个指定的value。

      ```sh
      127.0.0.1:6379> hincrby user:1000 birthyear 10
      (integer) 1987
      ```

      hincrby用于更改属性中的int值。

      ```sh
      127.0.0.1:6379> hmset user:1000 username xxx no_such_field yes
      OK
      127.0.0.1:6379> hgetall user:1000
      1) "username"
      2) "xxx"
      3) "birthyear"
      4) "1987"
      5) "verified"
      6) "1"
      7) "no_such_field"
      8) "yes"
      ```

      对于一个已存在的key进行赋值时，如果key存在会更改key的值，如果key不存在则会新建key的值。

5. Set

   1. 赋值与获取

      ```sh
      127.0.0.1:6379> sadd myset 1 2 3
      (integer) 3
      127.0.0.1:6379> smembers myset
      1) "1"
      2) "2"
      3) "3"
      ```

      sadd为key新增值，smembers获取值。

   2. 获取交集

      ```sh
      127.0.0.1:6379> sinter news:1000:tags myset
      1) "1"
      2) "2"
      ```

      sinter指定多个set获取他们的交集

   3. 弹出和批量赋值

      ```sh
      127.0.0.1:6379> sadd deck C1 C2 C3 C4 C5 C6 C7 C8 C9 C10 CJ CQ CK
      (integer) 13
      127.0.0.1:6379> sunionstore game:1:deck deck
      (integer) 13
      127.0.0.1:6379>  spop game:1:deck
      "C2"
      127.0.0.1:6379>  spop game:1:deck
      "C5"
      127.0.0.1:6379>  spop game:1:deck
      "C3"
      127.0.0.1:6379> scard game:1:deck
      (integer) 10
      127.0.0.1:6379> scard deck
      (integer) 13
      ```

      sunionstore 可以批量将一个set中的值赋值给另一个set，spop从set中随机弹出值，这种弹出不会影响到原来的set，scard统计给定key的长度。

6. 有序集合

   1. 增加/获取

      ```sh
      127.0.0.1:6379> zadd hackers 1940 "Alan Kay"
      (integer) 1
      127.0.0.1:6379> zadd hackers 1957 "Sophie Wilson"
      (integer) 1
      127.0.0.1:6379> zadd hackers 1953 "Richard Stallman"
      (integer) 1
      127.0.0.1:6379>  zadd hackers 1949 "Anita Borg"
      (integer) 1
      127.0.0.1:6379> zadd hackers 1965 "Yukihiro Matsumoto"
      (integer) 1
      127.0.0.1:6379> zadd hackers 1914 "Hedy Lamarr"
      (integer) 1
      127.0.0.1:6379> zadd hackers 1916 "Claude Shannon"
      (integer) 1
      127.0.0.1:6379>  zadd hackers 1969 "Linus Torvalds"
      (integer) 1
      127.0.0.1:6379> zadd hackers 1912 "Alan Turing"
      (integer) 1
      127.0.0.1:6379> zadd hackers 1912 "Alan Turig"
      (integer) 1
      127.0.0.1:6379> zrange hackers 0 -1
       1) "Alan Turig"
       2) "Alan Turing"
       3) "Hedy Lamarr"
       4) "Claude Shannon"
       5) "Anita Borg"
       6) "Richard Stallman"
       7) "Alan Kay"
       8) "Sophie Wilson"
       9) "Yukihiro Matsumoto"
      10) "Linus Torvalds"
      127.0.0.1:6379> zadd hackers 1957 "Alan Kay"
      (integer) 0
      ```

      zadd为所增添的值指定一个scores,对于同一个 scores可以指定多个值，但是对于同一个值不可以指定多个scores.

      zrange用于对于值的获取。

      ```sh
      127.0.0.1:6379> zrange hackers 0 -1 withscores
       1) "Alan Turig"
       2) "1912"
       3) "Alan Turing"
       4) "1912"
       5) "Hedy Lamarr"
       6) "1914"
       7) "Claude Shannon"
       8) "1916"
       9) "Anita Borg"
      10) "1949"
      11) "Richard Stallman"
      12) "1953"
      13) "Alan Kay"
      14) "1957"
      15) "Sophie Wilson"
      16) "1957"
      17) "Yukihiro Matsumoto"
      18) "1965"
      19) "Linus Torvalds"
      20) "1969"
      ```

      也可以带scores进行显示。

   2. 其余操作

      1.  zrangebyscore 

         ```sh
         127.0.0.1:6379>  zrangebyscore hackers -inf 1950
         1) "Alan Turig"
         2) "Alan Turing"
         3) "Hedy Lamarr"
         4) "Claude Shannon"
         5) "Anita Borg"
         ```

         获取socres大于某个值得数据集。

      2. zremrangebyscore

         ```sh
         127.0.0.1:6379> zremrangebyscore hackers 1940 1960
         (integer) 4
         127.0.0.1:6379> zrange hackers 0 -1 withscores
          1) "Alan Turig"
          2) "1912"
          3) "Alan Turing"
          4) "1912"
          5) "Hedy Lamarr"
          6) "1914"
          7) "Claude Shannon"
          8) "1916"
          9) "Yukihiro Matsumoto"
         10) "1965"
         11) "Linus Torvalds"
         12) "1969"
         ```

         删除在1940到1960之间的值

      3. zrank/zrevrank

         ```sh
         127.0.0.1:6379> zrank hackers "Alan Turig"
         (integer) 0
         127.0.0.1:6379>  zrevrank hackers "Alan Turig"
         (integer) 3
         ```

         zrank返回在从小到大排序的位置，zrevrank返回从小到大排序的位置。

      4. zrangebylex

         ```sh
         127.0.0.1:6379> zadd hackers 0 "B" 0 "C" 0 "A" 0 "D" 0 "D"
         (integer) 4
         127.0.0.1:6379> zrangebylex hackers [B [C
         1) "B"
         2) "C"
         ```

         获取value在所指定范围内的值。

7. Bitmaps

   bitmaps本身不是一种数据类型，它本身就是字符串，它可以对于字符串的位进行操作。

   ```sh
   127.0.0.1:6379> setbit key 10 1
   (integer) 0
   127.0.0.1:6379> setbit key 11 0
   (integer) 0
   ```

   setbit 是对与字符串指定的为进行操作，在第一条命令中就是将字符串的第十位赋值为了1，而第二条命令则是将字符串第十一位赋值为了0。

   ```sh
   127.0.0.1:6379> setbit key 10 1
   (integer) 0
   127.0.0.1:6379> setbit key 11 0
   (integer) 0
   127.0.0.1:6379> setbit key 13 1
   (integer) 0
   127.0.0.1:6379> bitcount key 
   (integer) 2
   ```

   bitcount用于统计key中为1的数组位的个数。用bitmaps可以方便的统计比如一天内的访问人数等信息。

8.  HyperLogLogs

   HyperLoglogs用于基数统计，即统计一个输入中不同值的个数，HyperLoglogs的特点是只需要消耗极少的空间便可以完成统计。

   ```sh
   127.0.0.1:6379> PFADD key 1 2 3 3 4 4
   (integer) 1
   127.0.0.1:6379> pfcount key
   (integer) 4
   ```

   PFADD 为指定key中添加值，pfcount统计不同个数的值。

