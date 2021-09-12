# 实验四 Janusgraph的安装与使用

[JanusGraph](https://docs.janusgraph.org/)是一个可扩展的图数据库，用于存储和查询分布在多机集群中的包含数千亿顶点和边的图。

### Step1：本地安装 - 安装Java

​	为管理方便在同一个Docker容器中安装四个数据库，选择在服务器Docker容器上进行本地安装。由于容器的image是最小版，没有安装java，所以先安装Java8。

①下载Java8 tar.gz 包：[Java官网](https://www.oracle.com/java/technologies/javase/javase-jdk8-downloads.html).

②解压Java包，配置路径

```bash
mkdir -p /usr/local/java
tar -vzxf jdk-8u241-linux-x64.tar.gz -C /usr/local/java/
```

③添加环境变量

```bash
vim /etc/profile
# 添加入下内容，并保存
export JAVA_HOME=/usr/local/java/jdk1.8.0_241
export CLASSPATH=$:CLASSPATH:$JAVA_HOME/lib/
export PATH=$PATH:$JAVA_HOME/bin
# 重新加载配置文件
source /etc/profile
```

④测试是否安装成功

```bash
> java -version
java version "1.8.0_301"
Java(TM) SE Runtime Environment (build 1.8.0_301-b09)
Java HotSpot(TM) 64-Bit Server VM (build 25.301-b09, mixed mode)
```

### step2：本地安装 - 安装Janusgraph数据库

①下载Janusgraph .zip包并解压，[github下载](https://github.com/JanusGraph/janusgraph/releases).

```bash
mkdir /usr/local/janusgraph
mv janusgraph-0.6.0.zip /usr/local/janusgraph
cd /usr/local/janusgraph
unzip janusgraph-0.6.0.zip
```

②运行Gremlin Console

```bash
> ./gremlin.sh
Sep 11, 2021 8:55:49 AM java.util.prefs.FileSystemPreferences$1 run
INFO: Created user preferences directory.

         \,,,/
         (o o)
-----oOOo-(3)-oOOo-----
SLF4J: Class path contains multiple SLF4J bindings.
SLF4J: Found binding in [jar:file:/usr/local/janusgraph/janusgraph-0.6.0/lib/slf4j-log4j12-1.7.30.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: Found binding in [jar:file:/usr/local/janusgraph/janusgraph-0.6.0/lib/logback-classic-1.1.3.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: See http://www.slf4j.org/codes.html#multiple_bindings for an explanation.
SLF4J: Actual binding is of type [org.slf4j.impl.Log4jLoggerFactory]
plugin activated: tinkerpop.server
plugin activated: tinkerpop.tinkergraph
08:55:52 WARN  org.apache.hadoop.util.NativeCodeLoader  - Unable to load native-hadoop library for your platform... using builtin-java classes where applicable
plugin activated: tinkerpop.hadoop
plugin activated: tinkerpop.spark
plugin activated: tinkerpop.utilities
plugin activated: janusgraph.imports
gremlin>
```

### step3：Janusgraph连接

①启动服务器

```bash
./janusgraph-server.sh start
# 要传入配置文件作为参数，使用
./janusgraph-server.sh console ./conf/gremlin-server/gremlin-server-[...].yaml
```

服务器的默认端口为8182，可以使用Janusgraph的:remote命令远程连接

```bash
./gremlin.sh
:remote connect tinkerpop.server conf/remote.yaml
# 得到结果：
==>Configured localhost/127.0.0.1:8182
```

### step4：Gremlin语法初见

>Gremlin是 Apache TinkerPop  框架下的图遍历语言。Gremlin是一种函数式数据流语言，可以使得用户使用简洁的方式表述复杂的属性图（property  graph）的遍历或查询。每个Gremlin遍历由一系列步骤（可能存在嵌套）组成，每一步都在数据流（data stream）上执行一个原子操作。

Gremlin Console 本身可以执行一些简单的运算：

![image-20210912225350181](C:\Users\11251\AppData\Roaming\Typora\typora-user-images\image-20210912225350181.png)

[JanusGraph基础使用教程](https://docs.janusgraph.org/getting-started/basic-usage/)

​	教程中的例子大量使用了一个使用Janusgraph表示的实例图，称为“诸神图”。这个特殊的实例描述了罗马万神殿中存在的人和地点之间的关系。该图（中英文版）如下所示：

#### 原版：

<img src="https://docs.janusgraph.org/getting-started/graph-of-the-gods-2.png" alt="Graph of the Gods" style="zoom:67%;" />

#### 中文版：

<img src="https://img-blog.csdn.net/20180103190001009" alt="img" style="zoom:67%;" />

| 标示           | 含义                                       |
| -------------- | ------------------------------------------ |
| 加粗的key      | 图索引键                                   |
| 加粗带星的key  | 必须有唯一值的图索引键                     |
| 带下划线的key  | 以顶点为中心的索引键                       |
| 空心箭头的边   | 函数性的（functional）/独特的边(不能重复） |
| 尾部为十字的边 | 单向边(只能在一个方向建立关系)             |

#### 使用过程：

```bash
graph = JanusGraphFactory.open('conf/janusgraph-inmemory.properties')
==>standardjanusgraph[inmemory:[127.0.0.1]]
# 加载图
gremlin> GraphOfTheGodsFactory.loadWithoutMixedIndex(graph, true)
==>null
gremlin> g = graph.traversal()
==>graphtraversalsource[standardjanusgraph[inmemory:[127.0.0.1]], standard]
```

​	以上`open`和`load`方法在返回新构造的图之前，执行以下操作：

 	1. 在图上创建全局和以顶点为中心的索引集合。
 	2. 将所有顶点及其属性添加到图中。
 	3. 将所有边及其属性添加到图中。

#### 全局图索引：

​	使用图数据库中数据的典型模式是首先使用图索引来确定起始点。该起始点是一个元素（或一组元素） - 即顶点或边。从起始点，Gremlin路径描述了如何通过图结构来遍历图中的其他点。通过name属性上的唯一索引，可以检索到Saturn顶点，然后可以查到它的所有属性值(即Saturn属性的键值对)。如图所示，顶点Saturn的名字是“Saturn”，年龄是10000，类别是“titan”。 Saturn的孙子通过遍历语句可以表示为：“谁是Saturn的孙子？” （“父亲”的反面是“孩子”）。 检索到的结果是Hercules。

```
gremlin> saturn = g.V().has('name', 'saturn').next()
==>v[256]
gremlin> g.V(saturn).valueMap()
==>[name:[saturn], age:[10000]]
gremlin> g.V(saturn).in('father').in('father').values('name')
==>hercules
```

​	属性place也在图索引中。属性place是边的一个属性。因此JanusGraph可以通过图的索引索引到边。它可以查询Gods图中发生在Athens(纬度:37.97, 经度:23.72) 50km范围内的信息。然后通过这些信息查看涉及到哪些顶点。

```
gremlin> g.E().has('place', geoWithin(Geoshape.circle(37.97, 23.72, 50)))
==>e[a9x-co8-9hx-39s][16424-battled->4240]
==>e[9vp-co8-9hx-9ns][16424-battled->12520]
gremlin> g.E().has('place', geoWithin(Geoshape.circle(37.97, 23.72, 50))).as('source').inV().as('god2').select('source').outV().as('god1').select('god1', 'god2').by('name')
==>[god1:hercules, god2:hydra]
==>[god1:hercules, god2:nemean]
```

​	图索引是JanusGraph中的一种索引结构。JanusGraph会自动使用索引来检索满足一个或多个约束条件的所有顶点(g.V)或边(g.E)。JanusGraph中另外一种索引是以顶点为中心的索引。以顶点为中心的索引可以加快图的遍历。

#### 图遍历示例：

​	前面证明了Saturn的孙子是Hercules，这也可以使用循环的方式来查询，实际上Hercules是从Saturn在in(‘father’) 路径上循环两次到达的点。

```
gremlin> hercules = g.V(saturn).repeat(__.in('father')).times(2).next()
==>v[1536]
```

​	Hercules是一个半神半人，为了证明Hercules半神半人，需要查询他父母的起源。可以从Hercules顶点去遍历他的母亲和父亲。最后可以去确定他们的类型是“god”和“human”。

```
gremlin> g.V(hercules).out('father', 'mother')
==>v[1024]
==>v[1792]
gremlin> g.V(hercules).out('father', 'mother').values('name')
==>jupiter
==>alcmene
gremlin> g.V(hercules).out('father', 'mother').label()
==>god
==>human
gremlin> hercules.label()
==>demigod
```

​	到目前为止，这些例子涉及了罗马万神殿中各种人物的遗传关系。 图的属性模型足以表示各种的事物和关系。比如说可以通过Hercules顶点遍历battled边，来发现Hercules参与了雅典附近的两场战争。

```
gremlin> g.V(hercules).out('battled')
==>v[2304]
==>v[2560]
==>v[2816]
gremlin> g.V(hercules).out('battled').valueMap()
==>[name:[nemean]]
==>[name:[hydra]]
==>[name:[cerberus]]
gremlin> g.V(hercules).outE('battled').has('time', gt(1)).inV().values('name')
==>cerberus
==>hydra
```

#### 更复杂的图遍历示例：

Tartarus的同居者

```
gremlin> pluto = g.V().has('name', 'pluto').next()
==>v[2048]
gremlin> // who are pluto's cohabitants?
gremlin> g.V(pluto).out('lives').in('lives').values('name')
==>pluto
==>cerberus
gremlin> // pluto can't be his own cohabitant
gremlin> g.V(pluto).out('lives').in('lives').where(is(neq(pluto))).values('name')
==>cerberus
gremlin> g.V(pluto).as('x').out('lives').in('lives').where(neq('x')).values('name')
==>cerberus
```

Pluto的兄弟

```
gremlin> // where do pluto's brothers live?
gremlin> g.V(pluto).out('brother').out('lives').values('name')
==>sky
==>sea
gremlin> // which brother lives in which place?
gremlin> g.V(pluto).out('brother').as('god').out('lives').as('place').select('god', 'place')
==>[god:v[1024], place:v[512]]
==>[god:v[1280], place:v[768]]
gremlin> // what is the name of the brother and the name of the place?
gremlin> g.V(pluto).out('brother').as('god').out('lives').as('place').select('god', 'place').by('name')
==>[god:jupiter, place:sky]
==>[god:neptune, place:sea]
```

​	最后Pluto居住在Tartarus因为他不在乎死亡。而他的兄弟们根据这些地方的品质来选择他们居住的地方。

```
gremlin> g.V(pluto).outE('lives').values('reason')
==>no fear of death
gremlin> g.E().has('reason', textContains('loves'))
==>e[6xs-sg-m51-e8][1024-lives->512]
==>e[70g-zk-m51-lc][1280-lives->768]
gremlin> g.E().has('reason', textContains('loves')).as('source').values('reason').as('reason').select('source').outV().values('name').as('god').select('source').inV().values('name').as('thing').select('god', 'reason', 'thing')
==>[god:neptune, reason:loves waves, thing:sea]
==>[god:jupiter, reason:loves fresh breezes, thing:sky]
```

-------------

### 遇到的问题

1. 在刚开始按照官方教程加载众神图配置时，遇到如下问题：

```gremlin
gremlin> graph = JanusGraphFactory.open('conf/janusgraph-berkeleyje-es.properties')
Could not execute operation due to backend exception
Type ':help' or ':h' for help.
Display stack trace? [yN]n
```

​	提示由于后端问题无法执行，解决方法：

```bash
# 退出gremlin
gremlin> :exit
# 打开服务器配置
cd ../conf/gremlin-server/
vi gremlin-server.yaml
```

![image-20210912233126742](C:\Users\11251\AppData\Roaming\Typora\typora-user-images\image-20210912233126742.png)

​	从配置文件中可看出里面只包含了一个`janusgraph-inmemory.properties`的配置。回去查看可用的配置文件：

![image-20210912233326356](C:\Users\11251\AppData\Roaming\Typora\typora-user-images\image-20210912233326356.png)

​	可以找到`janusgraph-inmemory.properties`这个配置文件，那就直接用这个即可。

![image-20210912233519375](C:\Users\11251\AppData\Roaming\Typora\typora-user-images\image-20210912233519375.png)

​	可以看到配置文件成功加载了。

​	事实上，教程里是有提那么一嘴的：

![image-20210912234904997](C:\Users\11251\AppData\Roaming\Typora\typora-user-images\image-20210912234904997.png)

2. 在加载图对象时，若直接使用：

```bash
gremlin> GraphOfTheGodsFactory.load(graph)
```

​	会出现报错：

```bash
The indexing backend with name "search" is not defined. Specify an existing indexing backend or use GraphOfTheGodsFactory.loadWithoutMixedIndex(graph,true) to load without the use of an indexing backend.
Type ':help' or ':h' for help.
Display stack trace? [yN]n
# 使用loadWithoutMixedIndex即可。
gremlin> GraphOfTheGodsFactory.loadWithoutMixedIndex(graph, true)
==>null
```



---------

### TODO

​	进一步学习[Gremlin使用方法](https://docs.janusgraph.org/getting-started/gremlin/)

![image-20210912233819724](C:\Users\11251\AppData\Roaming\Typora\typora-user-images\image-20210912233819724.png)

