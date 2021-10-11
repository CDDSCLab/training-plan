# 代码实现

## 一、LRU Replacer实现
### 1.数据结构设计
采用链表+hash表。引入hash表就是可以根据frame\_id快速找到其在list中对应的位置，从而降低时间复杂度。

	std::mutex latch;
	std::list<frame_id_t> LruList;
	int Lru_capacity;
	std::unordered_map<frame_id_t,std::list<frame_id_t>::iterator> cache;
### 2.Victim函数实现
（1）如果没有可以替换的页，返回false；  
（2）如果有，选择链表尾部的页并移除，然后删除该页在hash表中的对应映射关系。返回true。

	bool LRUReplacer::Victim(frame_id_t *frame_id) { 
	    latch.lock();
	    frame_id_t f_id = LruList.back();
	    if(cache.empty()) {
	        latch.unlock();
	        frame_id = nullptr;
	        return false; 
	    }
	    *frame_id = f_id;
	    LruList.pop_back();
	    cache.erase(f_id);
	    latch.unlock();
	    return true; 
	}
### 3.pin函数实现
pin函数表示某个页正在被程序进行引用，此时该页不能被LRU算法换出，所以需要将该页从LRU算法的替换链表中删除，同时删除的还有hash表中对应的映射关系。

	void LRUReplacer::Pin(frame_id_t frame_id) {
	    latch.lock();
	    if(cache.count(frame_id)!=0){
	      LruList.erase(cache[frame_id]);
	      cache.erase(frame_id);
	    }
	    latch.unlock();
	}
### 4.unpin函数实现
unpin函数表示某个页被程序引用完毕，此时该页又可以被LRU算法换出，所以需要将该页重新插入LRU算法的替换链表中。  
（1）先看该页是否在替换链表中；  
（2）如果不在，还需要检查替换链表的空间是否足够；  
（3）如果空间足够，直接插入链表的头部，同时在hash表中添加映射关系；  
（4）如果空间不够，需要移除链表的尾部节点，直到空间足够。

	void LRUReplacer::Unpin(frame_id_t frame_id) {
	    latch.lock();
	    if(cache.find(frame_id) == cache.end()){
	      if((int)cache.size()>=Lru_capacity){
	        while((int)cache.size()>=Lru_capacity){
	          frame_id_t f_id = LruList.back();
	          LruList.pop_back();
	          cache.erase(f_id);
	        }
	      }
	      LruList.push_front(frame_id);
	      cache[frame_id] = LruList.begin();
	    }
	    latch.unlock();
	}

## Buffer Pool实现
### 1.参数说明
* pool_size_：在缓冲池中页的个数
* *pages_：页对象，是缓冲池中页的数组
* *disk_manager_：指向磁盘管理
* *log_manager_：指向日志管理
* page_table_：容器页表用来追踪缓冲池中的页，<page_id,frame_id>
* *replacer_：替换器
* free_list_：有空页的链表
### 2.函数说明
* FetchPageImpl：从缓冲池得到需要的页  
1.在页表中找到需要的页  
（1）如果找到，则将该页进行pin，并返回该页  
（2）如果找不到，则需要将该页插入缓冲池  
2.将该页插入缓冲池  
a.先从空闲链表中找到一个空闲帧存储该页  
b.如果空闲链表为空，需要从替换器中找到可以替换的页  
  如果替换器为空，说明没有可以替换的帧  
  否则找到可替换页面所在的帧id，同时将替换掉的页从page\_table中删除，同时如果该页是脏页，需将其写回磁盘  
3.更新该页在缓冲池存储位置页的元数据：page\_id\_、pin\_count\_、is\_dirty\_  
4.从磁盘中将该页写到缓冲池中，返回该页

* UnpinPageImpl：从缓冲池中将目标页unpin  
1.如果从页表中找不到该页，返回false  
2.如果该页的pin\_count\_大于0，则只需将pin\_count\_减1  
3.如果该页的pin\_count\_等于0，则需要将该页进行unpin  
4.根据该页本身的脏页位和该函数的脏页位设置该页的脏页位  

* FlushPageImpl：将目标页刷新到磁盘（目标页成为脏页后）  
如果能在页表中找到该页，则将该页写回磁盘，同时将该页的脏页位设为false

* NewPageImpl：在缓冲池中创造一个新的页  
1.获取新的页号
2.将该页插入缓冲池  
a.先从空闲链表中找到一个空闲帧存储该页  
b.如果空闲链表为空，需要从替换器中找到可以替换的页  
  如果替换器为空，说明没有可以替换的帧  
  否则找到可替换页面所在的帧id，同时将替换掉的页从page\_table中删除，同时如果该页是脏页，需将其写回磁盘  
3.更新该页在缓冲池存储位置页的元数据：page\_id\_、pin\_count\_、is\_dirty\_ 

* DeletePageImpl：从缓冲池中删除一个页  
1.释放磁盘上该页面的内容  
2.在页表中找到需要的页，如果找不到，直接返回true    
3.如果该页的pin\_count\_大于0，说明还有程序正在引用该页，此时不能将该页删除  
4.如果该页为脏页，需要将该页写回磁盘  
5.否则，将该页从页表和替换器中删除，同时将该页所在的帧的page\_id\_、is\_dirty\_、pin\_count\_进行设置，同时将该帧插入空闲链表

* FlushAllPagesImpl：将缓冲池所有的页刷新到磁盘  
