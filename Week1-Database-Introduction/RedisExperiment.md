# Redis实验

[Redis](https://redis.io/)是一个开源的使用ANSI C语言编写、遵守BSD协议、支持网络、可基于内存亦可持久化的日志型、Key-Value数据库，并提供多种语言的API。它通常被称为数据结构服务器，因为值（value）可以是 字符串(String), 哈希(Hash), 列表(list), 集合(sets) 和 有序集合(sorted sets)等类型。

## Step1 安装

### Windows

在[Github](https://github.com/tporadowski/redis/releases)上面有别人编译打包好的windows版本的redis，直接可以下载安装。

### Linux

Linux安装Redis需要从源码开始编译。

1. 从[redis官网链接](http://redis.io/download)下载稳定版本。
2. 用下面的命令解压编译

```shell
tar xzf redis-xx.tar.gz
cd redis-xx
make
```
3. 运行redis

编译得到的可执行程序在src目录下面，可以通过下面的目录进行启动。

```shell
cd src 
# 使用默认配置启动redis服务器
./redis-server

# 使用自定义配置启动服务器
./redis-server ../redis.conf

# 启动客户端，连接到服务器
./redis-cli
```

### Ubuntu

在Ubuntu系统中可以使用下面的命令安装测试redis：

```shell
# 安装
sudo apt-get update
sudo apt-get install redis-server

# 启动服务器
redis-server

# 启动客户端
redis-cli

# 此时会进入redis命令行界面
redis 127.0.0.1:6379>
```

## Step2 使用

Redis有5种数据类型：字符串、哈希、列表、集合、有序集合，每种数据类型都有对应的set/get命令。

按照上面的方法安装启动redis之后，可以按照[官方文档](https://redis.io/topics/data-types-intro)的说明，熟悉上面的5种数据结构，进行基础的增/删/改/查操作。

## Step3 实验文档撰写

1. 使用Markdown编写实验文档
2. 记录实验过程
3. 记录实验中遇到的问题
