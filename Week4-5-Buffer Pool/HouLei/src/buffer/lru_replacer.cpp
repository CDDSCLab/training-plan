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

LRUReplacer::LRUReplacer(size_t num_pages):capacity_(num_pages){}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) 
{
    latch_.lock();
    if(lru_map_.empty())
    {
        latch_.unlock();
        return false; 
    }// if lru is empty, return false

    frame_id_t drop_frame = lru_list_.back(); //select the last frame to delete
    lru_map_.erase(drop_frame);
    lru_list_.pop_back();

    *frame_id = drop_frame;//return frame
    latch_.unlock(); 

    return true; 
}

void LRUReplacer::Pin(frame_id_t frame_id) 
{
    latch_.lock();
    if(lru_map_.count(frame_id))  //if frame dont exist, dont need to operate
    {
        lru_list_.erase(lru_map_[frame_id]);
        lru_map_.erase(frame_id);
    }
    latch_.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) 
{
    latch_.lock();
    if(lru_map_.count(frame_id))
    {
        latch_.unlock();    
        return;
    } // ready to pass unpin()*2 test
    if(Size()<capacity_)
    {
        lru_list_.push_front(frame_id);
        lru_map_[frame_id] = lru_list_.begin();
    }
    else
    {
        frame_id_t temp_frame = lru_list_.back();
        lru_list_.pop_back();
        lru_map_.erase(temp_frame);
        lru_list_.push_front(frame_id);
        lru_map_[frame_id] = lru_list_.begin();
    }
    latch_.unlock();
}

size_t LRUReplacer::Size() { return lru_list_.size(); }

}  // namespace bustub
