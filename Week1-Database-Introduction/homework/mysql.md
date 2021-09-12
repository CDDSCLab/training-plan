## 一、安装MySQL

### 使用Docker安装MySQL

拉取官方镜像：

`docker pull mysql:latest`

运行容器：

`docker run -itd --name hl_test -p 3306:3306 -e MYSQL_ROOT_PASSWORD=houlei mysql`

## 二、简单使用

运行docker中的MySQL容器：

`docker exec -it hl_mysql /bin/bash`

进入mysql：

`# mysql -u root  -p`

创建数据库：

`create databases hl_test;`

`use hl_test;`

建立一个student表和student_info表：

`mysql> create table student(
    -> id int primary key ,
    -> name varchar(20) ,
    -> sex varchar(8) default 'male' check(sex in ('male','female'))
    -> )；`

`mysql> create table student_info(
    -> id int primary key,
    -> chinese_score int unsigned not null,
    -> english_score int unsigned not null,
    -> math_score int unsigned not null,
    -> gym_score int unsigned not null
    -> );`

`alter table student_info add foreign key(id) references student(id);`

向表中插入信息：

`insert into student(id,name,sex) values(2001,'zhangsan','male');`

`insert into student(id,name,sex) values(2002,'lisi','female');`

`insert into student(id,name,sex) values(2003,'wangwu','female');`

`select * from student;`
`+------+----------+--------+`
`| id   | name     | sex    |`
`+------+----------+--------+`
`| 2001 | zhangsan | male   |`
`| 2002 | lisi     | female |`
`| 2003 | wangwu   | female |`
`+------+----------+--------+`

`insert into student_info(id,chinese_score,english_score,math_score,gym_score) values(2001,99,98,97,96);`

`insert into student_info(id,chinese_score,english_score,math_score,gym_score) values(2002,89,88,87,86);`

`insert into student_info(id,chinese_score,english_score,math_score,gym_score) values(2003,79,78,77,76);`

`select * from student_info;`
`+------+---------------+---------------+------------+-----------+`
`| id   | chinese_score | english_score | math_score | gym_score |`
`+------+---------------+---------------+------------+-----------+`
`| 2001 |            99 |            98 |         97 |        96 |`
`| 2002 |            89 |            88 |         87 |        86 |`
`| 2003 |            79 |            78 |         77 |        76 |`
`+------+---------------+---------------+------------+-----------+`

通过join查看id为2001的学生的所有分数信息：

 `select *  from student join student_info using(id) where student.id = 2001;`

`+------+----------+------+---------------+---------------+------------+-----------+`
`| id   | name     | sex  | chinese_score | english_score | math_score | gym_score |`
`+------+----------+------+---------------+---------------+------------+-----------+`
`| 2001 | zhangsan | male |            99 |            98 |         97 |        96 |`
`+------+----------+------+---------------+---------------+------------+-----------+`

## TODO

1.回忆一下基础知识

2.有机会多用用