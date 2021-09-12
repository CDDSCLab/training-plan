## 一、安装MongoDB

拉取官方最新版本镜像：

`docker pull mongo:latest`

运行容器：

`docker run -it --name hl_mongo mongo --auth`

`docker exec -it hl_mongo mongo admin`

#服务端和客户端

创建用户：

`db.createUser({ user:'admin',pwd:'123456',roles:[ { role:'userAdminAnyDatabase', db: 'admin'},"readWriteAnyDatabase"]});`

连接：

`db.auth('admin','123456')`

## 二、MongoDB M001课程

MongoDB是文档型NoSQL数据库  



## 三、参考资料

菜鸟教程：https://www.runoob.com/docker/docker-install-mongodb.html

