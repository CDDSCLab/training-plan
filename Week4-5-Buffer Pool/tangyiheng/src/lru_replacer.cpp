//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"
#include <iostream>

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) : num_pages_(num_pages) { lru_map_.reserve(num_pages_); }

LRUReplacer::~LRUReplacer() = default;

/**
 * 该方法驱逐最近最少访问的页面，从从lruReplacer中删除中删除，
 * 将页面内容存储到输出参数中返回true，lruReplacer为空返回false
 *
 * @param frame_id
 * @return
 */
bool LRUReplacer::Victim(frame_id_t *frame_id) {
  std::lock_guard<std::mutex> lock(latch_);
  //
  if (Size() == 0) {  // lruReplacer 为空
    return false;
  }
  assert(Size() > 0);
  *frame_id = lru_list_.front();      // 保存驱逐出去的内存帧
  lru_map_.erase(lru_list_.front());  // 删除map
  lru_list_.pop_front();              // 删除list
  return true;
}

/**
 * 该方法应该在缓冲池管理器固定一个页面到内存帧后调用(表示该页面正在被使用中，无法参与驱逐)
 * 它应该删除lruReplacer中指定的内存帧
 *
 * @param frame_id
 */
void LRUReplacer::Pin(frame_id_t frame_id) {
  std::lock_guard<std::mutex> lock(latch_);
  //
  //  lru_list_.remove(frame_id); // 通过遍历做的删除 O(n) 效率低
  // 存在lru中
  if (lru_map_.find(frame_id) != lru_map_.end()) {  // 通过map找到指定内存帧 O(1) 效率高
    lru_list_.erase(lru_map_[frame_id]);            // 删除list
    lru_map_.erase(frame_id);                       // 删除map
  }
}

/**
 * 该方法应该在pin_count恢复为0后调用(表示该页面没在使用，可以参与驱逐)
 * 它应该将指定内存帧添加到lruReplacer
 *
 * @param frame_id
 */
void LRUReplacer::Unpin(frame_id_t frame_id) {
  std::lock_guard<std::mutex> lock(latch_);
  // 不存在lru中
  if (lru_map_.find(frame_id) == lru_map_.end()) {
    lru_list_.push_back(frame_id);  // 加入list
    std::list<frame_id_t>::const_iterator it = lru_list_.end();
    it--;
    lru_map_[frame_id] = it;  // 加入map
  }
}

/**
 * 该方法返回当前在lruReplacer可供驱逐的页面数量
 *
 * @return
 */
size_t LRUReplacer::Size() { return lru_list_.size(); }

}  // namespace bustub
