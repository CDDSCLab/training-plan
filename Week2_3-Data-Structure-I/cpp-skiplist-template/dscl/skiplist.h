#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <cassert>
#include <cstdlib>
#include <vector>
#include "util/random.h"

namespace skiplist {

template <typename Key, class Comparator>
class SkipList {
 private:
  struct Node;

 public:
  // Create a new SkipList object that will use "cmp" for comparing keys,
  explicit SkipList(Comparator cmp);

  SkipList(const SkipList&) = delete;
  SkipList& operator=(const SkipList&) = delete;

  // Insert key into the list.
  // REQUIRES: nothing that compares equal to key is currently in the list.
  void Insert(const Key& key);
  // Remove key in the list iff the list contains the key
  bool Remove(const Key& key);
  // Returns true iff an entry that compares equal to key is in the list.
  bool Contains(const Key& key) const;

  // Iteration over the contents of a skip list
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
    void Prev();

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

 private:
  enum { kMaxHeight = 12 };

  inline int GetMaxHeight() const { return max_height_; }

  Node* NewNode(const Key& key, int height);
  int RandomHeight();
  bool Equal(const Key& a, const Key& b) const { return (compare_(a, b) == 0); }

  // Return true if key is greater than the data stored in "n"
  bool KeyIsAfterNode(const Key& key, Node* n) const;

  // Return the earliest node that comes at or after key.
  // Return nullptr if there is no such node.
  //
  // If prev is non-null, fills prev[level] with pointer to previous
  // node at "level" for every level in [0..max_height_-1].
  Node* FindGreaterOrEqual(const Key& key, Node** prev) const;

  // Return the latest node with a key < key.
  // Return head_ if there is no such node.
  Node* FindLessThan(const Key& key) const;

  // Return the last node in the list.
  // Return head_ if list is empty.
  Node* FindLast() const;

  // Immutable after construction
  Comparator const compare_;

  Node* const head_;

  // Modified only by Insert().
  int max_height_;  // Height of the entire list

  // Read/written only by Insert().
  Random rnd_;
};

// Implementation details follow
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

// Implement your code

}  // namespace skiplist

#endif  // DSCL_SKIPLIST_H_
