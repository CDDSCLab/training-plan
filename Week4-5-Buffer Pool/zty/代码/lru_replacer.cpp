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
    this->max_num_pages=num_pages;
}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
    latch.lock();//加锁
    if(LRUmap.empty()){
        latch.unlock();//解锁
        return false;
    }
    frame_id_t victim_id=LRUlist.back();
    LRUlist.pop_back();
    LRUmap.erase(victim_id);
    *frame_id=victim_id;
    latch.unlock();//解锁
    return true;    
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    latch.lock();
    if(LRUmap.count(frame_id)!=0){
        auto it=LRUmap.find(frame_id);
        LRUlist.erase(it->second);
        LRUmap.erase(it);
    }
    
    latch.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    latch.lock();
    if(LRUmap.count(frame_id)!=0){
        //理论上不会出现这种情况
        latch.unlock();
        return;
    }

    if(LRUmap.size()<max_num_pages){
        LRUlist.push_front(frame_id);
        auto it=LRUlist.begin();
        LRUmap.insert({frame_id,it});
        latch.unlock();
    }else{
        //从后面的实现看，这条路径走不到，max_num_pages这个变量貌似没啥用
        // frame_id_t temp;
        // Victim(&temp);
        frame_id_t victim_id=LRUlist.back();
        LRUlist.pop_back();
        LRUmap.erase(victim_id);

        LRUlist.push_front(frame_id);
        auto it=LRUlist.begin();
        LRUmap.insert({frame_id,it});
        latch.unlock();
    }
}

size_t LRUReplacer::Size() {
    latch.lock();
    size_t s=LRUmap.size();
    latch.unlock();
    return s;
}

}  // namespace bustub
