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

LRUReplacer::LRUReplacer(size_t num_pages)
: size_(0)
, num_pages_(num_pages)
, head_(new ReplacerNode(INVALID_PAGE_ID))
{}

LRUReplacer::~LRUReplacer() {
    delete head_;
    head_ = nullptr;
}

bool LRUReplacer::Victim(frame_id_t *frame_id) {
    g_mutex_.lock();
    if (this->size_ == 0) {
        g_mutex_.unlock();
        return false;
    }
    ReplacerNode *p = head_->last_;
    ReplacerNode *q = p->last_;
    q->next_ = head_;
    head_->last_ = q;
    *frame_id = p->frame_id_;
    frame_table_.erase(p->frame_id_);
    delete p;
    p = nullptr;
    this->size_--;
    g_mutex_.unlock();
    return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    g_mutex_.lock();
    if (frame_table_.count(frame_id) > 0) {
        ReplacerNode *p = frame_table_[frame_id];
        ReplacerNode *q = p->last_;
        ReplacerNode *r = p->next_;
        q->next_ = r;
        r->last_ = q;
        frame_table_.erase(p->frame_id_);
        delete p;
        p = nullptr;
        this->size_--;
    } else {
        // do nothing
    }
    g_mutex_.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    g_mutex_.lock();
    if (frame_table_.count(frame_id) == 0) {
        ReplacerNode *p = new ReplacerNode(frame_id);
        ReplacerNode *q = head_->next_;
        p->last_ = head_;
        p->next_ = q;
        head_->next_ = p;
        q->last_ = p;
        this->size_++;
        frame_table_[frame_id] = p;
    } else {
        // do nothing
    }
    g_mutex_.unlock();
}

size_t LRUReplacer::Size() {
    size_t s = 0;
    g_mutex_.lock();
    s = this->size_;
    g_mutex_.unlock();
    return s;
}

LRUReplacer::ReplacerNode::ReplacerNode(frame_id_t id_)
: frame_id_(id_)
, last_(this)
, next_(this)
{}

}  // namespace bustub
