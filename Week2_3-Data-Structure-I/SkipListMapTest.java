package com.github.dataStructure;

import java.util.Random;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

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
