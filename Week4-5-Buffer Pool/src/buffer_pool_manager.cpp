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
#include <mutex> 

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

//如果空闲链表非空，则不需要进行替换算法。直接返回一个空闲frame。这个情况是buffer pool未满
//如果空闲链表为空，则表示当前buffer pool已经满了，这个时候必须要执行LRU算法

//1 调用前面实现的Victim函数获取牺牲帧的frame id
//2 在pages_中找到对应的牺牲页，如果该页dirty则需要写回磁盘，并且reset pin count
//3 然后在page_table中删除对应映射关系
bool BufferPoolManager::find_replace(frame_id_t *frame_id) {
  // if free_list not empty then we don't need replace page
  // return directly
  if (!free_list_.empty()) {
    *frame_id = free_list_.front();
    free_list_.pop_front();
    return true;
  }
  // else we need to find a replace page
  if (replacer_->Victim(frame_id)) {
      int replace_frame_id = INVALID_PAGE_ID;
      auto it = page_table_.begin();
      while (it != page_table_.end())
      {
        if(it->second == frame_id)
        { 
          replace_frame_id = it->first;
          page_table_.erase(it->first);
          break;
        }
        it++;
      }
  if (replace_frame_id != -1) {
      Page *replace_page = &pages_[*frame_id];
      // If dirty, flush to disk
      if (replace_page->is_dirty_) {
        char *data = replace_page->data_;
        disk_manager_->WritePage(replace_page->page_id_, data);
        replace_page->pin_count_ = 0;  // Reset pin_count
      }
      // 3.     Delete R from the page table and insert P.
      page_table_.erase(replace_page->page_id_);
    }
    return true;
  }
  return false;
}

//get a page
//如果该页在缓冲池中直接访问并且记得把它的pin_count++，然后把调用Pin函数通知replacer
//否则调用find_replace函数，无论缓冲池是否有空闲，都可以获得可用的frame_id
//然后建立新的page_table映射关系
Page *BufferPoolManager::FetchPageImpl(page_id_t page_id) 
{
  std::lock_guard<std::mutex> guard(latch_);
  // 1.     Search the page table for the requested page (P).
  std::unordered_map<page_id_t, frame_id_t>::iterator it = page_table_.find(page_id);
  // 1.1    If P exists, pin it and return it immediately.
  if (it != page_table_.end()) {
    frame_id_t frame_id = it->second;
    Page *page = &pages_[frame_id];
    page->pin_count_++;       
    replacer_->Pin(frame_id);  
    return page;
  }
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  frame_id_t replace_fid;
  if (!find_replace(&replace_fid)) {
    return nullptr;
  }
  Page *replacePage = &pages_[replace_fid];
  // create new map
  // page_id <----> replaceFrameID;
  //  update replacePage info
  disk_manager_->ReadPage(page_id, replacePage->data_);
  replacePage->page_id_ = page_id;
  replacePage->pin_count_++;
  replacePage->is_dirty_ = false;
  replacer_->Pin(replace_fid);
  return replacePage;
}

//1 如果这个页的pin_couter>0我们直接--
//2 如果这个页的pin_couter==0我们需要给它加到Lru_replacer中。因为没有人引用它。所以它可以成为被替换的候选人
bool BufferPoolManager::UnpinPageImpl(page_id_t page_id, bool is_dirty){
  latch_.lock();
  // 1. 如果page_table中没有
  auto iter = page_table_.find(page_id);
  if (iter == page_table_.end()) {
    latch_.unlock();
    return false;
  }
  // 2. 找到要被unpin的page
  frame_id_t unpinned_Fid = iter->second;
  Page *unpinned_page = &pages_[unpinned_Fid];
  if (is_dirty) {
    unpinned_page->is_dirty_ = true;
  }
  // 3. 如果page的pin_count == 0 则直接return
  if (unpinned_page->pin_count_ == 0) {
    latch_.unlock();
    return false;
  }
  unpinned_page->pin_count_--;
  if (unpinned_page->GetPinCount() == 0) {
    replacer_->Unpin(unpinned_Fid);
  }
  latch_.unlock();
  return true;
}

//首先找到这一个页在缓冲池之中的位置
//写入磁盘
bool BufferPoolManager::FlushPageImpl(page_id_t page_id) 
{
  latch_.lock();
  // Make sure you call DiskManager::WritePage!
  auto iter = page_table_.find(page_id);
  if (iter == page_table_.end() || page_id == INVALID_PAGE_ID) {
    latch_.unlock();
    return false;
  }
  frame_id_t flush_fid = iter->second;
  disk_manager_->WritePage(page_id, pages_[flush_fid].data_);
  latch_.unlock();
  return false;
}

//1 利用find_replace函数在我们的缓冲池找到合适的地方建立映射
//2 更新数据
Page *BufferPoolManager::NewPageImpl(page_id_t *page_id) 
{
  latch_.lock();
  // 0.   Make sure you call DiskManager::AllocatePage!
  page_id_t new_page_id = disk_manager_->AllocatePage();
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  bool bool1 = true;
  for (int i = 0; i < static_cast<int>(pool_size_); i++) {
    if (pages_[i].pin_count_ == 0) {
      bool1 = false;
      break;
    }
  }
  if (bool1) {
    latch_.unlock();
    return nullptr;
  }
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  frame_id_t victim_fid;
  if (!find_replace(&victim_fid)) {
    latch_.unlock();
    return nullptr;
  }
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  Page *victim_page = &pages_[victim_fid];
  victim_page->page_id_ = new_page_id;
  victim_page->pin_count_++;
  replacer_->Pin(victim_fid);
  page_table_[new_page_id] = victim_fid;
  victim_page->is_dirty_ = false;
  *page_id = new_page_id;
   disk_manager_->WritePage(victim_page->GetPageId(), victim_page->GetData());
  latch_.unlock();
  return victim_page;
}

//1 如果这个page根本就不在缓冲池则直接返回
//2 如果这个page 的引用计数大于0(pin_counter>0)表示我们不能返回
//3 *如果这个page被修改过则要写回磁盘
//4 否则正常移除就好了。（在hash表中erase）
bool BufferPoolManager::DeletePageImpl(page_id_t page_id) 
{
   latch_.lock();
  // 1.   If P does not exist, return true.
  if (page_table_.find(page_id) == page_table_.end()) {
    latch_.unlock();
    return true;
  }
  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  frame_id_t frame_id = page_table_[page_id];
  Page *page = &pages_[frame_id];
  if (page->pin_count_ > 0)  {
    latch_.unlock();
    return false;
  }
  if (page->is_dirty_) {
    FlushPageImpl(page_id);
  }
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return it to the free list.
  disk_manager_->DeallocatePage(page_id);
  page_table_.erase(page_id);
  // reset
  page->is_dirty_ = false;
  page->pin_count_ = 0;
  page->page_id_ = INVALID_PAGE_ID;
  // return it to the free list
  free_list_.push_back(frame_id);
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
