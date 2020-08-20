package com.github.dataStructure;

import java.util.Random;

public class SkipList {

    /**
     * 每一个结点的内容
     */
   static class Entry {
        Entry right;
        Entry left;
        Entry up;
        Entry down;
        int key;
		public int pos;//方便输出结点
        
        public Entry(int key) {
            this.key = key;
        }
    }
    /**
     * 跳表结构
     */
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

    /**
     * 根据一个target去查找跳表中是否含有这个值
     */
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
    /**
     * 查找离目标结点最近的结点
     */
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
    /**
     * 将一个元素加入跳表中
     */
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
    //新增一层
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

    /**
     * 将跳表中的某一个元素删除
     */
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
    /**
     * 打印跳表
     */
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
      //输出每一层
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
        	
    }

	
    
    
