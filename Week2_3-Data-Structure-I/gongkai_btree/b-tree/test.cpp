//
// Created by gongkai on 2020/7/24.
//
#include "btree.h"
#include "gtest/gtest.h"

using namespace std;

TEST(btreee, insert) {
    BTree T;
    btree t_obj;
    traversal traversal_obj;
    t_obj.CreateBTree(T);
    ASSERT_EQ(1, t_obj.isEmpty(T));
    ASSERT_EQ(0,t_obj.inOrderBTree(T));
    int a[20];
    srand((unsigned) time(NULL));
    for (int i = 0; i < 20; i++) {
        a[i] = rand() % 1000;
        ASSERT_EQ(1, t_obj.insertValue(T, a[i]));
    }
    EXPECT_EQ(1, t_obj.insertValue(T, 1897));
    EXPECT_NE(1, t_obj.isEmpty(T));
    ASSERT_EQ(21, t_obj.keyNumSum(T));
    for (int i = 0; i < 20; i++)
        ASSERT_EQ(1, t_obj.search(T, a[i]));
    ASSERT_EQ(1, t_obj.deleteValue(T, a[1]));
    ASSERT_EQ(20, t_obj.keyNumSum(T));
    ASSERT_NEAR(3, t_obj.BTreeDepth(T), 1);
    ASSERT_EQ(1,t_obj.inOrderBTree(T));
    ASSERT_EQ(1,t_obj.levelOrderBTree(T));
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}