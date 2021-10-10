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
#include <unordered_map>
#include <vector>

#include "buffer/replacer.h"
#include "common/config.h"

namespace bustub {

/**
 * LRUReplacer implements the lru replacement policy, which approximates the Least Recently Used policy.
 */
class LRUReplacer : public Replacer {
 public:
  /**
   * Create a new LRUReplacer.
   * @param num_pages the maximum number of pages the LRUReplacer will be required to store
   *                    要求LRUReplacer存储的最大页数
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
  size_t num_pages_;                // 要求LRUReplacer存储的最大页数 与缓冲池相同
  std::mutex latch_;                // 锁 用于保护多线程访问lruReplacer
  std::list<frame_id_t> lru_list_;  //双向链表 用于执行驱逐策略
  std::unordered_map<frame_id_t, std::list<frame_id_t>::const_iterator>
      lru_map_;  // 哈希表 用于快速定位双向链表指定节点
};

}  // namespace bustub
