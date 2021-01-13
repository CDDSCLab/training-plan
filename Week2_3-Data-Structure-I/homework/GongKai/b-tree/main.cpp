#include <iostream>
#include "btree.h"
using namespace std;

int main() {
    BTree T;
    btree t_obj;
    traversal traversal_obj;
    t_obj.CreateBTree(T);
    if (t_obj.isEmpty(T))
        printf("成功创建一棵空B树\n\n");
    else
        printf("创建一棵B树失败\n\n");
    int a[20];
    srand((unsigned) time(NULL));
    for (int i = 0; i < 20; i++)
        a[i] = rand() % 1000;
    printf("将如下数字按序插入B树中:\n");
    bool success;
    for (int i = 0; i < 20; i++) {
        printf("插入%d:\n", a[i]);
        success = t_obj.insertValue(T, a[i]);
        if (success == TRUE) {
            printf("中序遍历结果为：");
            traversal_obj.inOrderBTree(T);
            printf("\n插入%d成功,此时树中有%d个元素，树的深度为%d，树的形状如下：\n", a[i],t_obj.keyNumSum(T),t_obj.BTreeDepth(T));
            traversal_obj.levelOrderBTree(T);

            printf("\n");
        }
    }
    printf("开始查询数据，退出查询输入-1即可\n");
    int scan;
    scanf("%d",&scan);
    while(scan!=-1){
        if(t_obj.search(T,scan))
            printf("查询%d成功\n",scan);
        else
            printf("查询%d失败\n",scan);
        scanf("%d",&scan);
    }
    printf("开始删除数据，退出删除输入-1即可\n");
    scanf("%d",&scan);
    while(scan!=-1){
        if(t_obj.deleteValue(T,scan))
            printf("删除%d成功，此时树中有%d个元素，树的深度为%d，树的形状如下：\n",scan,t_obj.keyNumSum(T),t_obj.BTreeDepth(T));
        else
            printf("删除%d失败，此时树中有%d个元素，树的深度为%d，树的形状如下：\n",scan,t_obj.keyNumSum(T),t_obj.BTreeDepth(T));
        traversal_obj.levelOrderBTree(T);
        scanf("%d",&scan);
    }
}
