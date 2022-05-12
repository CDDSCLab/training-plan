## DSCLab 新生培养计划

培养方向：分布式数据库研发

培养目标：

1. 具备一定的软件工程能力
2. 掌握基础数据结构和数据库关键技术
3. 掌握分布式原理和应用开发

技能培养：

**C++ / JAVA / GO 任选一门主力学习, Python 选修**

1. Git、Stencil、Visio 、Docker 等工具的使用
2. 软件工程能力（代码规范、协作开发、UML 的使用、代码测试、文档撰写）
3. Research、Presentation 能力

**时间安排表：**

- 第一学期

| Week                               | Point                          | Detail                                                                                                                                                                                                                                                                                         | Project                                                                                                                   | Further Reading                                                                                                                                                                                                                                        |
|:----------------------------------:|:------------------------------:| ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [1](./Week1-Database-Introduction) | 数据库分类及内部组件功能                   | 数据库与文件系统的区别，数据库产生的原因；学习关系型、文档型、KV 型、图数据库等不同类别等数据库，了解其中架构及功能模块，理解不同数据库的应用场景。<br /> 学习使用git进行协作开发                                                                                                                                                                                                | 1. 安装相关类别的数据库并简单使用（可以使用 [Docker](https://www.docker.com/) 等工具） <br/>2. 以3人小组为单位整理相关文档，并进行分享。（使用markdown格式撰写文档，并上传至github） |                                                                                                                                                                                                                                                        |
| 2                                  | 数据结构 1                         | 熟悉开发流程与规范<br />[Bloom Filter](https://llimllib.github.io/bloomfilter-tutorial/) <br/> [Cuckoo Filter](https://en.wikipedia.org/wiki/Cuckoo_filter) <br/> [Consistent Hash](https://en.wikipedia.org/wiki/Consistent_hashing) <br/> [Chord](https://en.wikipedia.org/wiki/Chord_(peer-to-peer)) | 实现两种过滤器：要求具有单元测试，与性能测试。<br />使用 UML 建模完成功能和测试（代码符合统一规范，具有完备的测试用例）最后形成文档。（后续的 Project 同样的要求）                               |                                                                                                                                                                                                                                                        |
| 3                                  | 关系型数据库 1                       | 学习关系型数据库的基本概念：关系模型，SQL语句，索引，buffer pool，事务，存储等，学习各个模块在关系型数据库中所处位置                                                                                                                                                                                                                              | 以3人小组为单位，对关系型数据库各个部分进行分析，并撰写相关文档 <br />                                                                                   |                                                                                                                                                                                                                                                        |
| 4-5                                | 关系型数据库 1                       | 以个人为单位，实现一个buffer pool，并测试其性能及内存占用。并撰写相关文档                                                                                                                                                                                                                                                     | [项目地址](https://15445.courses.cs.cmu.edu/fall2020/project1/)                                                               |                                                                                                                                                                                                                                                        |
| 6-7                                | 数据结构 2                         | [B-Tree](https://en.wikipedia.org/wiki/B-tree)<br/>[B+ Tree](https://en.wikipedia.org/wiki/B%2B_tree)<br/>                                                                                                                                                                                     | 在buffer pool的基础上，实现一个B+树，需包含单元测试及测试性能。[项目地址](https://15445.courses.cs.cmu.edu/fall2020/project2/)                         | [Mass-Tree](https://pdos.csail.mit.edu/papers/masstree:eurosys12.pdf) <br/>[Bw-Tree](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/bw-tree-icde2013-final.pdf) <br/> [Bz-Tree](http://www.vldb.org/pvldb/vol11/p553-arulraj.pdf) |
| 8-9                                | 数据结构 3                         | [Skip List](https://en.wikipedia.org/wiki/Skip_list)、[LSM Tree](https://en.wikipedia.org/wiki/Log-structured_merge-tree) 分析其在LevelDB中的实现方式<br /> 分析B+树与LSM Tree的特点及应用场景。                                                                                                                       | 继续完成B+树项目<br />对leveldb中跳表与lsm tree的实现方式进行分析，以3人为单位撰写leveldb代码分析文档                                                        |                                                                                                                                                                                                                                                        |
| 10-12                              | SQL Engine & Parser & Executor | 了解 SQL Engine 组件及功能，学习 SQL 解析流程及抽象语法树。学习执行算子，如 Join, Agg, Scan 的不同实现。了解基本的查询优化方法，了解 Volcano 及 Vectorized 执行器架构                                                                                                                                                                                 | 撰写相关文档<br />实现数据库的执行器。[项目地址](https://15445.courses.cs.cmu.edu/fall2020/project3/)                                         |                                                                                                                                                                                                                                                        |
| 13-15                              | 数据库事务                          | 理解事务的概念及产生原因、理解ACID特性<br/>并发控制（乐观、悲观）<br/>数据持久化和故障恢复（日志、检查点、快照）                                                                                                                                                                                                                                | 撰写相关文档<br />基于锁实现数据库的并发控制，要求保证事务的ACID特性 [项目地址](https://15445.courses.cs.cmu.edu/fall2020/project4/)                       | [Percolator](http://notes.stephenholiday.com/Percolator.pdf) <br/>                                                                                                                                                                                     |
| 16                                 | 分布式基础 议                        | 分布式相关概念、时钟 <br/> CAP、BASE <br/> 拜占庭问题 <br /> [Zab](https://marcoserafini.github.io/papers/zab.pdf) <br/> [Gossip](https://flopezluis.github.io/gossip-simulator/) <br/>                                                                                                                        | 实现单机的kv server [Project1 StandaloneKV](https://github.com/tidb-incubator/tinykv/blob/course/doc/project1-StandaloneKV.md) | [LCN](https://github.com/codingapi/tx-lcn) <br/> [Seata](http://seata.io/en-us/) <br/> Quorum                                                                                                                                                          |
| 17-20                              | 分布式基础 议                        | [Paxos](https://en.wikipedia.org/wiki/Paxos_(computer_science)) <br/> [Raft](https://raft.github.io/) <br/> RPC                                                                                                                                                                                |                                                                                                                           | Multi-Raft <br /> Parallel-Raft                                                                                                                                                                                                                        |

说明：整个学习流程的作业和文档需要在实验室 Github 仓库里面提交，每个 Project 会事先建立好框架，每个同学按照框架进行代码和文档的撰写。并且在代码过程中抽象公共代码库方便后续同学的使用，且需做好 todo-list，方便功能的完善。

- 第二学期
  
  承接上学期理论学习，下学期的新生培养以动手实践为主。项目1选取TiDB Talent Plan中TinyKV项目，目标是实现一个可扩展、高可用、支持分布式事务的KV数据库，使用golang进行开发[项目地址](https://github.com/tidb-incubator/tinykv)。项目2选取TiDB Talent Plan中的TinySQL项目。

| Week  | Point     | Detail                                                                 | Project                                                                                                                           |
|:-----:| --------- |:----------------------------------------------------------------------:|:---------------------------------------------------------------------------------------------------------------------------------:|
| 1-4   | raft基础    | 基于raft实现高可用的key-value server                                           | [Project2 RaftKV](https://github.com/tidb-incubator/tinykv/blob/course/doc/project2-RaftKV.md)                                    |
| 6-9   | raft进阶    | 基于multi-raft实现可扩展的key-value server                                     | [Project3 MultiRaftKV](https://github.com/tidb-incubator/tinykv/blob/course/doc/project3-MultiRaftKV.md)                          |
| 10-12 | 分布式事务     | 分布式事务(2PC / 3PC) <br />基于Percolator实现分布式事务，分析Percolaor的优缺点             | [Project 4 Transactions](https://github.com/tidb-incubator/tinykv/blob/course/doc/project4-Transaction.md)                        |
| 13    | 数据挖掘      | 数据预处理(滤波、插值、降维) <br />关联分析 <br />图挖掘，序列模式挖掘                            | 1. 整理相关算法说明文档<br />2. 使用现有工具库分析图数据中的重要节点、社团关系等，并对结果进行展示 <br /> [项目地址](./SpringTerm-Week13-DataMining)                             |
| 14    | 机器学习&深度学习 | 分类算法（KNN、贝叶斯、SVM、决策树）<br />聚类算法（kmeans、DBSCAN）<br />常用模型(CNN、RNN、LSTM) | 1. 整理相关算法说明文档 <br /> 2. 使用机器学习和深度学习方法实现手写数字识别 <br />3. 空气质量数据集进行预处理，使用深度学习方法，实现空气质量预测<br />[项目地址](./SpringTerm-Week14-DeepLearning) |
| 15    | 深度学习      | Attention机制、Transformer、GAN<br />自然语言处理基础：分词、去除停用词、命名实体识别、预训练模型        | 1. 使用NLP工具实现命名实体识别 <br />2. 搭建模型，实现情感分析<br />[项目地址](SpringTerm-Week15-NLP)                                                        |

## 如何提交作业

1. 第一次提交作业的时候请先 `fork` 当前项目到自己的仓库

2. 在自己的仓库里面进行作业的更新
   
   以第一次作业为例，比如你 `fork` 之后你的代码地址为 `git@github.com:ehds/training-plan.git`
   
   1. 克隆仓库到本地：
      
      `git clone git@github.com:ehds/training-plan.git`
   
   2. 进入当前实验文件夹`Week1-Databse-Introduction`
      
      `cd Week1-Databse-Introduction`
   
   3. 新建一个自己的文件夹，名字+实验名称，参考 Example
      
      `cp -r Example DongShengHe-Week1`
      
      完善实验文档 Markdown 文件
   
   4. add 作业内容
      
      `git add .`
   
   5. commit 修改
      
      `git commit -m "message"` （message使用英文填写你所做的修改）如 `git commit -m “complete week1 experiment`
   
   6. push 分支
      
      `git push origin master`

3. 提交 PR
   
   在 Gihub 仓库界面新建一个 pull request 到实验室仓库

4. 审核与修改
   
   待审核人审核后，根据修改意见进行修改，待通过后即可

## 如何提问

大家遇到问题首先合理利用搜索引擎，网络上可能已经存在比较完美的解决方案

在向同学和老师提问的时候，请先描述问题出现的背景和出现问题的状况，这样更加有利于问题的解决。

请参考[提问的智慧。](https://github.com/ryanhanwu/How-To-Ask-Questions-The-Smart-Way/blob/master/README-zh_CN.md)
