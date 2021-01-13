//
// Created by icepig on 7/21/20.
//
#include "dscl/sorted_single_linked_list.h"

#include <atomic>
#include <set>

#include "util/random.h"

#include "gtest/gtest.h"
namespace sortedsinglelinkedlist {

// template
typedef uint64_t Key;
struct Comparator {
  int operator()(const Key& a, const Key& b) const {
    if (a < b) {
      return -1;
    } else if (a > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

TEST(SortedSingleLinkedListTest, Empty) {
  Comparator cmp;
  SortedSingleLinkedList<Key, Comparator> list(cmp);
  ASSERT_TRUE(!list.Contains(10));
  SortedSingleLinkedList<Key, Comparator>::Iterator iter(&list);
  ASSERT_TRUE(!iter.Valid());
  iter.SeekToFirst();
  ASSERT_TRUE(!iter.Valid());
  iter.Seek(100);
  ASSERT_TRUE(!iter.Valid());
  iter.SeekToLast();
  ASSERT_TRUE(!iter.Valid());
}

TEST(SortedSingleLinkedListTest, InsertAndLookupAndRemove) {
  const int N = 2000;
  const int R = 5000;
  skiplist::Random rnd(1000);
  std::set<Key> keys;
  Comparator cmp;
  SortedSingleLinkedList<Key, Comparator> list(cmp);

  // use set to insert random number in 5000 to list none repeatedly.
  for (int i = 0; i < N; i++) {
    Key key = rnd.Next() % R;
    // return pair<set<int>::iterator,bool> .second whether insert success or
    // fail
    if (keys.insert(key).second) {
      // list could Insert a same value.
      list.Insert(key);
    }
  }

  // Simple iterator tests
  {
    SortedSingleLinkedList<Key, Comparator>::Iterator iter(&list);
    ASSERT_TRUE(!iter.Valid());

    // seek by value
    iter.Seek(0);
    ASSERT_TRUE(iter.Valid());
    // set has order
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
      SortedSingleLinkedList<Key, Comparator>::Iterator iter(&list);
      // >=
      iter.Seek(i);

      // Compare against model iterator
      // lower_bound 下界 >= upper_bound 上界
      std::set<Key>::iterator model_iter = keys.lower_bound(i);
      for (int j = 0; j < 3; j++) {
        // iff  model_iter is after the last one break
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
    SortedSingleLinkedList<Key, Comparator>::Iterator iter(&list);
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
}  // namespace sortedsinglelinkedlist
using namespace sortedsinglelinkedlist;
int main(int argc, char** argv) {
  // Note that static variable share ability
  {
    Comparator cmp;
    SortedSingleLinkedList<Key, Comparator> list(cmp);
  }

  {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }
}