#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "util/random.h"

namespace skiplist {
    template<typename Key, class Comparator>
    class SkipList {
    private:
        // Pre-declaration
        struct Node;

    public:
        // Create a new SkipList object that will use "cmp" for comparing keys,
        explicit SkipList(Comparator cmp);

        // delete copy and operator =
        SkipList(const SkipList &) = delete;

        SkipList &operator=(const SkipList &) = delete;

        // Insert key into the list.
        // REQUIRES: nothing that compares equal to key is currently in the list.
        void Insert(const Key &key);

        // Returns true  iff remove the key  successfully in the list iff the list
        // contains the key
        bool Remove(const Key &key);

        // Returns true iff an entry that compares equal to key is in the list.
        bool Contains(const Key &key) const;

        // Print the skiplist bottom-up.
        void Dump() const;

        // return the max height.
        inline int GetMaxHeight() const { return heights.rbegin()->first; }

        // Iteration over the contents of a skip list
        class Iterator {
        public:
            // Initialize an iterator over the specified list.
            // The returned iterator is not valid.
            explicit Iterator(const SkipList *list);

            // Returns true iff the iterator is positioned at a valid node.
            // valid indicates the iterator is not nullptr
            bool Valid() const;

            // Returns the key at the current position.
            // REQUIRES: Valid()
            const Key &key() const;

            // Advances to the next position.
            // REQUIRES: Valid()
            void Next();

            // Advances to the previous position.
            // REQUIRES: Valid()
            void Prev();

            // Advance to the first entry with a key >= target,may be nullptr
            void Seek(const Key &target);

            // Position at the first entry in list.
            // Final state of iterator is Valid() iff list is not empty.
            void SeekToFirst();

            // Position at the last entry in list.
            // Final state of iterator is Valid() iff list is not empty.
            void SeekToLast();

        private:
            const SkipList *list_;
            Node *node_;
        };

    private:
        enum {
            kMaxHeight = 12
        };

        // Immutable after construction
        Comparator const compare_;
        std::unique_ptr<Node> head_;

        // Read/written only by Insert().
        Random rnd_;
        // record the node number in every heights
        // map Ascending order the last one first meas the max height.
        std::map<int, int> heights;

        Node *NewNode(const Key &key, int height);

        //
        int RandomHeight();

        bool Equal(const Key &a, const Key &b) const { return (compare_(a, b) == 0); }

        // Return true if key is greater than the data stored in "n"
        bool KeyIsAfterNode(const Key &key, Node *node) const;

        // Return the earliest node that comes at or after key.
        // Return nullptr if there is no such node.
        //
        // If prev is non-null, fills prev[level] with pointer to previous
        // node at "level" for every level in [0..max_height_-1].
        Node *FindGreaterOrEqual(const Key &key, Node **prev) const;

        // Return the earliest node that comes at or after key.
        // Return nullptr if there is no such node.
        //
        // If prev is non-null, fills prev[level] with pointer to previous
        // node at "level" for every level in [0..max_height_-1].
        //
        // the difference between Fast and origin is that it does not need
        // traversal to level 0 iff the target is found before.
        Node *FindGreaterOrEqualFast(const Key &key, Node **prev) const;

        // Return the latest node with a key < key.
        // Return head_ if there is no such node.
        Node *FindLessThan(const Key &key) const;

        // Return the last node in the list.
        // Return head_ if list is empty.
        Node *FindLast() const;
    };

// Implementation details follow
    template<typename Key, class Comparator>
    struct SkipList<Key, Comparator>::Node {
        Node(const Key &k, int height) : key(k) {
            //    resize the next_ array of size height
            next_.resize(height);
        }

          explicit Node() : key(0) {}
        // public key data
        Key const key;

        // Accessors/mutators for links.  n level height,from 0
        std::shared_ptr<Node> Next(int n) {
            // return the next level n of this Node
            assert(n >= 0);
            return next_[n];
        }

        void SetNext(int n, std::shared_ptr<Node> x) {
            // set the next Node of level n
            assert((n >= 0));
            next_[n] = x;
        }

        int GetNodeHeight() const { return next_.size(); }

    private:
        std::vector<std::shared_ptr<Node>> next_;
    };

// Implement your code
    template<typename Key, class Comparator>
    typename SkipList<Key, Comparator>::Node *SkipList<Key, Comparator>::NewNode(
            const Key &key, int height) {
        Node *node = new Node(key, height);
        return node;
    }

    template<typename Key, class Comparator>
    inline SkipList<Key, Comparator>::Iterator::Iterator(const SkipList *list) {
        list_ = list;
        node_ = nullptr;
    }

    template<typename Key, class Comparator>
    inline bool SkipList<Key, Comparator>::Iterator::Valid() const {
        return node_ != nullptr;
    }

    template<typename Key, class Comparator>
    inline const Key &SkipList<Key, Comparator>::Iterator::key() const {
        assert(Valid());
        return node_->key;
    }

    template<typename Key, class Comparator>
    inline void SkipList<Key, Comparator>::Iterator::Next() {
        assert(Valid());
        node_ = node_->Next(0).get();
    }

    template<typename Key, class Comparator>
    inline void SkipList<Key, Comparator>::Iterator::Prev() {
        assert(Valid());
        node_ = list_->FindLessThan(node_->key);
        if (node_ == list_->head_.get()) {
            node_ = nullptr;
        }
    }

    template<typename Key, class Comparator>
    inline void SkipList<Key, Comparator>::Iterator::Seek(const Key &target) {
        node_ = list_->FindGreaterOrEqual(target, nullptr);
    }

    template<typename Key, class Comparator>
    inline void SkipList<Key, Comparator>::Iterator::SeekToFirst() {
        node_ = list_->head_->Next(0).get();
    }

    template<typename Key, class Comparator>
    inline void SkipList<Key, Comparator>::Iterator::SeekToLast() {
        node_ = list_->FindLast();
        if (node_ == list_->head_.get()) {
            node_ = nullptr;
        }
    }

/*
 * implement learned from redis
 */
    template<typename Key, class Comparator>
    int SkipList<Key, Comparator>::RandomHeight() {
//   Increase height with probability 1 in kBranching
//   levelDB 这种做法+的很少，不是概率上的0.25，因为是伪随机数，所以有问题。
//        static const unsigned int kBranching = 4;
//        int height = 1;
//        while (height < kMaxHeight && ((rnd_.Next() % kBranching) == 0)) {
//            height++;
//        }
//        assert(height > 0);
//        assert(height <= kMaxHeight);
//        return height;
        static const float SKIPLIST_P = 0.25;
        // int 4BYTES 32bit
        int height = 1;
        // why 0xFFFF not 0xFF or even 0x1
        while (height < kMaxHeight && (rnd_.Next() & 0xFFFF) < (SKIPLIST_P * 0xFFFF)) {
            height++;
        }
        assert(height > 0);
        assert(height <= kMaxHeight);
        return height;
    }

    template<typename Key, class Comparator>
    bool SkipList<Key, Comparator>::KeyIsAfterNode(const Key &key,
                                                   SkipList::Node *node) const {
        return (node != nullptr) && (compare_(node->key, key) < 0);
    }

    template<typename Key, class Comparator>
    typename SkipList<Key, Comparator>::Node *
    SkipList<Key, Comparator>::FindGreaterOrEqual(const Key &key,
                                                  SkipList::Node **prev) const {
        // new a Node x
        Node *x = head_.get();
        //  Node *x = head_;
        // get the level ,must -1
        int level = GetMaxHeight() - 1;
        while (true) {
            Node *next = x->Next(level).get();
            // forward
            if (KeyIsAfterNode(key, next)) {
                x = next;
            } else {
                // record prev for insert and remove
                if (prev != nullptr) prev[level] = x;
                // next store the ptr to a value equal or larger than key
                if (level == 0) {
                    return next;
                } else {
                    // go down
                    level--;
                }
            }
        }
    }

    template<typename Key, class Comparator>
    typename SkipList<Key, Comparator>::Node *
    SkipList<Key, Comparator>::FindGreaterOrEqualFast(const Key &key,
                                                      SkipList::Node **prev) const {
        Node *x = head_.get();
        int level = GetMaxHeight() - 1;
        while (true) {
            Node *next = x->Next(level).get();
            // forward
            if (KeyIsAfterNode(key, next)) {
                x = next;
            } else {
                // compare_(x->key,key)==0, fast return no need for level ==0
                if (compare_(x->key, key) == 0 || level == 0) {
                    return next;
                } else {
                    level--;
                }
            }
        }
    }

    template<typename Key, class Comparator>
    typename SkipList<Key, Comparator>::Node *
    SkipList<Key, Comparator>::FindLessThan(const Key &key) const {
        Node *x = head_.get();
        int level = GetMaxHeight() - 1;
        while (true) {
            // find less then ,except head_ x must smaller than
            assert(x == head_.get() || compare_(x->key, key) < 0);
            Node *next = x->Next(level).get();
            // forward
            if (KeyIsAfterNode(key, next)) {
                x = next;
            } else {
                // return x
                if (level == 0) {
                    return x;
                } else {
                    level--;
                }
            }
        }
    }

    template<typename Key, class Comparator>
    typename SkipList<Key, Comparator>::Node *SkipList<Key, Comparator>::FindLast()
    const {
        // stack
        Node *x = head_.get();
        int level = GetMaxHeight() - 1;
        while (true) {
            Node *next = x->Next(level).get();
            if (next != nullptr) {
                x = next;
            } else {
                // return x
                if (level == 0) {
                    return x;
                } else {
                    level--;
                }
            }
        }
    }

    template<typename Key, class Comparator>
    SkipList<Key, Comparator>::SkipList(Comparator cmp)
            : compare_(cmp),
              head_(std::move(
                      std::unique_ptr<Node>(NewNode(0 /* any key will do */, kMaxHeight)))),
            //      head_(NewNode(0 /* any key will do */, kMaxHeight)),
            //      max_height_(1 /* current max height */),
              rnd_(0xdeadbeef) {
        heights[1] = 1;  // dummpy heitht for max_height always >= 1

        for (int i = 0; i < kMaxHeight; i++) {
            head_->SetNext(i, nullptr);
        }
    }

    template<typename Key, class Comparator>
    void SkipList<Key, Comparator>::Insert(const Key &key) {
        // for insert which need know the prev node when make op
        Node *prev[kMaxHeight];
        // return  a node >= key,here return is useless.
        FindGreaterOrEqual(key, prev);
        int height = RandomHeight();
        if (height > GetMaxHeight()) {
            for (int i = GetMaxHeight(); i < height; i++) {
                prev[i] = head_.get();
            }
        }
        // update heghts
        if (heights.find(height) == heights.end()) heights[height] = 0;
        heights[height]++;
        // insert
        auto y = std::shared_ptr<Node>(NewNode(key, height));
        for (int i = 0; i < height; i++) {
            y->SetNext(i, prev[i]->Next(i));
            prev[i]->SetNext(i, y);
        }
    }

    template<typename Key, class Comparator>
    bool SkipList<Key, Comparator>::Contains(const Key &key) const {
        //  Node* x = FindGreaterOrEqual(key, nullptr);
        Node *x = FindGreaterOrEqualFast(key, nullptr);
        return x != nullptr && Equal(key, x->key);
    }

    template<typename Key, class Comparator>
    bool SkipList<Key, Comparator>::Remove(const Key &key) {
        Node *prev[kMaxHeight];
        Node *x = FindGreaterOrEqual(key, prev);
        if (x != nullptr && Equal(key, x->key)) {
            int height = x->GetNodeHeight();
            for (int i = 0; i < height; i++) {
                prev[i]->SetNext(i, x->Next(i));
            }
            // maintain heights
            if (--heights[height] == 0) {
                heights.erase(height);
            }
            return true;
        }
        return false;
    }

    template<typename Key, class Comparator>
    void SkipList<Key, Comparator>::Dump() const {
        Node *x = head_.get();

        int level = GetMaxHeight() - 1;
        for (int i = level; i >= 0; i--) {
            // forward all level [0, level]
            Node *next = head_->Next(i).get();
            // print all key in this level
            while (next != nullptr) {
                std::cout << next->key << " -> ";
                next = next->Next(i).get();
            }
            std::cout << "\n";
        }  //
    }
}  // namespace skiplist

#endif  // DSCL_SKIPLIST_H_
