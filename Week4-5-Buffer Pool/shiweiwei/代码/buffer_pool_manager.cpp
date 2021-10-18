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

//初始化新页面的信息
void BufferPoolManager::InitNewPage(frame_id_t frame_id, page_id_t page_id) {
  pages_[frame_id].ResetMemory(); //清除内存
  pages_[frame_id].page_id_ = page_id;
  pages_[frame_id].pin_count_ = 1;
  pages_[frame_id].is_dirty_ = false;
}

//先考虑freelist，在考虑从replacer找一个page来替换
frame_id_t BufferPoolManager::FindReplace() {
  frame_id_t replace_id = -1;
  if (!free_list_.empty()) {
    replace_id = free_list_.front();
    free_list_.pop_front();
  } else if (replacer_->Size() > 0) {
    replacer_->Victim(&replace_id); //找到可替换页面所在的帧id
    page_table_.erase(pages_[replace_id].GetPageId()); //将替换掉的数据从page_table中删除
    //如果该数据是脏数据，则将其写回磁盘
    if (pages_[replace_id].IsDirty()) {
      disk_manager_->WritePage(pages_[replace_id].GetPageId(), pages_[replace_id].GetData());
    }
  }
  return replace_id;
}

Page *BufferPoolManager::FetchPageImpl(page_id_t page_id) {
  std::lock_guard<std::mutex> lock(latch_);
  // 1.     Search the page table for the requested page (P).
  // 1.1    If P exists, pin it and return it immediately.
  if(page_table_.find(page_id)!=page_table_.end()){
    auto frame_id = page_table_[page_id];
    pages_[frame_id].pin_count_++;
    if(pages_[frame_id].GetPinCount()==1){
      replacer_->Pin(frame_id);
    }
    return &pages_[frame_id];
  }
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  // 2.     If R is dirty, write it back to the disk.
  // 3.     Delete R from the page table and insert P.
  if(free_list_.empty()&&replacer_->Size()==0) {
    return nullptr;
  }
  auto frame_id = FindReplace();
  // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to P.
  page_table_.insert({page_id,frame_id});
  //page_table_[page_id] = frame_id;
  InitNewPage(frame_id,page_id);
  disk_manager_->ReadPage(page_id,pages_[frame_id].GetData());
  return &pages_[frame_id];
}

bool BufferPoolManager::UnpinPageImpl(page_id_t page_id, bool is_dirty) { 
  std::lock_guard<std::mutex> lock(latch_);
  if(page_table_.find(page_id) == page_table_.end()){
    return false;
  }
  auto frame_id = page_table_[page_id];
  if(pages_[frame_id].pin_count_<0){
    return false;
  }
  if(pages_[frame_id].pin_count_>0){
    pages_[frame_id].pin_count_--;
  }
  if(pages_[frame_id].pin_count_==0){
    replacer_->Unpin(frame_id);
  }
  pages_[frame_id].is_dirty_ = pages_[frame_id].IsDirty() || is_dirty;
  return true;
}

bool BufferPoolManager::FlushPageImpl(page_id_t page_id) {
  std::lock_guard<std::mutex> lock(latch_);
  // Make sure you call DiskManager::WritePage!
  if(page_id==INVALID_PAGE_ID){
    return false;
  }
  if(page_table_.find(page_id)==page_table_.end()){
    return false;
  }
  auto frame_id = page_table_[page_id];
  disk_manager_->WritePage(page_id, pages_[frame_id].GetData());
  pages_[frame_id].is_dirty_ = false;
  return true;
}

Page *BufferPoolManager::NewPageImpl(page_id_t *page_id) {
  std::lock_guard<std::mutex> lock(latch_);
  // 0.   Make sure you call DiskManager::AllocatePage! 
  auto new_page_id = disk_manager_->AllocatePage();
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  if(free_list_.empty()&&replacer_->Size()==0){
    return nullptr;
  }
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  auto frame_id = FindReplace();
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  page_table_.insert({new_page_id,frame_id});
  //page_table_[new_page_id] = frame_id; //添加到页表
  InitNewPage(frame_id,new_page_id); //初始化页的信息
  // 4.   Set the page ID output parameter. Return a pointer to P.
  *page_id = new_page_id;
  return &pages_[frame_id];
}

bool BufferPoolManager::DeletePageImpl(page_id_t page_id) {
  std::lock_guard<std::mutex> lock(latch_);
  // 0.   Make sure you call DiskManager::DeallocatePage!
  disk_manager_->DeallocatePage(page_id);
  // 1.   Search the page table for the requested page (P).
  // 1.   If P does not exist, return true.
  if(page_table_.find(page_id)==page_table_.end()) { 
    return true;
  }
  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  auto frame_id = page_table_[page_id];
  if(pages_[frame_id].GetPinCount()>0){
    return false;
  }
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return it to the free list.
  //if(pages_[frame_id].is_dirty_){
  //  FlushPageImpl(page_id);
  //}
  page_table_.erase(page_id);
  pages_[frame_id].ResetMemory();
  pages_[frame_id].is_dirty_ = false;
  pages_[frame_id].page_id_ = INVALID_PAGE_ID;
  pages_[frame_id].pin_count_ = 0;
  replacer_->Pin(frame_id);
  free_list_.push_front(frame_id);
  return true;
}

void BufferPoolManager::FlushAllPagesImpl() {
  // You can do it!
  std::lock_guard<std::mutex> lock(latch_);
  for(size_t i=0;i<pool_size_;i++){
    FlushPageImpl(pages_[i].page_id_);
  }
}

}  // namespace bustub
