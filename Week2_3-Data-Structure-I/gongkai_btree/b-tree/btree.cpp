//
// Created by Administrator on 2020/7/14 0014.
//
#include "btree.h"
using namespace std;
void btree::CreateBTree(BTree &T) {
    T = NULL;
}

int btree::isEmpty(BTree &T) {
    if (T == NULL)
        return TRUE;
    return FALSE;
}

int btree::search(BTree T,KeyType k){
    int i,found=0;
    BTree p=T;
    while(p!=NULL&&found==0){
        for(i=1;i<=p->keynum&&p->key[i]<k;i++);
        if(i<=p->keynum&&k==p->key[i])
            found=1;
        else
            p=p->ptr[i-1];
    }
    if(found==1)
        return TRUE;
    else
        return FALSE;
}

void btree::findBTreetoResult(BTree T, KeyType k, searchResult &r) {
    int i, found = 0;
    BTree p = T, q = NULL;
    while (p != NULL && found == 0) {
        i = 1;
        for (; i <= p->keynum && k > p->key[i]; i++);//i 找到第一个大于k的位置
        if (i <= p->keynum && k == p->key[i])
            found = 1;
        else {
            q = p;
            p = p->ptr[i - 1];
        }
    }
    if (1 == found) {
        r.pt = p;
        r.i = i;
        r.tag = 1;
    } else {
        r.pt = q;
        r.i = i;
        r.tag = 0;
    }
}

void btree::split(BTree &q, int s, BTree &ap) {
    int i, j;
    ap = (BTree) malloc(sizeof(BTNode));
    ap->ptr[0] = q->ptr[s];
    for (i = 1, j = s + 1; j <= q->keynum; i++, j++) {
        ap->ptr[i] = q->ptr[j];
        ap->key[i] = q->key[j];
    }
    ap->keynum = q->keynum - s;
    ap->parent = q->parent;
    for (i = 0; i <= ap->keynum; i++) {
        if (ap->ptr[i] != NULL)
            ap->ptr[i]->parent = ap;
    }
    q->keynum = s - 1;
}

void btree::insertBTree(BTree &T, KeyType k, BTree q, int i) {
    int x, s, finished = 0, needNewNode = 0;
    BTree ap;
    if (q == NULL)
        this->createNewNode(T, NULL, k, NULL);
    else {
        x = k;
        ap = NULL;
        while (finished == 0 && needNewNode == 0) {
            int j;
            for (j = q->keynum; j >= i; j--) {
                q->ptr[j + 1] = q->ptr[j];
                q->key[j + 1] = q->key[j];
            }
            q->ptr[i] = ap;
            q->key[i] = x;
            if (ap != NULL)
                ap->parent = q;
            q->keynum++;
            if (q->keynum < m)
                finished = 1;
            else {
                s = (m + 1) / 2;
                this->split(q, s, ap);
                x = q->key[s];
                if (q->parent != NULL) {
                    q = q->parent;
                    i = 1;
                    while (i <= q->keynum && x > q->key[i])
                        i++;
                } else
                    needNewNode = 1;
            }
        }
        if (needNewNode == 1) {
            this->createNewNode(T, q, x, ap);
        }
    }

}

int btree::insertValue(BTree &T, KeyType k) {
    searchResult r;
    this->findBTreetoResult(T, k, r);
    if (r.tag == 1) {
        printf("BTree中已经存在%d,插入失败\n\n", k);
        return FALSE;
    }
    this->insertBTree(T, k, r.pt, r.i);
    return TRUE;
}


int btree::deleteValue(BTree &T,KeyType k){
    searchResult r;
    this->findBTreetoResult(T,k,r);
    if(r.tag==0){
        printf("BTree中不存在%d,删除失败\n\n",k);
        return FALSE;
    }
    deleteFromNode(r.pt,r.i);
    if (T->keynum == 0) {
        while (r.pt->parent != NULL && r.pt->parent->keynum != 0) {
            r.pt = r.pt->parent;
        }
        T = r.pt;
        T->parent = NULL;
    }
    if (T->keynum == 0)
        T = NULL;
    return TRUE;
}

void btree::deleteFromNode(BTree &p,int i){
    if(p->ptr[i]==NULL&&p->parent==NULL){
        this->remove(p,i);
    }
    else if(p->ptr[i]!=NULL){
        BTree q=p->ptr[i];
        while(q->ptr[0]){//找后继
            q=q->ptr[0];
        }
        KeyType temp=q->key[1];
        q->key[1]=p->key[i];
        p->key[i]=temp;
        this->deleteFromNode(q,1);
    }
    else{
        this->remove(p,i);
        if(p->keynum<(m-1)/2)
            adjust(p);
    }
}

void btree::adjust(BTree &p){
    BTree q=p->parent;
    BTree left,right;
    int j;
    if(q==NULL)
        return;
    for(j=0;j<=q->keynum;j++){
        if(p==q->ptr[j])
            break;
    }
    if(j!=0){//借节点
        left=q->ptr[j-1];
        if(left->keynum>(m-1)/2){
            KeyType temp=q->key[j];
            q->key[j]=left->key[left->keynum];
            for(int i=p->keynum;i>=0;i--){
                p->ptr[i+1]=p->ptr[i];
                if(i>0)
                    p->key[i+1]=p->key[i];
            }
            p->key[1]=temp;
            p->keynum++;
            p->ptr[0]=left->ptr[left->keynum];
            if(p->ptr[0]!=NULL)
                p->ptr[0]->parent=p;
            this->remove(left,left->keynum);
            return;
        }
    }
    if(j!=q->keynum){//借节点
        right=q->ptr[j+1];
        if(right->keynum>(m-1)/2){
            KeyType temp=q->key[j+1];
            q->key[j+1]=right->key[1];
            p->key[p->keynum+1]=temp;
            p->keynum++;
            p->ptr[p->keynum]=right->ptr[0];
            if(p->ptr[p->keynum]!=NULL)
                p->ptr[p->keynum]->parent=p;
            for(int i=0;i<right->keynum;i++){
                if(i>0)
                    right->key[i]=right->key[i+1];
                right->ptr[i]=right->ptr[i+1];
            }
            right->keynum--;
            return;

        }
    }
    if(j>0){//合并
        BTree left=q->ptr[j-1];
        int i;
        for (i = left->keynum + 1; i <= left->keynum + 1 + p->keynum; i++) {
            if (i == left->keynum + 1) {
                left->key[i] = q->key[j];
                left->ptr[i] = p->ptr[0];
                if (p->ptr[0] != NULL)
                    p->ptr[0]->parent = left;
            }
            else {
                left->key[i] = p->key[i - left->keynum - 1];
                left->ptr[i] = p->ptr[i - left->keynum - 1];
                if (p->ptr[i - left->keynum - 1] != NULL)
                    p->ptr[i - left->keynum - 1]->parent = left;
            }
        }
        left->keynum += 1 + p->keynum;
        remove(q, j);
        p = left;
    }
    else{//合并
        BTree right = q->ptr[j + 1];
        int i;
        for (i = p->keynum+1; i <= p->keynum + 1 + right->keynum; i++) {
            if (i == p->keynum + 1) {
                p->key[i] = q->key[j + 1];
                p->ptr[i] = right->ptr[0];
                if (right->ptr[0] != NULL)
                    right->ptr[0]->parent = p;
            }
            else {
                p->key[i] = right->key[i - p->keynum - 1];
                p->ptr[i] = right->ptr[i - p->keynum - 1];
                if (right->ptr[i - p->keynum - 1] != NULL)
                    right->ptr[i - p->keynum - 1]->parent = p;
            }
        }
        p->keynum += 1 + right->keynum;
        remove(q, j+1);
    }
    if (q->keynum < (m - 1) / 2 && q->parent != NULL)
        this->adjust(q);


}




void btree::remove(BTree p,int i){
    for(int j=i;j<p->keynum;j++){
        p->key[j]=p->key[j+1];
        p->ptr[j]=p->ptr[j+1];
    }
    p->ptr[p->keynum]=NULL;
    p->keynum--;
}

void btree::createNewNode(BTree &T, BTree p1, KeyType x, BTree p2) {
    T = (BTree) malloc(sizeof(BTNode));
    T->parent = NULL;
    T->keynum = 1;
    T->key[1] = x;
    T->ptr[0] = p1;
    T->ptr[1] = p2;
    if (p1 != NULL)
        p1->parent = T;
    if (p2 != NULL)
        p2->parent = T;
}

void traversal::initQueue(Queue &Q) {
    Q.front = NULL;
    Q.rear = NULL;
}

int traversal::isEmptyQueue(Queue &Q) {
    if (Q.front ==NULL && Q.rear == NULL)
        return TRUE;
    else
        return FALSE;
}

int traversal::EnQueue(Queue &Q, ElemType e) {
    QNodeptr qnode = (QNodeptr) malloc(sizeof(QNode));
    if (qnode == NULL)
        return FALSE;
    qnode->data = e;
    qnode->next = NULL;
    if (this->isEmptyQueue(Q)) {
        Q.front = qnode;
        Q.rear = qnode;
    } else {
        Q.rear->next=qnode;
        Q.rear=qnode;
    }
    return TRUE;
}

int traversal::DeQueue(Queue &Q, ElemType &e) {
    if (isEmptyQueue(Q))
        return FALSE;
    e = Q.front->data;
    QNodeptr q = Q.front;
    if(Q.front->next==NULL){
        Q.rear=Q.front=NULL;
    }
    else{
        Q.front=Q.front->next;
    }
    free(q);
    return TRUE;
}

int traversal::levelOrderBTree(BTree T) {
    Queue Q;
    int flag;
    this->initQueue(Q);
    BTree p=NULL, q1, q2;
    this->EnQueue(Q, T);
    while (0==this->isEmptyQueue(Q)) {
        this->DeQueue(Q, p);
        if (p != NULL) {
            this->EnQueue(Q, p->ptr[0]);
            for (int i = 1; i <= p->keynum; i++) {
                EnQueue(Q, p->ptr[i]);
                if (i < p->keynum) {
                    printf("%d,", p->key[i]);
                }
                if (i == p->keynum) {
                    printf("%d", p->key[i]);
                }
            }
            if (p->parent == NULL)
                printf("\n");
            else {
                flag = 0;
                q1 = q1 = p;
                while (q1->parent != NULL) {
                    q2 = q1;
                    q1 = q1->parent;
                    if (q1->ptr[q1->keynum] != q2)
                        flag = 1;
                }
                if (flag == 0)
                    printf("\n");
                else
                    printf("   ");
            }
        }
    }
    return TRUE;
}

int traversal::inOrderBTree(BTree T) {
    if(T==NULL)
        return FALSE;
    for(int i=0;i<=T->keynum;i++){
        inOrderBTree(T->ptr[i]);
        if(i!=T->keynum)
            printf("%d ",T->key[i+1]);
    }
    return TRUE;
}

int btree::keyNumSum(BTree T){
    Queue Q;
    this->initQueue(Q);
    BTree p;
    EnQueue(Q,T);
    int sum=0;
    while(!isEmptyQueue(Q)){
        DeQueue(Q,p);
        if(p!=NULL){
            sum+=p->keynum;
            for(int i=0;i<=p->keynum;i++){
                EnQueue(Q,p->ptr[i]);
            }
        }
    }
    return sum;
}

int btree::BTreeDepth(BTree T){

    if(T==NULL)
        return 0;
    int depth=1;
    BTree p=T;
    while(p->ptr[p->keynum]!=NULL){
        depth++;
        p=p->ptr[p->keynum];
    }
    return depth;

}