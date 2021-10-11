//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// buffer_pool_manager.cpp
//
// Identification: src/buffer/buffer_pool_manager.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/buffer_pool_manager.h"

#include <list>
#include <unordered_map>

namespace bustub {

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager, LogManager *log_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager), log_manager_(log_manager) {
  // We allocate a consecutive memory space for the buffer pool.
  pages_ = new Page[pool_size_];
  replacer_ = new LRUReplacer(pool_size);

  // Initially, every page is in the free list.
  for (size_t i = 0; i < pool_size_; ++i) {
    free_list_.emplace_back(static_cast<int>(i));
  }
}

BufferPoolManager::~BufferPoolManager() {
  delete[] pages_;
  delete replacer_;
}

Page *BufferPoolManager::FetchPageImpl(page_id_t page_id) {
  // 0
  std::scoped_lock<std::mutex> lock(mutex_);

  // 1.     Search the page table for the requested page (P).
  frame_id_t frame_id;
  // 1.1    If P exists, pin it and return it immediately.
  if (PageTableMap(page_id, &frame_id)) {
    pages_[frame_id].pin_count_++;
    replacer_->Pin(frame_id);
    // LOG_INFO("\tFetch\t%d\t%d", frame_id, page_id);
    return &pages_[frame_id];
  }
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  if (!GetVictimPage(&frame_id)) {
    // LOG_INFO("\tFetch\t\t%d failed", page_id);
    return nullptr;
  }

  // 2.     Delete R from the page table and insert P.

  page_table_.insert({page_id, frame_id});

  // 3.     If R is dirty, write it back to the disk.
  Page *R = &pages_[frame_id];
  if (R->is_dirty_) {
    disk_manager_->WritePage(R->page_id_, R->data_);
    R->is_dirty_ = false;
  }
  page_table_.erase(R->page_id_);
  // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to P.

  Page *P = &pages_[frame_id];
  P->page_id_ = page_id;

  P->pin_count_ = 1;
  replacer_->Pin(frame_id);

  P->is_dirty_ = false;

  disk_manager_->ReadPage(page_id, P->data_);

  // LOG_INFO("\tFetch\t%d\t%d", frame_id, page_id);
  return P;
}

bool BufferPoolManager::UnpinPageImpl(page_id_t page_id, bool is_dirty) {
  std::scoped_lock<std::mutex> lock(mutex_);
  // 1. get frame_id. note: befor unpin, this page must be in page table

  frame_id_t frame_id;
  if (!PageTableMap(page_id, &frame_id)) {
    return false;
  }
  // 2. get P
  Page *P = &pages_[frame_id];

  // 3.1 pin_count --

  if (P->pin_count_ <= 0) {
    return false;
  }

  // 3.2 if p count == 0, add to Replacer
  if (--P->pin_count_ == 0) {
    replacer_->Unpin(frame_id);
  }

  // 4. if is_dirty = flase && is_dirty = true, modify the is_dirty
  P->is_dirty_ = is_dirty || P->is_dirty_;

  // LOG_INFO("\tUnpin\t%d\t%d", frame_id, page_id);
  return true;
}

bool BufferPoolManager::FlushPageImpl(page_id_t page_id) {
  std::scoped_lock<std::mutex> lock(mutex_);
  // Make sure you call DiskManager::WritePage!
  // 1. find in page table
  frame_id_t frame_id;
  if (!PageTableMap(page_id, &frame_id)) {
    return false;
  }

  // 2. Write the Page to disk
  Page *R = &pages_[frame_id];
  disk_manager_->WritePage(page_id, R->data_);
  R->is_dirty_ = false;

  // LOG_INFO("\tFlush\t%d\t%d", frame_id, page_id);
  return true;
}

Page *BufferPoolManager::NewPageImpl(page_id_t *page_id) {
  std::scoped_lock<std::mutex> lock(mutex_);
  // 0.   Make sure you call DiskManager::AllocatePage!

  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  frame_id_t frame_id;
  if (!GetVictimPage(&frame_id)) {
    // LOG_INFO("\tNew\t\t\tfailed");

    return nullptr;
  }

  auto new_page_id = disk_manager_->AllocatePage();

  // ClearVictimPage(frame_id);
  // 2.1 clear the frame, write and remove page table
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  Page *P = &pages_[frame_id];

  if (P->is_dirty_) {
    disk_manager_->WritePage(P->page_id_, P->data_);
    P->is_dirty_ = false;
  }

  page_id_t old_page_id = P->page_id_;

  P->page_id_ = new_page_id;
  P->pin_count_ = 1;
  replacer_->Pin(frame_id);

  // mu_page_table_.WLock();
  page_table_.erase(old_page_id);
  page_table_.insert({new_page_id, frame_id});
  // mu_page_table_.WUnlock();
  // 4.   Set the page ID output parameter. Return a pointer to P.
  *page_id = new_page_id;
  // LOG_INFO("\tNew\t%d\t%d", frame_id, *page_id);
  return P;
}

bool BufferPoolManager::DeletePageImpl(page_id_t page_id) {
  std::scoped_lock<std::mutex> lock(mutex_);
  // 0.   Make sure you call DiskManager::DeallocatePage!
  disk_manager_->DeallocatePage(page_id);
  // 1.   Search the page table for the requested page (P).
  // 1.   If P does not exist, return true.
  frame_id_t frame_id;
  if (!PageTableMap(page_id, &frame_id)) {
    return true;
  }

  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  Page *P = &pages_[frame_id];

  if (P->pin_count_ > 0) {
    return false;
  }

  // 3.   Otherwise, P can be deleted.
  //      Remove P from the page table, reset its metadata and return it to the free list.

  P->page_id_ = INVALID_PAGE_ID;
  P->pin_count_ = 0;

  P->is_dirty_ = false;

  page_table_.erase(page_id);

  replacer_->Pin(frame_id);  // remove it from replacer

  free_list_.emplace_back(frame_id);

  return true;
}

void BufferPoolManager::FlushAllPagesImpl() {
  std::scoped_lock<std::mutex> lock(mutex_);
  for (size_t i = 0; i < pool_size_; i++) {
    if (pages_[i].page_id_ != INVALID_PAGE_ID) {
      disk_manager_->WritePage(pages_[i].page_id_, pages_[i].data_);
      pages_[i].is_dirty_ = false;
    }
  }
}

bool BufferPoolManager::PageTableMap(page_id_t page_id, frame_id_t *frame_id) {
  // mu_page_table_.RLock();

  auto got = page_table_.find(page_id);

  bool found = false;

  if (got != page_table_.end()) {
    // page_id in the buffer pool
    *frame_id = got->second;
    found = true;
  }
  // mu_page_table_.RUnlock();
  return found;
}

bool BufferPoolManager ::GetVictimPage(frame_id_t *frame_id) {
  frame_id_t victim_frame_id;
  // 1. found from the free_list

  if (!free_list_.empty()) {
    victim_frame_id = free_list_.front();
    free_list_.pop_front();

  } else {
    // 2. found from the replacer

    if (!replacer_->Victim(&victim_frame_id)) {
      return false;
    }
  }

  // 3. return
  *frame_id = victim_frame_id;

  return true;
}

}  // namespace bustub
