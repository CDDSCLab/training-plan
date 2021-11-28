# executor项目实现和问题总结

2021.11.26 by Sunlly0


该项目一共有两个Task：

- Task #1 - System Catalog
- Task #2 - Executors

#### Task 1:

主要是完善`src/include/catalog/catalog.h`中的几个方法，这个文件里定义了执行器中存放表和索引基本数据结构，
包括7个API:

```C++
//表：
CreateTable(Transaction *txn, const std::string &table_name, const Schema &schema)
GetTable(const std::string &table_name)
GetTable(table_oid_t table_oid)

//索引：
CreateIndex(txn, index_name, table_name, schema, key_schema key_attrs, keysize)
GetIndex(const std::string &index_name, const std::string &table_name)
GetIndex(index_oid_t index_oid),
GetTableIndexes(const std::string &table_name)
```

索引的定义用到了之前project2实现的B+树，如果B+树实现得有问题可能会报错。

#### Task 2:

按照官方文档中的要求实现9个执行器，每个类中需要实现Init()和Next()两个方法。

官方给出的代码中已经有很多实现好了的接口，可以直接调用。

实现时，首先应梳理清楚各个类之间的关系，熟悉里面一些会用到的API。比如tuple和tableheap中定义了许多api，可以直接使用（插入、删除、tableIterator等等）。


可以重点看一下`src/include/storage/`和`src/include/execution/expressions`下几个类

## 实现情况

task1：catalog_test测试通过。

![](./csq_images/catalog_test.png)

task2：executor_test测试均通过。

![](./csq_images/executor_test.png)

grading_executor_test测试通过。

![](./csq_images/grading_executor_test.png)

## 问题

#### 1.什么时候会调用executor中的Init和Next函数？

观察executor_engine.h中的Execute函数，有：

```C++
// prepare
executor->Init();

// execute
try {
  Tuple tuple;
  RID rid;
  while (executor->Next(&tuple, &rid)) {
    if (result_set != nullptr) {
      result_set->push_back(tuple);
    }
  }
}
...
```

可知，在执行过程中，Init函数只会被调用一次，而Next函数会执行多次（while循环），并在在每次返回true时重复执行，并将符合谓词条件（用predicate的Evaluate系列函数判断）的tuple和rid的返回结果添加到result_set中。

当Next返回false的时候，执行器执行结束，此时一般为执行器的iterator到了结尾（scan_executor），不再返回元组。

对于insert_executor、delete_executor等执行器，可能不需要返回tuple，此时可在Next内做循环（使用while,不再返回true，此时Next只会被调用一次）。

#### 2、所有执行器中的谓词判断是怎么实现的？

在这个项目中，所有的表达式都基于一个抽象类`abstract_expression.h`，在所有测试文件中谓词都被初始化为一个比较表达式`comparison_expression.h`,谓词的判断都是通过调用这个类中的Evaluate()方法实现的。

```c++
Value Evaluate(const Tuple *tuple, const Schema *schema) const override {
    Value lhs = GetChildAt(0)->Evaluate(tuple, schema);
    Value rhs = GetChildAt(1)->Evaluate(tuple, schema);
    return ValueFactory::GetBooleanValue(PerformComparison(lhs, rhs));
  }

  Value EvaluateJoin(const Tuple *left_tuple, const Schema *left_schema, const Tuple *right_tuple,
                     const Schema *right_schema) const override {
    Value lhs = GetChildAt(0)->EvaluateJoin(left_tuple, left_schema, right_tuple, right_schema);
    Value rhs = GetChildAt(1)->EvaluateJoin(left_tuple, left_schema, right_tuple, right_schema);
    return ValueFactory::GetBooleanValue(PerformComparison(lhs, rhs));
  }

  Value EvaluateAggregate(const std::vector<Value> &group_bys, const std::vector<Value> &aggregates) const override {
    Value lhs = GetChildAt(0)->EvaluateAggregate(group_bys, aggregates);
    Value rhs = GetChildAt(1)->EvaluateAggregate(group_bys, aggregates);
    return ValueFactory::GetBooleanValue(PerformComparison(lhs, rhs));
  }
```

针对不同的执行器有不同的函数调用，但思想都是一样的，比较表达式在初始化时都会有两个子节点表达式，一个是条件(常量表达式)，一个则是需要判断的列值表达式。
#### 3.如何使Next中输出的tuple符合OutputSchema？

可增加一个转换函数，将从迭代器中获取的tuple转换成为希望的schema形式。比如：

```C++
Tuple SeqScanExecutor::GetOutputTuple(const Tuple tuple, const Schema *schema){
  std::vector<Value> vals;
  for (const Column &col : schema->GetColumns()) {
    Value val = tuple.GetValue(schema, schema->GetColIdx(col.GetName()));
    vals.push_back(val);
  }
  return Tuple(vals,schema);
}

```

#### 4.aggregation执行器中提供的哈希表有什么用？

在没有Group by的聚合操作中，一般只需要返回一行结果，所以只需要调用Next()一次。但在有Group by的聚合操作中，就可能返回多行结果。

提供的`SimpleAggregationHashTable`结构就是为了区分不同的组中的aggregation值。

```c++
SimpleAggregationHashTable(const std::vector<const AbstractExpression *> &agg_exprs,
                             const std::vector<AggregationType> &agg_types)
      : agg_exprs_{agg_exprs}, agg_types_{agg_types} {}

private:
  /** The hash table is just a map from aggregate keys to aggregate values. */
  std::unordered_map<AggregateKey, AggregateValue> ht{};
  /** The aggregate expressions that we have. */
  const std::vector<const AbstractExpression *> &agg_exprs_;
  /** The types of aggregations that we have. */
  const std::vector<AggregationType> &agg_types_;
```

AggregateKey中存放的是group by的列的不同组合，AggregateValue中存放的是各种聚合函数的结果。经过这样处理后，按照group by分出来的各个组都存有一个自己的聚合函数结果，只需要在聚合执行器中多次调用Next()函数就可以获得不同小组的聚合值。

#### 5.索引在执行器的实现中有什么用？

此处使用的索引Index就是我们在project2中实现的b+树。

在索引扫描执行器(index_scan_executor)中，其实是和seq_scan_executor的table_iterator类似，使用到index_iterator遍历所有的叶子节点。

在插入执行中，在插入tuple后，会调用索引的InsertEntry为索引增加一个key-value对。此处的key是一个由index_schema指定下的key_tuple，value是元组的rid。

```C++
auto index_key=insert_tuple.KeyFromTuple(table_info_->schema_,index_info->key_schema_,index_info->index_->GetKeyAttrs());
index_info->index_->InsertEntry(index_key,*rid,exec_ctx_->GetTransaction());
```

在更新执行中，由于元组变化，key_tuple也可能会变化，此时将原来的索引中元组对应的index_key删除，再做重新插入。

```C++
auto index_key=tuple->KeyFromTuple(table_info_->schema_,index_info->key_schema_,index->GetKeyAttrs());
index->DeleteEntry(index_key,*rid,exec_ctx_->GetTransaction());
index->InsertEntry(index_key,*rid,exec_ctx_->GetTransaction());
```

在删除执行时，在删除元组的同时，将元组对应的index_key删除。注意此处删除表中的元组使用的是MarkDelete，也就是标记删除。真正的删除在事务提交时执行。

注意一个表上可能有多个索引(index_key不同)，应该对所有的索引都做处理。

```C++
for(auto &index_info:all_index_info_){
  auto index_key=tuple->KeyFromTuple(table_info_->schema_,index_info->key_schema_,index_info->index_->GetKeyAttrs());
  index_info->index_->DeleteEntry(index_key,*rid,exec_ctx_->GetTransaction());
}
```

在索引嵌套连接中，对于内层表，使用索引的scanKey来快速查找索引中的对应元组的rid，进一步从表中找到元组。

#### 6.为何执行器实现没有错误，测试却通不过？

有可能是前一个项目bplustree中的问题。主要集中在判空的条件上（比如is_empty）或并发加锁有误。

针对判空问题，可在实现过程中多用断言，方便后续调试。

死循环一般是加锁后忘了解锁，或重复加锁导致，检查一下就能发现错误所在。比如，忘记在上锁后将page添加到transation::page_set，或者在CoalesceOrRedistribute()函数实现中有对兄弟节点重复加锁的情况。

另外在跑测试之前可以尝试先去看看相应的测试文件，因为项目中没有涉及到SQL语句解析的部分，所以查询计划都是由手动初始化获得的，先看看测试文件可以很好的帮助理解不同的执行器中会调用到哪些类、函数，也可以明白执行器中用抽象类初始化的变量具体是指向哪个类。


参考：

1. executor:cum课程笔记： cnblogs.com/JayL-zxl/p/14460304.html
