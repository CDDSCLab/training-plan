# 实验一 MySQL的安装和使用  
我的基本实验环境是在Windows下安装了VMware Workstation，使用的是Ubuntu系统  


## Step1:Docker的安装 



更新软件列表  
  sudo apt-get update  
安装Docker  
  sudo apt install docker.io  
查看Docker是否安装成功  
  docker version  
启动Docker  
  systemctl start docker.service  
Docker项目实例：从官方仓库拉取image文件  
  docker image pull library/hello-world(其中library/hello-world是image文件在仓库里面的位置，library是image文件所在的组，hello-world是image文件的名字）  
查看是否拉取成功  
  docker image ls  
运行这个容器  
  docker container run hello-world(看到Hello from Docker即为成功)  


## Step2:MySQL的安装 



从Docker仓库中拉取MySQL镜像  
  docker pull mysql  
查看是否拉取成功  
  docker images  
运行MySQL容器并设置密码  
  docker run -d -e MYSQL_ROOT_PASSWORD=zhoufan19970217 --name mysql-master -p 3307:3306 mysql  
查看是否安装成功  
  docker ps  
连接Docker中的MySQL  
  docker exec -it mysql-master /bin/bash   
启动容器服务  
  docker start mysql-master 
登录MySQL  
  mysql -u root -p  


## Step3:MySQL的增删改查  



* 创建数据库  
  create database mysql_fanfan;  
* 删除数据库  
  drop database mysql_fanfan;  
* 查看所有数据库  
  show databases;  
* 创建student表  
  create table student(        
    'id' int(12) not null PRIMARY key,      
    'name' varchar(12),  
    'age' varchar(12),  
    'sex' varchar(12)  
  ); 
* 创建student_info表  
  create table student_info(    
    'id' int(12) not null PRIMARY key auto_increment,    
    'name' varchar(12)    
  );  
* 在student表中增加一条数据  
  insert into student(id,name,age,sex) values(1,'zhoufan',23,'girl');  
* 在student_info表中增加多条数据  
  insert into student_info(name) values('jk'),('jimin'),('jhope'),('suga');  
* 查询单条数据  
  select * from student_info where id=5;  
* 更新某条数据  
  update student_info set name='haoxi' where id=5;  
* 删除一条记录  
  delete from student_info where id=5;  
* 删除student表  
  drop table student;  
* 查看表的字段信息  
  desc student;  
* 清空表信息  
  truncate table student; 


## 遇到的问题  


1. Docker只启动了客户端，没有启动服务端  
  >原因：权限问题  
  >方法：添加用户到管理组，以下是代码  
        * sudo gpasswd -a $fanfan0217 docker(fanfan0217为用户名，结果显示用户不存在)  
        * sudo gpasswd -a $USER docker(添加用户到管理组)  
        * newgrp docker  
        * docker version  
2. 不知道是否成功连接到Docker中的MySQL  
  >原因：宿主机和Docker都安装了MySQL,所以把两种进入数据库方式弄混了  
  >方法:* mysql -h localhost -u root -p(登录宿主机MySQL)  
       * 登录Docker中的MySQL前面已叙述过   
3. 在登录数据库时不知道随机密码  
  >原因：百度了一些方法，一直没成功，最后还是解决了   
  >方法：* 查看数据库用户和密码  
               sudo cat /etc/mysql/debian.cnf   
        * 修改数据库密码  
                >update user set plugin="mysql_native_password";  
                >update mysql.user set authentication_string=password('123456') where user='root' and Host='localhost';  
                >sudo service mysql restart  

 ## TODO  



          -了解一个SQL的执行过程
          -多学习Docker，MySQL的底层知识，现在只懂一点表面的东西，同时还要多学学理论知识  




#实验二 redis的安装和使用  


因为自己是按照菜鸟教程来学习的，所以redis的安装与MySQL的安装完全类似  

## Step1:Docker下载redis  



从Docker仓库中拉取redis最新镜像  
  docker pull redis：latest    
查看是否拉取成功  
  docker images  
运行redis容器  
  docker run -itd --name redis-test -p 6379:6379 redis  
查看是否安装成功  
  docker ps  
通过redis-cli连接测试使用redis服务    
  docker exec -it redis-test /bin/bash  
启动容器服务 (如果容器没启动)    
  docker start redis-test  
通过redis-cli连接  
  redis-cli  


## Step2:redis 五种数据类型  



### 字符串类型(String)  

string是redis最基本的类型，一个key对应一个value。  
string类型是二进制安全的。意思是redis的string可以包含任何数据，比如jpg图片或者序列化对象。最大存储容量512MB  

#### 具体操作

* 存储一个数据  

  set username fanfan  

* 获得一个数据  

  get username  

* 删除一个数据  

  del username  

* 整数i+4  

  incrby i 4  

* 整数i-3  

  decrby i 3  

### 哈希类型(Hash)  

Hash是一个键值(key=>value)对集合，是一个string类型的field和value的映射表，Hash特别适合用于存储对象

#### 具体操作

* 将哈希表blog中的字段field的值设为value(多个同时)  

  hmset blog title 'my first' auther 'zhoufan'  

* 获取所有给定字段的值  

  hmget blog title auther   

* 获取哈希表中的所有值  

  hgetall blog  

* 获取哈希表中所有字段值  

  hkeys blog  

* 获取哈希表中所有value值   

  hvals blog  

* 删除blog表中的多个数据  

  hdel blog auther username  

### 列表类型(List)  

redis列表是简单的字符串列表，按照插入顺序排序，你可以添加一个元素到列表的头部(左侧)或者尾部(右侧)

#### 具体操作  

* 从左侧增加元素b  

  lpush mylist b  

* 从右侧增加元素c  

  rpush mylist c  

* 从a前插入d  

  linsert mylist before a d  

* 返回所有元素  

  lrange mylist 0 -1  

* 从表尾开始删除两个a  

  lrem mylist -2 a  

* 从左侧删除  

  lpop mylist  

* 从右侧删除  

  rpop mylist  

### 集合类型(Set)  

redis的set是string类型的无序集合，不能出现重复的数据  

#### 具体操作  

* 向members集合里添加成员  

  sadd members fanfan jimin jin fanfan  

* 计算集合成员数量  

  scard members  

* 求两集合差集并存储在members2  

  sdiffstore members2 members members1  

* 返回members2集合中所有成员  

  smembers members2  

* 求两集合交集并存储在members3  

  sinterstore members3 members members1  

* 将jimin从members2移动到members3  

  smove members2 members3 jimin  

* 移除members3集合中随机的一个元素并返回  

  spop members3  

* 移除集合中固定的元素  

  srem members3 jimin  

### 有序集合类型(Sorted Set/Zset)  

Zset和Set一样也是string类型元素的集合，且不允许重复的成员。不同的是每个元素都会关联一个Double类型的分数，redis正是通过分数来为集合中的成员进行从小到大的排序，Zset的成员是唯一的，但是分数却可以重复  

#### 具体操作  

* 向有序集合class中添加多个成员  

  zadd class 90 fanfan 100 jimin 35 jk 72 taetae  

* 计算集合成员数量  

  zcard class  

* 计算指定分数区间成员数量  

  zcount class 0 100  

* 对指定成员的分数加上增量  

  zincrby class 20 jk  

* 覆盖指定成员分数  

  zadd class 89 jk  

* 按分数递增排列  

  zrange class 0 -1 withscores  

* 按分数递减排列  

  zrevrange class 0 -1 withscore  

* 删除指定成员  

  zrem class jk  



## TODO  



          -从基础开始学redis，现在除了了解一些命令之外，对于redis的用途，概念还是很模糊





# 实验三 Mongodb的安装和使用  

因为自己是按照菜鸟教程来学习的，所以redis的安装与MySQL的安装完全类似  

## Step1:Docker下载Mongodb  



从Docker仓库中拉取redis最新镜像  
  docker pull mongo：latest    
查看是否拉取成功  
  docker images  
运行mongo容器(--auth:需要密码才能访问容器服务)  
  docker run -itd --name mongo -p 27017:27017 mongo --auth   
查看是否安装成功  
  docker ps  
连接mongodb的admin数据库  
  $docker exec -it mongo mongo admin  
创建用户名和密码  
  >db.createUser({user:'fanfan0217',pwd:'zhoufan19970217',roles:[{role:'userAdminAnyDatabase',db:'admin'}]});  
  >// 使用上面创建的用户信息进行登录  
  >db.auth('fanfan0217','zhoufan19970217') 


## Step2:Mongodb的增删改查  



1. 进入当前数据库  
> use admin  
> switched to db admin  

2. 展示用户  
> show users  
> ...
> "user" : "fanfan0217",  
> "db" : "admin",  
> ...

3. 创建集合 
> db.createCollection("students") 

4. 显示当前数据库的集合(students创建成功)          
> show collections  
> class  
> students  
> runoob  
> system.users  
> system.version  

5. 无需提前创建集合，通过插入语句，数据库也会自动创建集合  
> db.mycol2.insert({"name":"fanfan"})   
> writeResult({ "nInserted" : 1})   

6. 删除集合  
> db.mycol2.drop()  

7. 查看所有文档  
> db.col.find()  

8. 将数据定义为一个变量并插入  
> document=({title:'mongodb jiaocheng',description:'mongodb is a Nosql',by:'cainiaojiaocheng',url:'http://www.runoob.com',tags:['mongodb','database','Nosql'],likes:100});  

9. 更新文档(更新title的值)  
> db.col.update({'title':'mongodb'},{$set:{'title':'mongo'}})  

10. 使命令行更加完美的显示  
>db.col.find().pretty()  

11. 删除指定文档  
>db.col.remove({'title':'mongodb jiaocheng'})  
>WriteRedult({ "nRemove" : 2})  

12. 条件查询:by相当于MySQL中的where   
>dol.col.find({"by":"cainiaojiaocheng","title":"mongo"}).pretty()  



## 遇到问题  


1. 创建数据库出现问题    
  >原因：通过添加insert语句创建数据库，会出现认证问题，权限问题，认证过多问题等，这几个问题反反复复出现，明明解决了可是还是要出现，问题在于根本没懂启动mongodb的顺序问题，现在处于比较混乱的状态
  >方法：还没解决，之后会再看看，然后更新文件     
2. 添加用户时出现角色错误    
  >原因：不清楚用户自定义角色有限制，有些角色只适用于admin数据库
  >方法:db.grantRolesToUser("fanfan0217",[{role:"dbOwner",db:"admin"}])  


附具体角色  


Read：允许用户读取指定数据库  
readWrite：允许用户读写指定数据库  
dbAdmin:允许用户在指定数据库中执行管理函数
userAdmin：允许用户向system.users集合写入，可以找指定数据库里创建、删除和管理用户  
clusterAdmin：只在admin数据库中可用，赋予用户所有分片和复制集相关函数的管理权限  
readAnyDatabase：只在admin数据库中可用，赋予用户所有数据库的读权限  
readWriteAnyDatabase：只在admin数据库中可用，赋予用户所有数据库的读写权限  
userAdminAnyDatabase：只在admin数据库中可用，赋予用户所有数据库的userAdmin权限  
dbAdminAnyDatabase：只在admin数据库中可用，赋予用户所有数据库的dbAdmin权限  
root：只在admin数据库中可用。超级账号，超级权限  




##TODO  


          -Mongodb运行出错很多，之后还要多了解了解，多实践
          -先把所有命令行运行成功，再去了解基础知识   




# 实验四 Janusgraph的安装和使用

还不太理解，对于java，配置文件等都还很模糊，以下几乎是照着官方实例运行了一遍

JanusGraph是一个可扩展的图形数据库，已优化用于存储和查询包含分布在多计算机集群中的数千亿个顶点和边的图形，可以支持数千个并发用户实时执行复杂的图形遍历，支持ACID和最终的一致性  

## Step1:本地安装  

提前在Windows系统下安装了Janusgraph  

打开虚拟机的终端，安装桌面版的虚拟工具条，以支持Windows和Ubuntu之间的拖放    
  直接复制粘贴，然后解压      
把janusgraph文件剪切到home文件夹下，因为自己用cd命令行进不去桌面，所以选择移动文件   
进入home文件夹   
  cd /home  
进入janusgraph文件，其中fanfan0217为自己的虚拟机名字  
  cd ./fanfan0217/janusgraph-full-0.5.2   
运行Gremlin控制台，并通过控制台访问    
  bin/gremlin.sh  



## Step2:在Gremlin控制台模式下练习官方实例   


###加载众神图，选择conf/janusgraph-inmemory.properties快速打开一个内存后端(方法借鉴) 

* 加载所有边，顶点，属性等  

gremlin> graph=JanusGraphFactory.open('conf/janusgraph-inmemory.properties')  
==>standardjanusgraph[inmemory；[127.0.0.1]]  
gremlin> GraphOfTheGodsFactory.localWithoutMixedIndex(graph,ture)  
==>null  

* 遍历数据  

gremlin> g=graph.traversal()  
==>graphtraversalsource[standardjanusgraph[inmemory:[127.0.0.1]],standard]  

在图形数据库中访问数据的典型模式是首先使用图形索引将入口点定位到图形中，该入口点是一个元素(或元素集),即一个顶点或边

* 获取节点名字为saturn的位置  

gremlin> saturn=g.V().has('name','saturn').next()  
==>v[4160]  

* 获取saturn的信息  

gremlin> g.V(saturn).valueMap()  
==>[name:[saturn],age:[10000]]  

* 查看saturn的孙子为hercules  

gremlin> g.V(saturn).in('father').in('father').values('name')  
==>hercules  

属性place是边缘属性。可以通过该属性了解雅典50公里内发生的所有事情

* 查找Athens方圆50公里发生的事件  

gremlin> g.E().has('place',geoWithin(Geoshape.circle(37.97,23.72,50)))
==>e[4qw-6dc-7x1-374][8256-battled->4144]
==>e[4co-6dc-7x1-6gg][8256-battled->8368]

在上面的命令中，知道了saturn的孙子是hercules,现在通过saturn来了解hercules  

* 得到hercules顶点  

gremlin> hercules=g.V(saturn).repeat(__.in('father')).times(2).next()  
==>v[8256]  

* 得到hercules父母的顶点  

gremlin> g.V(hercules).out('father','mother')  
==>v[4272]  
==>v[4216]  

* 得到父母的名字  

gremlin> g.V(hercules).out('father','mother').values('name')  
==>jupiter  
==>alcmene  

* 得到父母的身份信息  

gremlin> g.V(hercules).out('father','mother').label()  
==>god  
==>human  

* 得到hercules的身份信息  

gremlin> hercules.label()  
==>demigod  

上述的例子都是关于神殿中各个角色的遗传系。通过众神图得知hercules参与了附近的两次战斗，现在通过battled从hercules顶点移除边来探索这些事情  

* 得到与hercules战斗的顶点  

gremlin> g.V(hercules).out('battled')  
==>v[4144]  
==>v[8368]  
==>v[12368]  

* 得到上述顶点的相关信息

gremlin> g.V(hercules).out('battled').valueMap()  
==>[name:[nemean]]  
==>[name:[hydra]]  
==>[name:[cerberus]]  

* 得到hydra和cerberus字段  

gremlin> g.V(hercules).outE('battled').has('time',gt(1)).inV().values('name')  
==>cerberus  
==>hydra  



###更复杂的图遍历示例


塔塔鲁斯的同居者  

* 获取节点名字为pluto顶点的位置   

gremlin> pluto=g.V().has('name','pluto').next()    
==>v[8200]  

* 得到pluto的同居者  

gremlin> g.V(pluto).out('lives').in('lives').values('name')  
==>pluto  
==>cerberus  

* pluto不是自己的同居者，通过下面语句得到正确的同居者  

gremlin> g.V(pluto).out('lives').in('lives').where(is(neq(pluto))).values('name')  
==>cerberus   

查找pluto的兄弟

* 查找pluto的兄弟生活在哪  

gremlin> g.V(pluto).out('brother').out('lives').values('name')  
==>sky  
==>sea  

* 查看每个兄弟住在哪个地方  

gremlin> g.V(pluto).out('brother').as('god').out('lives').as('place').select('god','place')  
==>[god:v[4272],place:v[4176]]  
==>[god:v[8272],place:v[4104]]   

* 查看每个兄弟的名字和住址  

gremlin> g.V(pluto).out('brother').as('god').out('lives').as('place').select('god','place').by('name')  
==>[god:jupiter,place:sky]    
==>[god:neptune,place:sea]  

pluto住在塔塔鲁斯，因为他不关心死亡，现在查询这些兄弟选择住址的原因  

* pluto选择住址的原因  

gremlin> g.V(pluto).outE('lives').values('reason')  
==>no fear of death  