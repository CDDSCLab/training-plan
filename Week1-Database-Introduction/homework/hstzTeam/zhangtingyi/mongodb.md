### 一、MongoDB 官方入门课程 M001前两章的学习笔记
1. mongodb是一个NoSQL文档数据库。

2. NoSQL：任何不以关系型数据库的方法存储数据的数据库；这意味着你以有组织的方式存储数据，但不是按行和列存储。

3. 文档：一种将数据组织和存储为一组<字段，值>对的方法。

4. 集合：有组织地存储有着通用字段的文档。

5. Atlas：云数据库，为以mongodb为核心的各种应用程序而构建的完全托管的数据库。

6. 创建和部署一个atlas集群。

7. BSON：二进制的json；mongodb在内部和网络以BSON格式存储数据；但是，你可以在json中表示的任何内容，都可以本地存储在MongoDB中，并且可以在json中轻松搜索；即以BSON的格式存储，以JSON的格式查看。

8. 导入和导出数据：
	* mongoimport：`mongoimport --uri "<Atlas Cluster URI>"  --drop=<filename>.json`
	    从json文件中导入数据
	
	* mongoexport：`mongoexport --uri "<Atlas Cluster URI>"  --collection=<collection name> --out=<filename>.json`
	    以JSON格式导出数据
	
	* mongorestore：`mongorestore --uri "<Atlas Cluster URI>" --drop dump`
	    从BSON文件导入数据
	
	* mongodump: `mongodump --uri "<Atlas Cluster URI>"` 
	    以BSON格式导出数据
	
9.  查询数据：
	* 连接admin数据库：`mongo "mongodb+srv://m001-student:m001-mongodb-basics@sandbox.z81nm.mongodb.net/admin"`
	* 显示集群中的数据库列表：`show dbs`
	* 选择想使用的数据库：`use <数据库名>`
	* 查看数据库中的集合：`show collections`
	* 查询指定集合：`db.<collection name>.find(<query>)`
	* `it`命令：“iterate”，it允许我们遍历find查询命令返回的对象。
	* 统计符合条件的文档的数量：`db.<collection name>.find(<query>).count()`
	* 使结果界面更美观：`db.<collection name>.find(<query>).pretty()`
	* find的空查询指令：`db.<collection>.find({})`：展示集合的前20个文档（随机的顺序）
	* 实验记录：
		```sql
		1. mongo "mongodb+srv://m001-student:m001-mongodb-basics@sandbox.z81nm.mongodb.net/admin"
		2. show dbs
		3. use sample_training
		4. show collections
		5. db.zips.find({"state": "NY"})
		6. db.zips.find({"state": "NY"}).count()
		7. db.zips.find({"state": "NY", "city": "ALBANY"})
		8. db.zips.find({"state": "NY", "city": "ALBANY"}).pretty()
		```

### 二、安装MongoDB数据库
#### 官网安装教程，在ubuntu20.04操作系统上进行
1. 安装mongodb的公钥：`wget -qO - https://www.mongodb.org/static/pgp/server-5.0.asc | sudo apt-key add -`
2. 创建一个list文件：`echo "deb http://repo.mongodb.org/apt/debian buster/mongodb-org/5.0 main" | sudo tee /etc/apt/sources.list.d/mongodb-org-5.0.list`
3. 重新加载系统的包数据库：`sudo apt-get update`
4. 安装MongoDB的最新版本：`sudo apt-get install -y mongodb-org`
5. 启动mongod：`sudo service mongod start`

#### docker安装MongoDB容器（推荐）
1. 创建mongodb容器：`docker run -itd --name ztyMongoDB -p 57017:27017 mongo --auth`
	参数：--auth：需要密码才能访问容器服务。
2. 进入容器：`docker exec -it ztyMongoDB mongo admin`
3. 创建一个admin用户和密码：`db.createUser({ user:'admin',pwd:'123456',roles:[ { role:'userAdminAnyDatabase', db: 'admin'},"readWriteAnyDatabase"]});`
4. 创建一个root用户：`db.createUser({user:"root",pwd:"root",roles:["root"]})`
5. 使用上面创建的用户信息进行连接:`db.auth('admin', '123456')`

### 三、MongoDB的基本概念
1. 和MySQL进行对比：
![](./images/01.png)

2. MySQL中的表格和MongoDB中的集合对比：
![](./images/02.png)

3. MySQL的数据库服务端和客户端
![](./images/03.png)

### 四、MongoDB的基本使用
#### 1、数据库
##### （1）创建数据库
`use DATABASE_NAME`：如果数据库不存在，则创建数据库，否则切换到指定数据库。但是刚创建的数据库并不在数据库的列表中，要显示它，需要向该数据库插入一些数据。
##### （2）查看数据库
`show dbs`：查看所有的数据库
`db`：查看当前正在使用的数据库
##### （3）删除数据库
`db.dropDatabase()`：删除当前数据库，你可以使用`db`命令查看当前数据库名。
#### 2、集合
##### （1）创建集合
`db.createCollection(name, options)`：name为要创建的集合名称；options为可选参数, 指定有关内存大小及索引的选项。
当你插入一些文档时，如果指定的集合不存在，MongoDB会自动根据你指定的集合名创建集合，具体请看实验记录。
##### （2）查看集合
`show collections`: 查看当前数据库有哪些集合
##### （3）删除集合
`db.collection.drop()`：如果成功删除选定集合，则 drop() 方法返回 true，否则返回 false。
#### 3、文档
##### （1）插入文档
`db.COLLECTION_NAME.insert(document)`:
`db.collection.insertOne(document)`:用于向集合插入一个新文档
`db.collection.insertMany(document)`:用于向集合插入一个或多个文档
##### （2）更新文档
`db.collection.update(<query>,<update>,)`:更新已存在的文档,query是update的查询条件；update为update的对象和一些更新的操作符（如$,$inc...）等；
`db.collection.save(<document>)`:通过传入的文档来替换已有文档，新文档的_id和旧文档的_id有相同的就是更新，不存在相同的就插入。
##### （3）删除文档
`db.collection.remove(<query>)`：query指定删除的文档的条件。
##### （4）查询文档
`db.collection.find(query, projection)`：query(可选)使用查询操作符指定查询条件; projection(可选)使用投影操作符指定返回的键,查询时返回文档中所有键值， 只需省略该参数即可（默认省略）。
`db.col.find().pretty()`：以易读的方式来读取数据
#### 4、实验记录
```sql
* 创建数据库
1. use student
2. db #显示当前数据库为student
3. show dbs #然而数据库列表中没有student
4. db.student.insert({"name":"zhangsan"}) #向student数据库插入一个文档
5. show dbs #数据库列表中出现了student

* 删除数据库
1. db #查看当前正在使用的数据库，显示为student数据库
2. db.dropDatabase() #删除数据库student，显示权限不够
3. db.auth('root','root') #换成root用户
4. use student
5. db.dropDatabase() #成功删除
6. show dbs #列表中无student

* 创建集合
1. use test #选择test数据库
2. db.createCollection('student') #创建student集合
3. show collections #有student数据集
4. db.test.insert({"name" : "zty"}) #自动在test数据库中创建test集合
5. show collections #有test集合

* 删除集合
1. show collections #显示student和test集合
2. db.test.drop() #删除test集合，返回true
3. show collections #只显示了student集合

* 插入文档
1. db.student.insert({'name':'zhangsan'}) #向student集合插入一个文档
2. db.student.find() #查看已插入的文档

* 更新文档
1. db.student.insert({'name':'lisi','age':14}) #插入新的文档，增加字段age
2. db.student.find() #显示集合中所有文档，发现之前的zhangsan没有age
3. db.student.update({'name':'zhangsan'},{$set:{'age':15}}) #找到zhangsan所在文档，并更新age字段的值
4. db.student.find() #都有age

* 删除文档
1. db.student.find()  #查看集合所有文档
2. db.student.remove({'name':'zhangsan'}) #删除zhangsan的文档
3. db.student.find()  #查看集合所有文档，发现没有zhangsan的文档

* 查询文档
1. db.student.find().pretty() #查询了集合student中的所有文档
2. db.student.find({'name':'lisi'}) #一个查询条件
3. db.student.find({'name':'lisi','age':14}) #两个查询条件的AND操作
4. db.student.find({$or:[{'name':'lisi'},{'age':15}]}) #两个查询条件的OR操作

```

### 五、遇到的问题
* 使用MongoDB官网安装教程在Ubuntu/Debian容器中安装时，在进行到最后一步时，即执行命令`sudo service mongod start`时，返回：`mongod: unrecognized service`。
	* 解决方法：启动`/usr/bin/mongod --config /etc/mongod.conf`，停止`/usr/bin/mongod --config /etc/mongod.conf --shutdown`
### 六、TODO
* 学习正则.
