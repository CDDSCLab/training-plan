#include "dscl/skiplist.h"

#include <atomic>
#include <set>

#include "gtest/gtest.h"

namespace skiplist {
    typedef uint64_t Key;

    struct Comparator {
        int operator()(const Key &a, const Key &b) const {
            if (a < b) {
                return -1;
            } else if (a > b) {
                return +1;
            } else {
                return 0;
            }
        }
    };

    TEST(SkipTest, Empty) {
        Comparator cmp;
        SkipList<Key, Comparator> list(cmp);
        ASSERT_TRUE(!list.Contains(10));
        SkipList<Key, Comparator>::Iterator iter(&list);
        ASSERT_TRUE(!iter.Valid());
        iter.SeekToFirst();
        ASSERT_TRUE(!iter.Valid());
        iter.Seek(100);
        ASSERT_TRUE(!iter.Valid());
        iter.SeekToLast();
        ASSERT_TRUE(!iter.Valid());
    }

    TEST(SkipTest, InsertAndLookup) {
        const int N = 2000;
        const int R = 5000;
        Random rnd(1000);
        std::set<Key> keys;
        Comparator cmp;
        SkipList<Key, Comparator> list(cmp);
        // test remove a not exist number;

        // use set to insert random number in 5000 to list none repeatedly.
        for (int i = 0; i < N; i++) {
            Key key = rnd.Next() % R;
            // return pair<set<int>::iterator,bool> .second whether insert success or
            // fail
            if (keys.insert(key).second) {
                list.Insert(key);
            }
        }

        // Simple iterator tests
        {
            SkipList<Key, Comparator>::Iterator iter(&list);
            ASSERT_TRUE(!iter.Valid());

            // seek by value
            iter.Seek(0);
            ASSERT_TRUE(iter.Valid());
            // set has oreder
            ASSERT_EQ(*(keys.begin()), iter.key());

            iter.SeekToFirst();
            ASSERT_TRUE(iter.Valid());
            ASSERT_EQ(*(keys.begin()), iter.key());

            iter.SeekToLast();
            // seek to last
            ASSERT_TRUE(iter.Valid());
            ASSERT_EQ(*(keys.rbegin()), iter.key());
        }

        // Forward iteration test
        {
            for (int i = 0; i < R; i++) {
                SkipList<Key, Comparator>::Iterator iter(&list);
                iter.Seek(i);

                // Compare against model iterator
                // lower_bound 不小于 up_bound 大于
                std::set<Key>::iterator model_iter = keys.lower_bound(i);
                for (int j = 0; j < 3; j++) {
                    if (model_iter == keys.end()) {
                        ASSERT_TRUE(!iter.Valid());
                        break;
                    } else {
                        ASSERT_TRUE(iter.Valid());
                        ASSERT_EQ(*model_iter, iter.key());
                        ++model_iter;
                        iter.Next();
                    }
                }
            }
        }

        // Backward iteration test
        {
            SkipList<Key, Comparator>::Iterator iter(&list);
            iter.SeekToLast();

            // Compare against model iterator
            for (std::set<Key>::reverse_iterator model_iter = keys.rbegin();
                 model_iter != keys.rend(); ++model_iter) {
                ASSERT_TRUE(iter.Valid());
                ASSERT_EQ(*model_iter, iter.key());
                iter.Prev();
            }
            ASSERT_TRUE(!iter.Valid());
        }

        // Contains Remove  Remove Contains
        for (int i = 0; i < R; i++) {
            if (list.Contains(i)) {
                ASSERT_EQ(keys.count(i), 1);
                ASSERT_TRUE(list.Remove(i));
                ASSERT_FALSE(list.Contains(i));
            } else {
                ASSERT_EQ(keys.count(i), 0);
                ASSERT_FALSE(list.Remove(i));
                ASSERT_FALSE(list.Contains(i));
            }
        }
    }

}  // namespace skiplist

using namespace skiplist;
#define TEST_RANDOM_LEVELDB_REDIS 1
#define TEST_GOOGLE 1
#define MY_TEST 0
int main(int argc, char **argv) {
#if MY_TEST
    Comparator cmp;
    SkipList<Key, Comparator> list(cmp);
    SkipList<Key, Comparator>::Iterator iter(&list);
    for(int i=0; i<100;i++){
        list.Insert(i);
    }
    list.Dump();
#endif

#if TEST_RANDOM_LEVELDB_REDIS
    int kBranching =4;
    double SKIPLIST_P = 0.5;
    int count =0;
    int random;
    Random rnd_(0xdeadbeef);
    std::vector<int> v1;
    std::vector<int> v2;
    std::vector<int> v3;
    for(int i=0;i<100;i++){
        random = rnd_.Next();
        v1.push_back(random);
        v2.push_back(random%kBranching==0);
        v3.push_back((random & 0xFFFF) < (SKIPLIST_P * 0xFFFF));

    }

    for (auto iter = v1.begin(); iter != v1.end(); iter++) {
        std::cout << (*iter)<<" ";
    }
    std::cout<<std::endl;
    for (auto iter = v2.begin(); iter != v2.end(); iter++) {
        std::cout << (*iter)<<" ";
    }
    std::cout<<std::endl;
    for (auto iter = v3.begin(); iter != v3.end(); iter++) {
        std::cout << (*iter)<<" ";
    }
#endif

#if TEST_GOOGLE
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
}
