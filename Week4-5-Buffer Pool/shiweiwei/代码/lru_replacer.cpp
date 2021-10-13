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

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {
    this->Lru_capacity = num_pages;
}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) { 
    latch.lock();
    frame_id_t f_id = LruList.back();
    if(cache.empty()) {
        latch.unlock();
        frame_id = nullptr;
        return false; 
    }
    *frame_id = f_id;
    LruList.pop_back();
    cache.erase(f_id);
    latch.unlock();
    return true; 
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    latch.lock();
    if(cache.count(frame_id)!=0){
      LruList.erase(cache[frame_id]);
      cache.erase(frame_id);
    }
    latch.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    latch.lock();
    if(cache.find(frame_id) == cache.end()){
      if((int)cache.size()>=Lru_capacity){
        while((int)cache.size()>=Lru_capacity){
          frame_id_t f_id = LruList.back();
          LruList.pop_back();
          cache.erase(f_id);
        }
      }
      LruList.push_front(frame_id);
      cache[frame_id] = LruList.begin();
    }
    latch.unlock();
}

size_t LRUReplacer::Size() { return cache.size(); }

}  // namespace bustub
