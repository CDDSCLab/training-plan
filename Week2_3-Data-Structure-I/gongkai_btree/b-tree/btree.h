//
// Created by Administrator on 2020/7/14 0014.
//

#ifndef B_TREE_BTREE_H
#define B_TREE_BTREE_H
using namespace std;
#include "stdio.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"


#define m 3
#define TRUE 1
#define FALSE 0
typedef int KeyType;
typedef struct BTNode {
    int keynum;
    KeyType key[m + 1];  //不用key[0]
    struct BTNode *parent;
    struct BTNode *ptr[m + 1];
} BTNode, *BTree;




typedef BTree ElemType;




class traversal {
protected:
    typedef struct QNode {
        ElemType data;
        struct QNode *next;
    } QNode, *QNodeptr;

    typedef struct {
        QNodeptr front;
        QNodeptr rear;
    } Queue;

public:
    traversal(){};

    void initQueue(Queue &Q);

    int isEmptyQueue(Queue &Q);

    int EnQueue(Queue &Q, ElemType e);

    int DeQueue(Queue &Q, ElemType &e);

    int levelOrderBTree(BTree T);

    int inOrderBTree(BTree T);

};

class btree : public traversal {
protected:
    typedef struct {
        BTree pt;
        int i;
        int tag;
    } searchResult;
public:
    btree(){};

    void CreateBTree(BTree &T);

    int isEmpty(BTree &T);

    void createNewNode(BTree &T, BTree p1, KeyType x, BTree p2);

    int insertValue(BTree &T, KeyType k);

    void insertBTree(BTree &T, KeyType k, BTree q, int i);

    void findBTreetoResult(BTree T, KeyType k, searchResult &r);

    int keyNumSum(BTree T);

    int BTreeDepth(BTree T);

    void split(BTree &q, int s, BTree &ap);

    int search(BTree T,KeyType k);

    int deleteValue(BTree &T,KeyType k);

    void deleteFromNode(BTree &p,int i);

    void remove(BTree p,int i);

    void adjust(BTree &p);
};



#endif //B_TREE_BTREE_H
