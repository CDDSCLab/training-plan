# skiplist 实验

1. 编码规范请参考：

   [google-style-en](https://google.github.io/styleguide/cppguide.html)
   
   [google-style-cn](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/)

2. 代码格式：google-style
   在提交代码前请使用 clang-format对代码进行格式化
   ` clang-format -i skiplist.h -style=file `
   或者使用支持代码格式化的 IDE 格式化你的代码，格式化文件在根目录下的 .clang-format

## Step1 环境搭建

环境依赖：cmake3, g++

1. 安装 cmake3 和 g++

2. gteset下载

   进入到本目录下，`git submodule update --init`

## Step2 实现

代码在dscl目录下

dscl

├── skiplist.h

└── skiplist_test.cc

具体实现在 skiplist 下，测试代码在 skiplist_test.cc 下

```c++

template <typename Key, class Comparator>
class SkipList {
  void Insert(const Key& key);
  // Remove key in the list iff the list contains the key
  bool Remove(const Key& key);
  // Returns true iff an entry that compares equal to key is in the list.
  bool Contains(const Key& key) const;
}
```
`Key`： key的类型
`Comparator`：key的比较函数

任务：需要实现上述的接口，包括插入、删除以及判断是否存在某个 key.

其中跳表本质是一个链表结构，所以需要实现一个链表.

```C++
template <typename Key, class Comparator>
struct SkipList<Key, Comparator>::Node {
  Node(const Key& k, int height) : key(k) {
    // resize the next_ array of size height
  }

  explicit Node() : key(0) {}
  Key const key;

  // Accessors/mutators for links.
  Node* Next(int n) {
    // return the next level n of this Node
  }
  void SetNext(int n, Node* x) {
    // set the next Node of level n
  }

 private:
  // Array of length equal to the node height.
  std::vector<Node*> next_;
};
```

需要简单的接口即：获得下一个指定层级 n 的 Node.

最终需要实现一个 `Iterator`的接口方便对整个链表进行遍历.

```c++
class Iterator {
   public:
    // Initialize an iterator over the specified list.
    // The returned iterator is not valid.
    explicit Iterator(const SkipList* list);

    // Returns true iff the iterator is positioned at a valid node.
    // valid indicates the iterator is not nullptr
    bool Valid() const;

    // Returns the key at the current position.
    // REQUIRES: Valid()
    const Key& key() const;

    // Advances to the next position.
    // REQUIRES: Valid()
    void Next();

    // Advances to the previous position.
    // REQUIRES: Valid()
    void Prev(); //tips：can be implemented by find less than 

    // Advance to the first entry with a key >= target
    void Seek(const Key& target);

    // Position at the first entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    void SeekToFirst();

    // Position at the last entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    void SeekToLast();

   private:
    const SkipList* list_;
    Node* node_;
  };
```

实现上述的接口方便我们很好的遍历跳表.



## Step3 编译与测试

当你实现完你的跳表后，即可进行编译与测试

``` shell
mkdir -p build  # 创建build文件夹
cmake ..		# 构建项目的编译环境
make -j			# 开始编译
ctest			# 开始运行测试
```

可以根据自己的情况添加相应的单元测试.



## TODO

1. 支持一写多读  （可用锁/原子类型）



