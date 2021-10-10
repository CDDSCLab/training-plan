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
  latch_.lock();
  // 1.     Search the page table for the requested page (P).
  auto iter = page_table_.find(page_id);
  // 1.1    If P exists, pin it and return it immediately.
  if (iter != page_table_.end()) {
    Page *page = &pages_[iter->second];
    page->pin_count_++;
    replacer_->Pin(iter->second);
    latch_.unlock();
    return page;
  }
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  frame_id_t replace_fid;
  if (!free_list_.empty()) {
    replace_fid = free_list_.front();
    free_list_.pop_front();
  }
  // 2.     If R is dirty, write it back to the disk.
  if (replacer_->Victim(&replace_fid)) {
    page_id_t replace_pid = INVALID_PAGE_ID;
    for(auto &p : page_table_) {
      if (p.second == replace_fid) {
        replace_pid = p.first;
        break;
      }
    }
    if (replace_pid != INVALID_PAGE_ID) {
      Page *replace_page = &pages_[replace_fid];
      if (replace_page->is_dirty_) {
        disk_manager_->WritePage(replace_pid, replace_page->data_);
        replace_page->pin_count_ = 0;
        replace_page->page_id_ = INVALID_PAGE_ID;
        replace_page->is_dirty_ = false;
      }
      page_table_.erase(replace_pid);
    } else {
      // WTF??
    }
    // avaliable fid got!
  } else {
    latch_.unlock();
    return nullptr;
  }
  // 3.     Delete R from the page table and insert P.
  page_table_[page_id] = replace_fid;
  Page *new_page = &pages_[replace_fid];
  // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to P.
  disk_manager_->ReadPage(page_id, new_page->data_);
  new_page->page_id_ = page_id;
  new_page->pin_count_++;
  replacer_->Pin(replace_fid);
  latch_.unlock();
  return new_page;
}

bool BufferPoolManager::UnpinPageImpl(page_id_t page_id, bool is_dirty) {
  latch_.lock();
  // 1. if page_id not in the page_table
  auto iter = page_table_.find(page_id);
  if (iter == page_table_.end()){
    latch_.unlock();
    return false;
  }
  // 2. get the info of the page
  frame_id_t unpin_fid = page_table_[page_id];
  Page *unpin_page = &pages_[unpin_fid];
  if (is_dirty) {
    unpin_page->is_dirty_ = true;
  }
  // 3. if the pin_count_ already == 0
  if (unpin_page->pin_count_ == 0) {
    latch_.unlock();
    return false;
  }
  // 3. if the pin_count_ >= 1
  unpin_page->pin_count_--;
  if (unpin_page->pin_count_ == 0) {
    replacer_->Unpin(unpin_fid);
  }
  latch_.unlock();
  return true;
}

bool BufferPoolManager::FlushPageImpl(page_id_t page_id) {
  latch_.lock();
  auto iter = page_table_.find(page_id);
  if (iter == page_table_.end() || page_id == INVALID_PAGE_ID) {
    latch_.unlock();
    return false;
  }
  frame_id_t flush_fid = iter->second;
  disk_manager_->WritePage(page_id, pages_[flush_fid].data_);
  latch_.unlock();
  return true;
}

Page *BufferPoolManager::NewPageImpl(page_id_t *page_id) {
  latch_.lock();
  // 0.   Make sure you call AllocatePage!
  page_id_t new_pid = disk_manager_->AllocatePage();
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  if (free_list_.empty() && replacer_->Size() <= 0) {
    latch_.unlock();
    return nullptr;
  }
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  frame_id_t replace_fid;
  if (!free_list_.empty()) {
    replace_fid = free_list_.front();
    free_list_.pop_front();
  }
  if (replacer_->Victim(&replace_fid)) {
    page_id_t replace_pid = INVALID_PAGE_ID;
    for (auto &p : page_table_) {
      if (p.second == replace_fid) {
        replace_pid = p.first;
        break;
      }
    }
    if (replace_pid != INVALID_PAGE_ID) {
      Page *replace_page = &pages_[replace_fid];
      if (replace_page->is_dirty_) {
        disk_manager_->WritePage(replace_pid, replace_page->data_);
        replace_page->pin_count_ = 0;
        replace_page->page_id_ = INVALID_PAGE_ID;
        replace_page->is_dirty_ = false;
      }
      page_table_.erase(replace_pid);
    }
  }
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  page_table_[new_pid] = replace_fid;
  Page *new_page = &pages_[replace_fid];
  memset(new_page->data_, 0, PAGE_SIZE*sizeof(char));
  new_page->page_id_ = new_pid;
  new_page->pin_count_++;
  replacer_->Pin(replace_fid);
  // 4.   Set the page ID output parameter. Return a pointer to P.
  *page_id = new_pid;
  disk_manager_->WritePage(new_pid, new_page->data_);
  latch_.unlock();
  return new_page;
}

bool BufferPoolManager::DeletePageImpl(page_id_t page_id) {
  latch_.lock();
  // 0.   Make sure you call DeallocatePage!
  // 1.   Search the page table for the requested page (P).
  auto iter = page_table_.find(page_id);
  // 1.   If P does not exist, return true.
  if (iter == page_table_.end()) {
    latch_.unlock();
    return true;
  }
  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  frame_id_t del_fid = page_table_[page_id];
  Page *del_page = &pages_[del_fid];
  if (del_page->pin_count_ > 0) {
    latch_.unlock();
    return false;
  }
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return it to the free list.
  if (del_page->is_dirty_) {
    disk_manager_->WritePage(page_id, del_page->data_);
  }
  page_table_.erase(page_id);
  del_page->pin_count_ = 0;
  del_page->is_dirty_ = false;
  del_page->page_id_ = INVALID_PAGE_ID;
  free_list_.push_back(del_fid);
  disk_manager_->DeallocatePage(page_id);
  latch_.unlock();
  return true;
}

void BufferPoolManager::FlushAllPagesImpl() {
  // You can do it!
  latch_.lock();
  for (auto &p : page_table_){
    page_id_t flush_pid = p.first;
    frame_id_t flush_fid = p.second;
    disk_manager_->WritePage(flush_pid, pages_[flush_fid].data_);
  }
  latch_.unlock();
}

}  // namespace bustub
