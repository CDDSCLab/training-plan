# Skiplist-java实现   

开发环境：eclipse  

###  Skiplist相关    

- 概念    

  跳表是有序链表。链表的查询和插入的算法复杂度是O(n)，对链表进行优化，可以进行跳跃查找，降低时间复杂度，通过增加结点就可以进行跳跃查找，所以跳表是用了空间换时间的思想。

  跳表会用于redis的有序集合(sorted set)类型。根据不同的需求，会对跳表的结点结构进行整改。所以需要根据实际情况定义跳表的结点结构。

- 1. 特性  

  
  - 由很多层结构组成  
  - 每一层都是一个有序的链表  
  - 最底层的链表包含所有元素  
  - 如果一个元素出现在level i 的链表中，则它在level i之下的链表也都会出现  
  - 每个结点包含4个指针，分别为上下左右指针(自定义)      

###  java实现  

1. 数据结点结构  

   key是关键值，数据。pos是为了方便输出结点。right,left,up,down是为了实现跳表的链接关系。

  ```
   static class Entry {
   
          Entry right;
          Entry left;
          Entry up;
          Entry down;
          int key;//关键值
  		public int pos;//方便输出结点
          
          public Entry(int key) {
              this.key = key;
          }
      }
  ```

2.  跳表结构   

   通过调用SkipList()函数，创建基本的跳表结构——头结点和尾结点

   ```
   public class SkipList {
   
       private int MAX_LEVEL;//当前最高高度 
       private int nodes;//结点总数
       private Entry head;//头结点
       private Entry tail;//尾结点
       private Random random;//生成随机层数用到的概率值 
   
       public SkipList() {
           this.head = new Entry(Integer.MIN_VALUE);
           this.tail = new Entry(Integer.MAX_VALUE);
           head.right = tail;
           tail.left = head;
           this.random = new Random();
           this.nodes = 0;
           this.MAX_LEVEL = 0;
           
       }
   ```

3. 基本操作  

   - 查找  

     先通过查找函数返回离目标值最近的结点，函数从头结点开始查找，如果下一个结点值小于等于目标值，则p往前走一个结点，相反，则往下走一个结点。走到最底层的时候，停止，然后返回p结点。

     ```
        public Entry findEntry(int target) {
         	Entry p;
         	p=head;//从头开始查找  
         	while(true) {
         	  while(p.right.key!=Integer.MAX_VALUE&&p.right.key<=target)//防止p走到尾结点
         		p = p.right;//向前走
         	  if(p.down!=null)
         			p = p.down;
         	  else 
         			break;
         	}
         	return p;
         }
     ```

      在查找函数返回p结点后，再通过搜索函数去查找该跳表中是否有该结点，如果p结点的值等于目标值，则查找成功，反之，则查找不成功。

     ```
     public boolean search(int target) {
             Entry p;
             p=findEntry(target);
             if(p.key==target)
             	{
             	   System.out.println("查找"+target+"成功");
             	   return true;
             	}
             else
             	{
             	   System.out.println("查找"+target+"失败");
             	   return false;
             	}
         }
     ```

   - 插入  

     1. 通过查找函数确认插入位置  
     2. 如果跳表中存在该目标值，则返回true，反之，则进行插入操作，将新结点q插入到最底层  
     3. 最底层插入操作完成后，现在需要决定该结点是否需要上层插入 ，通过随机函数决定，如果随机值小于0.5，则进行上一层的插入，在插入过程中需注意该结点目前所位于的层数是否是最高层，如果是，那就需要新建一层(头尾结点)，将该结点的值插入新的一层中。最后，需要更新跳表长度。

     ```
     public boolean add(int num) {
             Entry p,q;
             int NOW_LEVEL = 0;//新结点的层数
             p = findEntry(num);//查找插入位置
             //如果跳跃表中存在含有num值的结点，则直接返回
             if(p.key==num)
             	return true;
             //如果跳跃表中不存在含num值的结点，则进行新增操作
             q = new Entry(num);
             q.left = p;
             q.right = p.right;
             p.right.left = q;
             p.right = q;
             //抛硬币随机决定是否上层插入  
             while(random.nextDouble()<0.5) {
             	if(NOW_LEVEL==MAX_LEVEL)//已到达最高层，需新增一层
             		addEmptyLevel();
             	while(p.up==null)
             		p = p.left;
             	p = p.up;
             	//注意：除底层结点之外的结点对象不需要value值
             	Entry s;
             	s = new Entry(num);
             	s.left = p;
             	s.right = p.right;
             	s.down = q;
             	p.right.left = s;
             	p.right = s;
             	q.up = s;
             	q = s;//q指向新结点s
             	NOW_LEVEL = NOW_LEVEL + 1;//新结点层数增加 
             }
             nodes = nodes + 1;//更新链表长度 
             System.out.println("第"+num+"个数的高度为"+(NOW_LEVEL+1));
             System.out.println("目前结点总数为"+nodes);
             return true;
             	
             }
     ```

     以下函数为新增一层跳表结构的函数，注意：最后需要更新跳表高度

     ```
     private void addEmptyLevel() {
         	Entry p1,p2;
         	p1 = new Entry(Integer.MIN_VALUE);
             p2 = new Entry(Integer.MAX_VALUE);
             p1.right = p2;
             p1.down = head;
             p2.left = p1;
             p2.down = tail;
             head.up = p1;
             tail.up = p2;
             head = p1;
             tail = p2;
             MAX_LEVEL = MAX_LEVEL + 1;
             
         }
     ```

   - 删除  

     首先，通过查找函数找到p结点，如果p结点的值不等于目标结点的值，则返回false；否则，将p结点一层一层的删除。

     ```
      public boolean erase(int num) {
             Entry p,q;
             p = findEntry(num);
             if(p.key!=num) //不存在目标结点
                 return false;
             while(p!=null) {
             	q = p.up;
             	p.left.right = p.right;
             	p.right.left = p.left;
             	p = q;
             }
             	
             	return true;
             	
             }
     ```

   - 打印

     为打印跳表做准备，最后打印出整个跳表结构  

     ```
      public void printHorizontal()
     	  {
     	     String s = "";
     	     int i;
        	     Entry p;
     
     	    //将p移动到底层
     	     p = head;
     
     	     while ( p.down != null )
     	     {
     	        p = p.down;
     	     }
             //给底层每个结点编号，方便打印空白的结点
     	     i = 0;
     	     while ( p != null )
     	     {
     	        p.pos = i++;
     	        p = p.right;
     	     }
     
     	    //打印
     	     p = head;
     
     	     while ( p != null )
     	     {
     	        s = getOneRow( p );
     		    System.out.println(s);
     
     	        p = p.down;
     	     }
     	  }
     ```

     打印每一层的结点

     ```
      public String getOneRow(Entry p )
     	  {
     	     String s;
     	     int a, b, i;
     
     	     a = 0;
             //输出第一个结点的值
     	     s = "" + p.key;
     	     
     	     p = p.right;
     
     
     	     while ( p != null )
     	     {
     	        Entry q;
     	        q = p;
     	        while (q.down != null) 
     	        {
     		        q = q.down;
     	        }
     	        b = q.pos;
     
     	        s = s + " <-";
     
                 //输出结点p前空白结点
     	        for (i = a+1; i < b; i++)
     	           s = s + "--------";
     	        //输出p结点的值
     	        s = s + "> " + p.key;
     
     	        a = b;
     
     	        p = p.right;
     	     }
     
     	     return(s);
     	  }
     ```

   - 测试  

     ```
     public class SkipListMapTest {
     
         private SkipList skiplist;
         Random r = new Random();
     
         @Before
         public void setUp() throws Exception {
             skiplist = new SkipList();
         }
     
         @Test
         public void testAdd() throws Exception {
         	//插入10个随机数
         	for (int i = 0; i < 10; i++ )
     	      {
     			int tmp = r.nextInt(100);
     			skiplist.add(tmp);
     			skiplist.printHorizontal();
     	      }    
         }
     
      @Test
         public void testSearch() throws Exception {
          skiplist.add(11);
             skiplist.printHorizontal();
             Assert.assertEquals(true, skiplist.search(11));
         }
     
         @Test
         public void testErase() throws Exception {
             skiplist.erase(11);
             Assert.assertEquals(false, skiplist.search(11));
             skiplist.printHorizontal();
         }
     }
     ```
   
   - 运行结果  
   
     ```
     第4个数的高度为4
     目前结点总数为7
     -2147483648 <-> 4 <-----------------------------------------> 60 <-> 2147483647
     -2147483648 <-> 4 <-----------------------------------------> 60 <-> 2147483647
     -2147483648 <-> 4 <-> 6 <-> 11 <---------> 46 <---------> 60 <-> 2147483647
     -2147483648 <-> 4 <-> 6 <-> 11 <-> 41 <-> 46 <-> 54 <-> 60 <-> 2147483647
     第32个数的高度为1
     目前结点总数为8
     -2147483648 <-> 4 <-------------------------------------------------> 60 <-> 2147483647
     -2147483648 <-> 4 <-------------------------------------------------> 60 <-> 2147483647
     -2147483648 <-> 4 <-> 6 <-> 11 <-----------------> 46 <---------> 60 <-> 2147483647
     -2147483648 <-> 4 <-> 6 <-> 11 <-> 32 <-> 41 <-> 46 <-> 54 <-> 60 <-> 2147483647
     第0个数的高度为1
     目前结点总数为9
     -2147483648 <---------> 4 <-------------------------------------------------> 60 <-> 2147483647
     -2147483648 <---------> 4 <-------------------------------------------------> 60 <-> 2147483647
     -2147483648 <---------> 4 <-> 6 <-> 11 <-----------------> 46 <---------> 60 <-> 2147483647
     -2147483648 <-> 0 <-> 4 <-> 6 <-> 11 <-> 32 <-> 41 <-> 46 <-> 54 <-> 60 <-> 2147483647
     第97个数的高度为1
     目前结点总数为10
     -2147483648 <---------> 4 <-------------------------------------------------> 60 <---------> 2147483647
     -2147483648 <---------> 4 <-------------------------------------------------> 60 <---------> 2147483647
     -2147483648 <---------> 4 <-> 6 <-> 11 <-----------------> 46 <---------> 60 <---------> 2147483647
     -2147483648 <-> 0 <-> 4 <-> 6 <-> 11 <-> 32 <-> 41 <-> 46 <-> 54 <-> 60 <-> 97 <-> 2147483647
     -2147483648 <---------> 4 <-------------------------------------------------> 60 <---------> 2147483647
     -2147483648 <---------> 4 <-------------------------------------------------> 60 <---------> 2147483647
     -2147483648 <---------> 4 <-> 6 <-> 11 <-----------------> 46 <---------> 60 <---------> 2147483647
     -2147483648 <-> 0 <-> 4 <-> 6 <-> 11 <-> 32 <-> 41 <-> 46 <-> 54 <-> 60 <-> 97 <-> 2147483647
     查找11成功
     查找11失败
     -2147483648 <---------> 4 <-----------------------------------------> 60 <---------> 2147483647
     -2147483648 <---------> 4 <-----------------------------------------> 60 <---------> 2147483647
     -2147483648 <---------> 4 <-> 6 <-----------------> 46 <---------> 60 <---------> 2147483647
     -2147483648 <-> 0 <-> 4 <-> 6 <-> 32 <-> 41 <-> 46 <-> 54 <-> 60 <-> 97 <-> 2147483647
     ```