//
// Created by icepig on 7/21/20.
//
/*
 * A simple sorted linked list imitate from LevelDB skiplist
 * Todo 1 smart ptr
 * Todo 2 atomic op for currency
 */
#ifndef SORTED_SINGLE_LINKED_LIST_H_
#define SORTED_SINGLE_LINKED_LIST_H_
#include <cassert>
#include <iostream>
// for memory test
static int count = 0;

namespace sortedsinglelinkedlist {

template <typename Key, class Comparator>
class SortedSingleLinkedList {
 private:
  // Pre-declaration
  struct Node;

 public:
  explicit SortedSingleLinkedList(Comparator cmp);
  // traverse to free the node
  ~SortedSingleLinkedList() {
    Node* iter = head_;
    Node* x;
    while (iter != nullptr) {
      x = iter;
      iter = iter->Next();
      delete x;
    }
    // for memory check
    std::cout << count << std::endl;
  }
  // delete copy and operator =
  SortedSingleLinkedList(const SortedSingleLinkedList&) = delete;
  SortedSingleLinkedList& operator=(const SortedSingleLinkedList&) = delete;
  // Insert key into the list.
  // REQUIRES: nothing that compares equal to key is currently in the list.
  void Insert(const Key& key);
  // Returns true  iff remove the key  successfully in the list iff the list
  // contains the key
  bool Remove(const Key& key);
  // Returns true iff an entry that compares equal to key is in the list.
  bool Contains(const Key& key) const;
  // Print the SortedSingleLinkedList bottom-up.
  void Dump() const;

  // Iteration over the contents of a sorted single linked list.
  class Iterator {
   public:
    // Initialize an iterator over the sorted single linked list.
    // The returned iterator is not valid.
    explicit Iterator(const SortedSingleLinkedList* list)
        : list_(list), node_(nullptr){};

    // Returns true iff the iterator is positioned at a valid node.
    // valid indicates the iterator is not nullptr
    bool Valid() const { return node_ != nullptr; };
    // Returns the key at the current position.
    // REQUIRES: Valid()
    const Key& key() const {
      assert(Valid());
      return node_->GetKey();
    };
    // Advances to the next position.
    // REQUIRES: Valid()
    void Next() {
      assert(Valid());
      node_ = node_->Next();
    };
    // Advances to the previous position.
    // REQUIRES: Valid()
    void Prev() {
      assert(Valid());
      node_ = list_->FindLessThan(this->node_->GetKey());
      // iff node_ is head_ ,nullptr;
      if (node_ == list_->head_) {
        node_ = nullptr;
      }
    };

    // Advance to the first entry with a key >= target,may be nullptr
    void Seek(const Key& target) {
      node_ = list_->FindGreaterOrEqual(target, nullptr);
    };
    // Position at the first entry in list.
    // if empty ,head_->Next() = nullptr,so there is no need to check empty.
    // Final state of iterator is Valid() iff list is not empty.
    void SeekToFirst() { node_ = list_->head_->Next(); };
    // Position at the last entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    void SeekToLast() {
      node_ = list_->FindLast();
      // check empty
      if (node_ == list_->head_) {
        node_ = nullptr;
      }
    };

   private:
    //
    const SortedSingleLinkedList* list_;
    Node* node_;
  };

 private:
  // a list with head Node
  // in this implement  head_ does not maintain the length or the list
  // because of the list is sorted, so the key is umimmutable
  Node* head_;
  // Immutable after construction
  const Comparator compare_;

  Node* NewNode(const Key& key);

  bool KeyIsAfterNode(const Key& key, Node* node) const {
    return (node != nullptr && compare_(node->GetKey(), key) < 0);
  };
  bool Equal(const Key& a, const Key& b) const { return (compare_(a, b) == 0); }

  Node* FindGreaterOrEqual(const Key& key, Node** prev) const;

  Node* FindLessThan(const Key& key) const;

  Node* FindLast() const;
};

template <typename Key, class Comparator>
struct SortedSingleLinkedList<Key, Comparator>::Node {
 public:
  explicit Node(const Key& k) : key(k), next_(nullptr) {
    //    std::cout << "cons" << std::endl;
    count++;
  }
  ~Node() {
    //    std::cout << "des" << std::endl;
    count--;
  }
  Node* Next() { return next_; }

  const Key& GetKey() { return key; }

  void SetNext(Node* next) { next_ = next; }

 private:
  Key const key;
  Node* next_;
};
template <typename Key, class Comparator>
SortedSingleLinkedList<Key, Comparator>::SortedSingleLinkedList(Comparator cmp)
    : compare_(cmp), head_(NewNode(0)) {}

template <typename Key, class Comparator>
void SortedSingleLinkedList<Key, Comparator>::Insert(const Key& key) {
  Node* prev[1];
  FindGreaterOrEqual(key, prev);
  Node* x = NewNode(key);
  x->SetNext((*prev)->Next());
  (*prev)->SetNext(x);
}

template <typename Key, class Comparator>
bool SortedSingleLinkedList<Key, Comparator>::Remove(const Key& key) {
  Node* prev[1];
  Node* x = FindGreaterOrEqual(key, prev);
  // find it
  if (x != nullptr && x->GetKey() == key) {
    (*prev)->SetNext(x->Next());
    delete x;
    return true;
  }
  return false;
}

template <typename Key, class Comparator>
bool SortedSingleLinkedList<Key, Comparator>::Contains(const Key& key) const {
  Node* x = FindGreaterOrEqual(key, nullptr);
  return x != nullptr && x->GetKey() == key;
}

template <typename Key, class Comparator>
typename SortedSingleLinkedList<Key, Comparator>::Node*
SortedSingleLinkedList<Key, Comparator>::NewNode(const Key& key) {
  Node* node = new Node(key);
  return node;
}

template <typename Key, class Comparator>
typename SortedSingleLinkedList<Key, Comparator>::Node*
SortedSingleLinkedList<Key, Comparator>::FindGreaterOrEqual(
    const Key& key, SortedSingleLinkedList::Node** prev) const {
  Node* x = head_;
  while (true) {
    Node* next = x->Next();
    if (KeyIsAfterNode(key, next)) {
      x = next;
    } else {
      if (prev != nullptr) {
        *prev = x;
      }
      return next;
    }
  }
}

template <typename Key, class Comparator>
typename SortedSingleLinkedList<Key, Comparator>::Node*
SortedSingleLinkedList<Key, Comparator>::FindLessThan(const Key& key) const {
  Node* x = head_;
  while (true) {
    Node* next = x->Next();
    if (KeyIsAfterNode(key, next)) {
      x = next;
    } else {
      return x;
    }
  }
}

template <typename Key, class Comparator>
typename SortedSingleLinkedList<Key, Comparator>::Node*
SortedSingleLinkedList<Key, Comparator>::FindLast() const {
  Node* x = head_;
  while (true) {
    Node* next = x->Next();
    if (next != nullptr) {
      x = next;
    } else {
      return x;
    }
  }
}
template <typename Key, class Comparator>
void SortedSingleLinkedList<Key, Comparator>::Dump() const {
  Node* iter = head_;
  while (iter != nullptr) {
    std::cout << iter->GetKey() << " -> ";
    iter = iter->Next();
  }
  std::cout << std::endl;
}

}  // namespace sortedsinglelinkedlist
#endif  // SORTED_SINGLE_LINKED_LIST_H_
