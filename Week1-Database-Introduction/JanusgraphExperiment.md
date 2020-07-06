# Janusgraph 实验

[JanusGraph](https://docs.janusgraph.org/)是一个可扩展的图数据库，用于存储和查询分布在多机集群中的包含数千亿顶点和边的图。

## Step1 安装

1. 安装分为两种：Docker和本地安装，使用Docker需要先安装Docker环境，本地安装需要Java环境，可以参考网上的方法进行Docker和Java的环境搭建。

2. JanusGraph的具体安装流程参考[Installation](https://docs.janusgraph.org/getting-started/installation/)

   要求掌握其中一种安装方式，推荐Docker省时省力。

## Step2 使用

Docker推荐使用：

```
docker run --rm --link janusgraph-default:janusgraph -e GREMLIN_REMOTE_HOSTS=janusgraph \
    -it janusgraph/janusgraph:latest ./bin/gremlin.sh
```

来启动环境。

1. 按照[官方示例](https://docs.janusgraph.org/getting-started/basic-usage/) ,跑一遍流程，并做好相关的实验记录

2. 自己设计一个查询用例，查询并输出，记录结果

## Step3 实验文档撰写

1. 使用Markdown编写实验文档
2. 记录实验过程
3. 记录实验中遇到的问题

