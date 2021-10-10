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
  // 1.     Search the page table for the requested page (P).
  std::lock_guard<std::mutex> lock(latch_);

  auto it = page_table_.find(page_id);
  Page *pp=nullptr;
  frame_id_t free_frame=-1;
  // 1.1    If P exists, pin it and return it immediately.

  if(it!=page_table_.end()){
    //已存在于缓冲池中，则直接调用
    pp=pages_+ (it->second);
    //基地址(pages_) + 偏移地址(frame_id)
    pp->pin_count_++;
    //引用计数加一
    if(pp->pin_count_==1){
      replacer_->Pin(it->second);
    }
    //如果之前在LRU中，则将其从LRU中消除
    return pp;
  }

  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  if(!free_list_.empty()){
    //如果还有空闲帧没有使用，则使用空闲帧
    free_frame=free_list_.front();
    pp=pages_+free_frame;
    free_list_.pop_front();
    pp->page_id_=page_id;
    pp->ResetMemory();
    page_table_.insert({page_id,free_frame});
    disk_manager_->ReadPage(page_id,pp->GetData());
    pp->pin_count_++;
    
  }else{
    //没有空闲帧，就要使用LRU替换一个已经使用的帧，帧的旧页如果为脏页，则需要先将旧页写回磁盘，再将新页装入这个帧
    if(!replacer_->Victim(&free_frame)) return nullptr;
    //缓冲池中所有帧都有线程在使用
    pp=pages_+free_frame;
    if(pp->IsDirty()){
      // 2.     If R is dirty, write it back to the disk.
      disk_manager_->WritePage(pp->GetPageId(),pp->GetData());
    }
    pp->is_dirty_=false;
    pp->pin_count_=0;
    pp->ResetMemory();

    page_table_.erase(pp->GetPageId());
    // 3.     Delete R from the page table and insert P.
    page_table_.insert({page_id,free_frame});
    pp->page_id_=page_id;
    // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to P.
    pp->pin_count_++;
    disk_manager_->ReadPage(page_id,pp->GetData());
  }

  return pp;
}

bool BufferPoolManager::UnpinPageImpl(page_id_t page_id, bool is_dirty) {
  std::lock_guard<std::mutex> lock(latch_);
  auto it=page_table_.find(page_id);
  if(it==page_table_.end())return false;
  Page *pp=pages_+ it->second;
  pp->pin_count_--;
  if(is_dirty && !pp->is_dirty_)pp->is_dirty_=true;
  if(pp->pin_count_==0){
    replacer_->Unpin(it->second);
  }
  return true; 
}

bool BufferPoolManager::FlushPageImpl(page_id_t page_id) {
  // Make sure you call DiskManager::WritePage!
  std::lock_guard<std::mutex> lock(latch_);
  if(page_id==INVALID_PAGE_ID){
    return false;
  }
  auto it=page_table_.find(page_id);
  if(it==page_table_.end()){
    return false;
  }
  Page *pp=pages_+ it->second;
  disk_manager_->WritePage(page_id, pp->GetData());
  pp->is_dirty_=false;
  return true;
}

Page *BufferPoolManager::NewPageImpl(page_id_t *page_id) {
  // 0.   Make sure you call DiskManager::AllocatePage!
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  // 4.   Set the page ID output parameter. Return a pointer to P.
  std::lock_guard<std::mutex> lock(latch_);
  page_id_t new_page_id=disk_manager_->AllocatePage();
  *page_id=new_page_id;

  Page *pp=nullptr;
  frame_id_t free_frame=-1;
  if(!free_list_.empty()){
    free_frame=free_list_.front();
    free_list_.pop_front();
    pp=pages_+free_frame;
  }else if(replacer_->Victim(&free_frame)){
    pp=pages_+free_frame;
    if(pp->IsDirty()){
      disk_manager_->WritePage(pp->GetPageId(),pp->GetData());
      pp->is_dirty_=false;
    }
    pp->pin_count_=0;
    pp->ResetMemory();
    page_table_.erase(pp->GetPageId());
  }else{
    return nullptr;
  }
  
  pp->page_id_=new_page_id;
  pp->pin_count_++;
  page_table_.insert({pp->GetPageId(),free_frame});

  return pp;
}

bool BufferPoolManager::DeletePageImpl(page_id_t page_id) {
  // 0.   Make sure you call DiskManager::DeallocatePage!
  // 1.   Search the page table for the requested page (P).
  // 1.   If P does not exist, return true.
  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return it to the free list.
  std::lock_guard<std::mutex> lock(latch_);
  disk_manager_->DeallocatePage(page_id);
  auto it=page_table_.find(page_id);
  if(it==page_table_.end()){
    return true;
  }
  Page *pp=pages_ + it->second;
  if(pp->pin_count_!=0){
    return false;
  }
  
  pp->is_dirty_=false;
  pp->page_id_=INVALID_PAGE_ID;
  pp->ResetMemory();
  replacer_->Pin(it->second);
  //从LRU中清除
  free_list_.push_back(it->second);
  //将这个帧插入空闲链表中
  page_table_.erase(page_id);
  //从页表中删除
  return true;
}

void BufferPoolManager::FlushAllPagesImpl() {
  std::lock_guard<std::mutex> lock(latch_);
  Page *pp=nullptr;

  for(auto &frame:page_table_){
    pp=pages_+frame.second;
    disk_manager_->WritePage(pp->GetPageId(), pp->GetData());
    pp->is_dirty_=false;
  }
}

}  // namespace bustub
