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
#include <mutex>
#include <list>
#include <cstddef>

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {

}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  // 选择一个牺牲frame
  latch.lock();
  if (lru_map.empty()) {
    latch.unlock();
    return false;
  }
  frame_id_t lru_frame = lru_list.back();
  lru_map.erase(lru_frame);
  lru_list.pop_back();
  *frame_id = lru_frame;
  latch.unlock();
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id){
  latch.lock();
  if (lru_map.count(frame_id) != 0) {
    lru_list.erase(lru_map[frame_id]);
    lru_map.erase(frame_id);
  }
  latch.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  latch.lock();
  if(lru_map.count(frame_id) != 0){
    return;
  }
  // 是否需要移除frame
  while (lru_list.size() >= capacity) {
       frame_id_t frame1 = lru_list.back();
       lru_list.pop_back();
       lru_map.erase(frame1);
      }
  lru_list.push_front(frame_id);
  lru_map[frame_id] = lru_list.begin();
  latch.unlock();
}

size_t LRUReplacer::Size() {return lru_list.size(); }


} // namespace bustub
