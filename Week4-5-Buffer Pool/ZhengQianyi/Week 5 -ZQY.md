## Week 5

#### 问题：

1. ScaleTest无法通过。

   解决：应该是GetValue中没有Unpin。正在修改。

#### 疑问：

1. SplitTest 测试中，叶节点大小为2，内部节点大小为3，依次插入{1，2，3，4，5}到空树后。我生成的B+树为：

![image-20211018123256889](https://raw.githubusercontent.com/zqyi/img/master/20211018123256.png)

但是，在测试时，在得到第一个叶节点，然后再在叶节点上进行遍历时，会GetNext四次，造成错误，无法通过测试。

![image-20211018123324829](https://raw.githubusercontent.com/zqyi/img/master/20211018123324.png)

所以对此处的测试用例存在疑问。

