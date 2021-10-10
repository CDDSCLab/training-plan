# bufferpool项目实现过程

## lru算法
### lru算法的简介
如果一个数据在最近一段时间没有被访问到，那么在将来它被访问的可能性也很小。也就是说，当限定的空间已存满数据时，应当把最久没有被访问到的数据淘汰。通过这种算法来进行bufferpool页面的置换。
所有算法需要加锁以防止并发错误。

### victim实现
先看是否有可以牺牲的页，如果没有则返回false； 如果有就选择最近最少使用的页，这里删除需要同时删除链表上和hash上。
```C++
bool LRUReplacer::Victim(frame_id_t *frame_id) {
  // 选择一个牺牲frame
  latch.lock();
  if (lruMap.empty()) {
    latch.unlock();
    return false;
  }

  // 选择列表尾部 也就是最少使用的frame
  frame_id_t lru_frame = lru_list.back();
  lruMap.erase(lru_frame);
  lru_list.pop_back();
  //返回id
  *frame_id = lru_frame;
  latch.unlock();
  return true;
}
```

### pin实现
pin意味着这个frame被进程所引用，不能被victim,将它从数据结构中去掉。
```C++
void LRUReplacer::Pin(frame_id_t frame_id){
  latch.lock();
   // 被引用的frame 不能出现在lru list中
  if (lruMap.count(frame_id) != 0) {
    lru_list.erase(lruMap[frame_id]);
    lruMap.erase(frame_id);
  }

  latch.unlock();
}
```

### unpin实现

将Pin的页面重新加入数据结构，首先得看该页是否在链表中，不存在则需要查看链表现在是否有空闲位置并放入，如果没有则移除尾部节点一直到有。
```C++
void LRUReplacer::Unpin(frame_id_t frame_id) {
  latch.lock();
  if(lru_map.count(frame_id) != 0){
    latch_unlock();
    return;
  }
  // 是否需要移除frame
  while (lru_list.size() >= capacity) {
       frame_id_t frame1 = lru_list.back();
       lru_list.pop_back();
       lru_map.erase(frame1);
      }
  lru_list.push_front(frame_id);
  lru_map[frame_id] = lru_list.begin();
  }
  latch.unlock();
}
```

### size实现
```C++
size_t LRUReplacer::Size() { return lru_list.size(); }
```

### lru算法的测试
执行下面的语句即可
```
cd build
 make lru_replacer_test
 ./test/lru_replacer_test
 ```

## bufferpoolmanager
为了便于梳理代码，集合了一个find_replace函数来实现替换页面的操作。

如果空闲链表非空，则不需要进行替换算法。直接返回一个空闲frame就okay啦。这个情况是buffer pool未满 如果空闲链表为空，则表示当前buffer pool已经满了，这个时候必须要执行LRU算法。

调用前面实现的Victim函数获取牺牲帧的frame id，在pages中找到对应的牺牲页，如果该页时脏页则需要写回磁盘，并且reset pin count，然后在pagetable中删除对应映射关系。
```C++
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
    // Remove entry from page_table
    int replace_frame_id = -1;
    for (const auto &p : page_table_) {
      page_id_t pid = p.first;
      frame_id_t fid = p.second;
      if (fid == *frame_id) {
        replace_frame_id = pid;
        break;
      }
    }
    if (replace_frame_id != -1) {
      Page *replace_page = &pages_[*frame_id];
      // If dirty, flush to disk
      if (replace_page->is_dirty_) {
        char *data = replace_page.data_;
        disk_manager_->WritePage(replace_page->page_id_, data);
        replace_page->pin_count_ = 0;  // Reset pin_count
      }
      // 3.     Delete R from the page table and insert P.
      page_table_.erase(replace_page->page_id_);
      page_table_[page_id] = replace_fid;
    }

    return true;
  }

  return false;
}
```
### FetchPageImpl实现
拿到一个page，如果该页在缓冲池中直接访问并且记得把它的pin_count++，然后把调用Pin函数通知replacer；否则调用find_replace函数，无论缓冲池是否有空闲，都可以获得可用的frame_id；然后建立新的page_table映射关系
```C++
Page *BufferPoolManager::FetchPageImpl(page_id_t page_id)
{
  latch_.lock();
  // 1.     Search the page table for the requested page (P).
  std::unordered_map<page_id_t, frame_id_t>::iterator it = page_table_.find(page_id);
  // 1.1    If P exists, pin it and return it immediately.
  if (it != page_table_.end()) {
    frame_id_t frame_id = it->second;
    Page *page = &pages_[frame_id];
    page->pin_count_++;       
    replacer_->Pin(frame_id);  
    latch_.unlock();
    return page;
  }
  // 1.2    If P does not exist, find a replacement page (R) from either the free list or the replacer.
  //        Note that pages are always found from the free list first.
  frame_id_t replace_fid;
  if (!find_replace(&replace_fid)) {
    latch_.unlock();
    return nullptr;
  }
  Page *replacePage = &pages_[replace_fid];
  // create new map
  // page_id <----> replaceFrameID;
  //  update replacePage info
  Page *newPage = replacePage;
  disk_manager_->ReadPage(page_id, newPage->data_);
  newPage->page_id_ = page_id;
  newPage->pin_count_++;
  newPage->is_dirty_ = false;
  replacer_->Pin(replace_fid);
  latch_.unlock();

  return newPage;

}
```

### UnpinPageImpl实现
进程完成了这个页的操作，需要对它进行unpin，需要考虑pin_counter，如果＞0则直接--，如果==0，可以成为被替换的候选。
```C++
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
```

### FlushPageImpl实现
把一个page写入磁盘
```C++
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
  latch.unlock();
  return false;
}
```

### NewPageImpl实现
分配一个新的page
```C++
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
```
### DeletePageImpl实现
把缓冲池中的page移出
```C++
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
```

### FlushAllPageImpl实现
```C++
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
```
### buffer_pool_manager的测试
执行下面的语句即可
```
cd build
 make buffer_pool_manager_test
 ./test/buffer_pool_manager_test
```
