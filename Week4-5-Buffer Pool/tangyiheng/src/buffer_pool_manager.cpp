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

/**
 *  该方法从缓冲池中获取一个页面
 *
 * @param page_id
 * @return
 */
Page *BufferPoolManager::FetchPageImpl(page_id_t page_id) {
  // 1.     Search the page table for the requested page (P).
  // 1.1    If P exists, pin it and return it immediately.
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  // 2.     If R is dirty, write it back to the disk.
  // 3.     Delete R from the page table and insert P.
  // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to P.
  std::lock_guard<std::mutex> lock(latch_);
  Page *page = nullptr;  // 目标返回页面
  // 1 在页表中搜索请求的页面
  auto map_iterator = page_table_.find(page_id);
  // 1.1 如果页面在页表中 说明页面在缓冲池 固定页面后直接返回
  if (map_iterator != page_table_.end()) {
    frame_id_t frame_id = map_iterator->second;
    page = pages_ + frame_id;
    page->pin_count_++;        // pin
    replacer_->Pin(frame_id);  // pin
    return page;
  }
  // 1.2 如果页面不在页表中
  // 先尝试从空闲链表中获取空闲内存帧
  if (!free_list_.empty()) {
    frame_id_t frame_id = free_list_.front();
    free_list_.pop_front();           // 更新空闲链表
    page_table_[page_id] = frame_id;  // 更新页表
    // 构造页面
    page = pages_ + frame_id;
    page->page_id_ = page_id;
    page->pin_count_ = 1;  // 置1表示刚从磁盘读取的页面加载到内存中 首次固定
    page->is_dirty_ = false;
    page->ResetMemory();                            // 将页面内容清空
    disk_manager_->ReadPage(page_id, page->data_);  // 从磁盘读取页面
    return page;
  }
  // 空闲链表无空闲内存帧 说明缓冲池已满 需要驱逐页面
  frame_id_t frame_id;
  if (!replacer_->Victim(&frame_id)) {  // 驱逐页面 返回被驱逐的内存帧
    return nullptr;
  }
  // 2. 如果被驱逐页面为脏 需要写回磁盘
  Page *page_evicted = pages_ + frame_id;
  if (page_evicted->is_dirty_) {
    disk_manager_->WritePage(page_evicted->page_id_, page_evicted->data_);
  }
  // 3. 从页表中删除被驱逐页面 将新页面并加入页表
  page_table_.erase(page_evicted->page_id_);
  page_table_[page_id] = frame_id;
  // 4. 更新 新页面的元信息 从磁盘读取其数据
  page = pages_ + frame_id;
  page->page_id_ = page_id;
  page->pin_count_ = 1;
  page->is_dirty_ = false;
  page->ResetMemory();
  disk_manager_->ReadPage(page_id, page->data_);
  return page;
}

/**
 * 该方法将页面取消固定
 *
 * @param page_id
 * @param is_dirty
 * @return
 */
bool BufferPoolManager::UnpinPageImpl(page_id_t page_id, bool is_dirty) {
  std::lock_guard<std::mutex> lock(latch_);
  // 判断页面是否在缓冲池中
  if (page_table_.find(page_id) == page_table_.end()) {  // 不在缓冲池中
    return true;
  }
  // 在缓冲池中
  frame_id_t frame_id = page_table_[page_id];
  Page *page = pages_ + frame_id;
  if (page->pin_count_ == 0) {
    return true;
  } else if (page->pin_count_ < 0) {
    return false;
  }
  // 更新pin计数
  page->pin_count_--;
  page->is_dirty_ = is_dirty;
  // 更新置换器
  if (page->pin_count_ == 0) {
    replacer_->Unpin(frame_id);
  }
  return true;
}

/**
 * 该方法将页面刷回磁盘
 *
 * @param page_id
 * @return
 */
bool BufferPoolManager::FlushPageImpl(page_id_t page_id) {
  // Make sure you call DiskManager::WritePage!
  std::lock_guard<std::mutex> lock(latch_);
  // 判断页面是否存在缓冲池中
  auto map_iterator = page_table_.find(page_id);
  if (map_iterator == page_table_.end()) {  // 不存在
    return false;
  }
  Page *page = pages_ + map_iterator->second;
  if (page->page_id_ == INVALID_PAGE_ID) {  // 无效页面
    return false;
  }
  // 在缓冲池中 但处于固定状态
  if (page->pin_count_ > 0) {
    return false;
  }
  // 有效且存在缓冲池中的页面
  // 检查页面是否为脏 只将脏页写回磁盘
  if (page->is_dirty_) {
    disk_manager_->WritePage(page_id, page->data_);
    page->is_dirty_ = false;
  }
  return true;
}

/**
 * 该方法在磁盘上创建一个新页面 并将其加载到缓冲池中
 *
 * @param page_id
 * @return
 */
Page *BufferPoolManager::NewPageImpl(page_id_t *page_id) {
  // 0.   Make sure you call DiskManager::AllocatePage!
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the free list first.
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  // 4.   Set the page ID output parameter. Return a pointer to P.
  std::lock_guard<std::mutex> lock(latch_);
  // 1. 如果缓冲中已满且所有页面都被固定，则无法创建页面
  if (free_list_.empty() && replacer_->Size() == 0) {  // 内存中无未使用的帧 以及 可置换的页面
    return nullptr;
  }
  // 0. 在磁盘上分配页面（实际上是给页面ID做自增）
  *page_id = disk_manager_->AllocatePage();
  Page *page = nullptr;
  // 2. 先从空闲链表中获取未使用的内存帧
  if (!free_list_.empty()) {
    frame_id_t frame_id = free_list_.front();
    free_list_.pop_front();            // 更新空闲链表
    page_table_[*page_id] = frame_id;  // 更新页表
    // 构造页面
    page = pages_ + frame_id;
    page->page_id_ = *page_id;
    page->pin_count_ = 1;  // 置1表示刚从磁盘读取的页面加载到内存中 首次固定
    page->is_dirty_ = false;
    page->ResetMemory();  // 将页面内容清空
    //    disk_manager_->ReadPage(page->page_id_, page->data_);  // 是否需要从磁盘读取页面？
    return page;
  }
  // 2. 若无空闲内存帧则从置换器中驱逐页面
  frame_id_t frame_id;
  assert(replacer_->Victim(&frame_id));
  Page *page_evicted = pages_ + frame_id;
  if (page_evicted->is_dirty_) {  // 将脏页刷回磁盘
    disk_manager_->WritePage(page_evicted->page_id_, page_evicted->data_);
  }
  // 3. 更新 新页面的元信息 清空页面数据 将新页面加入页表
  page_table_.erase(page_evicted->page_id_);
  page_table_[*page_id] = frame_id;
  page = pages_ + frame_id;
  page->page_id_ = *page_id;
  page->pin_count_ = 1;
  page->is_dirty_ = false;
  page->ResetMemory();
  // 4. 返回页面
  return page;
}

/**
 * 该方法从缓冲池中删除一个页面
 *
 * @param page_id
 * @return
 */
bool BufferPoolManager::DeletePageImpl(page_id_t page_id) {
  // 0.   Make sure you call DiskManager::DeallocatePage!
  // 1.   Search the page table for the requested page (P).
  // 1.   If P does not exist, return true.
  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return it to the free list.
  std::lock_guard<std::mutex> lock(latch_);
  // 1 通过页表搜索请求的页面
  auto map_iterator = page_table_.find(page_id);
  if (map_iterator == page_table_.end()) {
    // 1. 如果页面不在缓冲池中 直接返回true
    return true;
  }
  frame_id_t frame_id = map_iterator->second;
  Page *page = pages_ + frame_id;
  // 2. 如果页面存在 但处于固定状态 表明页面正在使用中 返回false
  if (page->pin_count_ > 0) {
    return false;
  }
  //  if (page->is_dirty_) {
  //    disk_manager_->WritePage(page->page_id_, page->data_);
  //  }
  // 3. 将目标页面从页表中删除 重置其元数据 将其返回到空闲链表中
  disk_manager_->DeallocatePage(page_id);
  replacer_->Pin(frame_id);          // 更新置换器
  page_table_.erase(page_id);        // 更新页表
  page->page_id_ = INVALID_PAGE_ID;  // 更新页面元信息
  page->pin_count_ = 0;
  page->is_dirty_ = false;
  page->ResetMemory();
  free_list_.push_back(frame_id);  // 更新空闲链表
  return false;
}

/**
 * 该方法将缓冲池中的所有页面写回磁盘
 *
 */
void BufferPoolManager::FlushAllPagesImpl() {
  // You can do it!
  std::lock_guard<std::mutex> lock(latch_);
  // 遍历缓冲池页面数组 挨个调用flushPage
  for (size_t i = 0; i < pool_size_; ++i) {
    FlushPageImpl(pages_[i].page_id_);
  }
}

void BufferPoolManager::PrintOut() {
  for (size_t i = 0; i < pool_size_; ++i) {
    std::cout << "page: {frame_id:" << i << ", page_id:" << pages_[i].page_id_ << ", pin_count:" << pages_[i].pin_count_
              << std::endl;
  }
}

}  // namespace bustub
