# 实验一 MySQL的安装和使用

## 一、使用docker安装mysql容器
#### 1、获取mysql镜像
    $ docker pull mysql:5.7
#### 2、建MySQL容器
    $ docker run --name sww_mysql -d -it -p 5432:5432 -e MYSQL_ROOT_PASSWORD=root mysql:5.7
#### 3、进入容器
    $ docker exec -it sww_mysql bash
#### 4、进入MySQL
    $ mysql -h localhost -u root -p
    输入密码root

## 二、使用MySQL
#### 1、创建并使用数据库
    > create database mysqltest;      
    > use mysqltest;
#### 2、创建表并查看

    > create table student(  
       id  int(11),  
       name  varchar(20),   
       age int(11)  
      );  
    > show tables;  
    +----------------------+  
    | Tables\_in_mysqltest |  
    +----------------------+  
    | student              |  
    +----------------------+  
    > desc student;   
    +-------+-------------+------+-----+---------+-------+  
    | Field | Type        | Null | Key | Default | Extra |  
    +-------+-------------+------+-----+---------+-------+  
    | id    | int(11)     | YES  |     | NULL    |       |  
    | name  | varchar(20) | YES  |     | NULL    |       |  
    | age   | int(11)     | YES  |     | NULL    |       |  
    +-------+-------------+------+-----+---------+-------+  
#### 3、插入数据
    > INSERT INTO student ( id, name, age ) VALUES ( 2, "zhangsan", 20 );
#### 4、删除数据
    > DELETE FROM student WHERE id=1;
#### 5、查询数据
    > select * from student;
#### 6、更新数据
    > UPDATE student SET age=18 WHERE id=2;
#### 7、删除表
    > DROP TABLE student;

## 三、建立一个student表和student_info表，并查询一个学生对应的student_info信息
#### 1、创建学生表 （学号，姓名，性别，年龄，电话）
    > CREATE TABLE STUDENT(  
       SID     BIGINT  UNSIGNED PRIMARY KEY  AUTO_INCREMENT,  
       NAME    VARCHAR(40),  
       SEX     VARCHAR(10)  DEFAULT 'male' CHECK(SEX IN ('male','female')),  
       AGE     INT  UNSIGNED,  
       TEL     BIGINT  UNSIGNED  
      )engine=InnoDB default charset=utf8 auto_increment=1;  
    > desc STUDENT;   
    +-------+---------------------+------+-----+---------+----------------+  
    | Field | Type                | Null | Key | Default | Extra          |  
    +-------+---------------------+------+-----+---------+----------------+  
    | SID   | bigint(20) unsigned | NO   | PRI | NULL    | auto_increment |  
    | NAME  | varchar(40)         | YES  |     | NULL    |                |  
    | SEX   | varchar(10)         | YES  |     | male    |                |  
    | AGE   | int(10) unsigned    | YES  |     | NULL    |                |  
    | TEL   | bigint(20) unsigned | YES  |     | NULL    |                |  
    +-------+---------------------+------+-----+---------+----------------+  
#### 2、创建学生信息表（学号，英语、数学、语文、综合成绩）
    > CREATE TABLE STUDENT_INFO(  
       SID BIGINT UNSIGNED PRIMARY KEY  AUTO_INCREMENT,  
       ENGLISH_SCORE INT UNSIGNED NOT NULL,  
       MATH_SCORE INT UNSIGNED NOT NULL,  
       CHINESE_SCORE INT UNSIGNED NOT NULL,  
       COMPREHENSIVE_SCORE INT UNSIGNED NOT NULL  
       )engine=InnoDB default charset=utf8 auto_increment=1;  
    > desc STUDENT_INFO;  
    +---------------------+---------------------+------+-----+---------+----------------+  
    | Field               | Type                | Null | Key | Default | Extra          |  
    +---------------------+---------------------+------+-----+---------+----------------+   
    | SID                 | bigint(20) unsigned | NO   | PRI | NULL    | auto_increment |  
    | ENGLISH_SCORE       | int(10) unsigned    | NO   |     | NULL    |                |  
    | MATH_SCORE          | int(10) unsigned    | NO   |     | NULL    |                |  
    | CHINESE_SCORE       | int(10) unsigned    | NO   |     | NULL    |                |  
    | COMPREHENSIVE_SCORE | int(10) unsigned    | NO   |     | NULL    |                |  
    +---------------------+---------------------+------+-----+---------+----------------+  
#### 3、添加学生信息表对学生表学号的外键
    > ALTER TABLE STUDENT_INFO  
      ADD CONSTRAINT INFO_STUDENT FOREIGN KEY(SID)  
      REFERENCES STUDENT(SID) ON DELETE  CASCADE ON UPDATE CASCADE;  
#### 4、学生表插入数据
    > INSERT INTO STUDENT(SID,NAME,SEX,AGE,TEL) VALUES (202121081001,'shiweiwei','female',23,13086652910);  
    > INSERT INTO STUDENT(SID,NAME,SEX,AGE,TEL) VALUES (202121081002,'huangqizhi','male',22,13086651123);  
    > INSERT INTO STUDENT(SID,NAME,SEX,AGE,TEL) VALUES (202121081003,'zhangtingyi','male',21,13256384412);  
    > INSERT INTO STUDENT(SID,NAME,SEX,AGE,TEL) VALUES (202121081004,'tangyiheng','male',22,15623568854);   
    > SELECT * FROM STUDENT;  
    +--------------+-------------+--------+------+-------------+  
    | SID          | NAME        | SEX    | AGE  | TEL         |  
    +--------------+-------------+--------+------+-------------+  
    | 202121081001 | shiweiwei   | female |   23 | 13086652910 |  
    | 202121081002 | huangqizhi  | male   |   22 | 13086651123 |  
    | 202121081003 | zhangtingyi | male   |   21 | 13256384412 |  
    | 202121081004 | tangyiheng  | male   |   22 | 15623568854 |  
    +--------------+-------------+--------+------+-------------+  
#### 5、学生信息表插入数据
    > INSERT INTO STUDENT_INFO (SID,ENGLISH_SCORE,MATH_SCORE,CHINESE_SCORE,COMPREHENSIVE_SCORE) VALUES    (202121081001,130,137,114,210);  
    > INSERT INTO STUDENT_INFO (SID,ENGLISH_SCORE,MATH_SCORE,CHINESE_SCORE,COMPREHENSIVE_SCORE) VALUES(202121081002,135,133,120,230);  
    > INSERT INTO STUDENT_INFO (SID,ENGLISH_SCORE,MATH_SCORE,CHINESE_SCORE,COMPREHENSIVE_SCORE) VALUES(202121081003,124,140,120,234);  
    > INSERT INTO STUDENT_INFO (SID,ENGLISH_SCORE,MATH_SCORE,CHINESE_SCORE,COMPREHENSIVE_SCORE) VALUES(202121081004,138,139,122,250);  
    > SELECT * FROM STUDENT_INFO;  
    +--------------+---------------+------------+---------------+---------------------+  
    | SID          | ENGLISH_SCORE | MATH_SCORE | CHINESE_SCORE | COMPREHENSIVE_SCORE |  
    +--------------+---------------+------------+---------------+---------------------+  
    | 202121081001 |           130 |        137 |           114 |                 210 |  
    | 202121081002 |           135 |        133 |           120 |                 230 |  
    | 202121081003 |           124 |        140 |           120 |                 234 |  
    | 202121081004 |           138 |        139 |           122 |                 250 |  
    +--------------+---------------+------------+---------------+---------------------+  
#### 6、连接两个表
    > SELECT * FROM STUDENT JOIN STUDENT_INFO USING (SID);  
    +--------------+-------------+--------+------+-------------+---------------+------------+---------------+---------------------+  
    | SID          | NAME        | SEX    | AGE  | TEL         | ENGLISH_SCORE | MATH_SCORE | CHINESE_SCORE | COMPREHENSIVE_SCORE |  
    +--------------+-------------+--------+------+-------------+---------------+------------+---------------+---------------------+  
    | 202121081001 | shiweiwei   | female |   23 | 13086652910 |           130 |        137 |           114 |                 210 |  
    | 202121081002 | huangqizhi  | male   |   22 | 13086651123 |           135 |        133 |           120 |                 230 |  
    | 202121081003 | zhangtingyi | male   |   21 | 13256384412 |           124 |        140 |           120 |                 234 |  
    | 202121081004 | tangyiheng  | male   |   22 | 15623568854 |           138 |        139 |           122 |                 250 |  
    +--------------+-------------+--------+------+-------------+---------------+------------+---------------+---------------------+  
## 四、MySQL架构
MySQL 在整体架构上分为 Server 层和存储引擎层。  
Server 层，包括连接器、查询缓存、分析器、优化器、执行器等，存储过程、触发器、视图和内置函数都在这层实现。  
存储引擎层负责数据的存储和提取，如 InnoDB、MyISAM、Memory 等引擎。  
在客户端连接到 Server 层后，Server 会调用数据引擎提供的接口，进行数据的变更。  
![MySQL整体架构图](images/01.png)
