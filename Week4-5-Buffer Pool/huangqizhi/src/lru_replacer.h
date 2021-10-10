//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.h
//
// Identification: src/include/buffer/lru_replacer.h
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <list>
#include <mutex>  // NOLINT
#include <vector>
#include <unordered_map>

#include "buffer/replacer.h"
#include "common/config.h"

namespace bustub {

/**
 * LRUReplacer implements the Least Recently Used replacement policy.
 */
class LRUReplacer : public Replacer {
 public:
  /**
   * Create a new LRUReplacer.
   * @param num_pages the maximum number of pages the LRUReplacer will be required to store
   */
  explicit LRUReplacer(size_t num_pages);

  /**
   * Destroys the LRUReplacer.
   */
  ~LRUReplacer() override;

  bool Victim(frame_id_t *frame_id) override;

  void Pin(frame_id_t frame_id) override;

  void Unpin(frame_id_t frame_id) override;

  size_t Size() override;

 private:
  // TODO(student): implement me!
  // All right.
  /**
   * The built-in linked list nodes for LRUReplacer.
   * @param frame_id_ the frame_id of the frame.
   * @param next_ the pointer of the next node.
   */
  struct ReplacerNode {
    frame_id_t frame_id_;
    ReplacerNode *last_;
    ReplacerNode *next_;
    explicit ReplacerNode(frame_id_t id_);
  };
  size_t size_; // the size of the linked list.
  size_t num_pages_;  // the capacity of the replacer.
  std::mutex g_mutex_;  // the lock of the entire class.
  ReplacerNode *head_;  // the head node of the linked list.
  std::unordered_map<frame_id_t, ReplacerNode*> frame_table_;
};

}  // namespace bustub
