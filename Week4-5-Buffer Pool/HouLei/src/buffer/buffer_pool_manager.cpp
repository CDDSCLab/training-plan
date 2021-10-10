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


Page *BufferPoolManager::FetchPageImpl(page_id_t page_id) 
{
  latch_.lock();
  // 1.     Search the page table for the requested page (P).
  std::unordered_map<page_id_t,frame_id_t>::iterator it = page_table_.find(page_id);
  // 1.1    If P exists, pin it and return it immediately.
  frame_id_t frame_id;
  if(it != page_table_.end())
  {
    frame_id = it->second;
    Page *page = &pages_[frame_id];
    page->pin_count_++;
    replacer_->Pin(frame_id);
    latch_.unlock();
    return page;
  }
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  if(!free_list_.empty())
  {
    frame_id = free_list_.front();
    free_list_.pop_front(); 
  }
  else
  {
    frame_id_t *victim_frame = &frame_id;
    if(replacer_->Victim(victim_frame))
    {
      frame_id = *victim_frame;
      auto it = page_table_.begin();
      while (it != page_table_.end())
      {
        if(it->second == frame_id)
        {
          page_table_.erase(it->first);
          break;
        }
        it++;
      }
    }
    else 
    {
      latch_.unlock();
      return nullptr;
    }
  }
  Page *page = &pages_[frame_id];
  // 2.     If R is dirty, write it back to the disk.
  if(page->is_dirty_)
  {
    char *data = page->data_;
    disk_manager_->WritePage(page->page_id_, data);
    page->pin_count_ = 0;  
  }
  // 3.     Delete R from the page table and insert P.
  page_table_.erase(page->page_id_);
  page_table_[page_id] = frame_id;
  // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to P.
  
  disk_manager_->ReadPage(page_id, page->data_);
  page->page_id_ = page_id;
  page->pin_count_++;
  page->is_dirty_ = false;
  replacer_->Pin(frame_id);
  
  latch_.unlock();
  return page; 

}

bool BufferPoolManager::UnpinPageImpl(page_id_t page_id, bool is_dirty)
{
  latch_.lock();
  std::unordered_map<page_id_t,frame_id_t>::iterator it = page_table_.find(page_id);
  if(it == page_table_.end())
  {
    latch_.unlock();
    return false;
  }  //if page dont exist, return
  frame_id_t unpin_frame = it->second;
  Page *unpin_page = &pages_[unpin_frame];
  if(is_dirty)
  {
    unpin_page->is_dirty_ = true;
  }
  if(unpin_page->pin_count_ == 0)
  {
    latch_.unlock();
    return false;
  }
  unpin_page->pin_count_--;
  if(unpin_page->pin_count_ == 0)
  {
    replacer_->Unpin(unpin_frame);
  }
  latch_.unlock();
  return true; 
}

bool BufferPoolManager::FlushPageImpl(page_id_t page_id) 
{
  latch_.lock();
  // Make sure you call DiskManager::WritePage!
  auto it = page_table_.find(page_id);
  if(it == page_table_.end()) 
  {
    latch_.unlock();
    return false;
  }
  frame_id_t flush_frame = it->second;
  disk_manager_->WritePage(page_id, pages_[flush_frame].data_);
  latch_.unlock();
  return true;
}

Page *BufferPoolManager::NewPageImpl(page_id_t *page_id) 
{
  latch_.lock();
  // 0.   Make sure you call DiskManager::AllocatePage!
  page_id_t new_page_id = disk_manager_->AllocatePage();
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  if(replacer_->Size() == 0 && free_list_.empty())
  {
    latch_.unlock();
    return nullptr;
  }
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  frame_id_t replace_frame;
  if(!free_list_.empty())
  {
    replace_frame = free_list_.front();
    free_list_.pop_front();
  }
  else
  {
    frame_id_t *victim_frame = &replace_frame;
    if(replacer_->Victim(victim_frame))
    {
      replace_frame = *victim_frame;
      auto it = page_table_.begin();
      while (it != page_table_.end())
      {
        if(it->second == replace_frame)
        {
          page_table_.erase(it->first);
          break;
        }
        it++;
      }
    }
    else
    {
      latch_.unlock();
      return nullptr;
    }
  }
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  Page *new_page = &pages_[replace_frame];
  if(new_page->is_dirty_)
  {
    char *data = new_page->data_;
    disk_manager_->WritePage(new_page->page_id_, data);
  }

  new_page->page_id_ = new_page_id;
  new_page->pin_count_ = 1;
  replacer_->Pin(replace_frame);
  page_table_[new_page_id] = replace_frame;
  new_page->is_dirty_ = false;
  new_page->ResetMemory();
  disk_manager_->WritePage(new_page_id,new_page->data_);
  // 4.   Set the page ID output parameter. Return a pointer to P.
  *page_id = new_page_id;

  latch_.unlock();
  return new_page;
}

bool BufferPoolManager::DeletePageImpl(page_id_t page_id) 
{
  latch_.lock();
  // 0.   Make sure you call DiskManager::DeallocatePage!
  // 1.   Search the page table for the requested page (P).
  // 1.   If P does not exist, return true.
  auto it = page_table_.find(page_id);
  if(it == page_table_.end())
  {
    latch_.unlock();
    return true;
  }// 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  frame_id_t del_frame = it->second;
  Page *page = &pages_[del_frame];
  if(page->pin_count_>0)
  {
    latch_.unlock();
    return false;
  }
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return it to the free list.
  disk_manager_->DeallocatePage(page_id);
  page_table_.erase(page_id);
  page->ResetMemory();
  page->pin_count_ = 0;
  page->is_dirty_ = false;
  page->page_id_ = INVALID_PAGE_ID;
  free_list_.push_back(del_frame);
  latch_.unlock();
  return true;
}

void BufferPoolManager::FlushAllPagesImpl() 
{
  // You can do it!  // ? just do it
  latch_.lock();
  auto it = page_table_.begin();
  while (it != page_table_.end())
  {
    FlushPageImpl(it->second);
    it++;
  }
  latch_.unlock();
}

}  // namespace bustub
