# MongoDB 学习-- Linux 平台安装MongoDB

## 步骤一：下载安装 MongoDB 版本

MongoDB 提供了 linux 各发行版本 64 位的安装包，你可以在官网下载安装包。

[Ubuntu 下安装 MongoDB 教程](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/)

## 步骤二：创建数据库目录

MongoDB 的数据存储在 data 目录的 db 目录下，但是这个目录在安装过程不会自动创建，所以你需要手动创建 data 目录，并在 data 目录中创建 db 目录。

以下实例中我们将data目录创建于根目录下(/)。

注意：/data/db 是 MongoDB 默认的启动的数据库路径( --dbpath ) 。

```
mkdir -p /data/db
```

## 步骤三：命令行中运行 MongoDB 服务

你可以再命令行中执行mongo安装目录中的bin目录执行mongod命令来启动mongdb服务。

> 注意：如果你的数据库目录不是/data/db，可以通过 --dbpath 来指定。

```
$ ./mongod
2015-09-25T16:39:50.549+0800 I JOURNAL  [initandlisten] journal dir=/data/db/journal
2015-09-25T16:39:50.550+0800 I JOURNAL  [initandlisten] recover : no journal files present, no recovery needed
2015-09-25T16:39:50.869+0800 I JOURNAL  [initandlisten] preallocateIsFaster=true 3.16
2015-09-25T16:39:51.206+0800 I JOURNAL  [initandlisten] preallocateIsFaster=true 3.52
2015-09-25T16:39:52.775+0800 I JOURNAL  [initandlisten] preallocateIsFaster=true 7.7
```

## 步骤四：MongoDB 后台管理 Shell

如果你需要进入MongoDB后台管理，你需要先打开mongodb装目录的下的bin目录，然后执行mongo命令文件。

MongoDB Shell是MongoDB自带的交互式Javascript shell,用来对MongoDB进行操作和管理的交互式环境。

当你进入mongoDB后台后，它默认会链接到 test 文档（数据库）：

```
$ cd /usr/local/mongodb/bin
$ ./mongo
MongoDB shell version: 3.0.6
connecting to: test
Welcome to the MongoDB shell.
……
```

由于它是一个JavaScript shell，您可以运行一些简单的算术运算:

```
> 2+2
4
> 3+6
9
```

现在让我们插入一些简单的数据，并对插入的数据进行检索：

```
> db.runoob.insert({x:10})
WriteResult({ "nInserted" : 1 })
> db.runoob.find()
{ "_id" : ObjectId("5604ff74a274a611b0c990aa"), "x" : 10 }
>
```

第一个命令将数字 10 插入到 runoob 集合的 x 字段中。

---

# 参考学习资料

1.[强烈推荐: MongoDB 官方入门课程 M001](https://university.mongodb.com/courses/M001/about)

2.[macOs | windows | Linux安装教程](https://docs.mongodb.com/manual/administration/install-community/)