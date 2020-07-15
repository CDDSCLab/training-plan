package com.github.dataStructure;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.SortedMap;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class SkipListMapTest {

    private Skiplist skiplist;

    @Before
    public void setUp() throws Exception {
        skiplist = new Skiplist();
    }

    @Test
    public void testAdd() throws Exception {
        skiplist.add(1);
        skiplist.add(2);
        Assert.assertEquals(true, skiplist.search(1));
        Assert.assertEquals(true, skiplist.search(2));
        skiplist.add(3);
        Assert.assertEquals(true, skiplist.search(3));
    }

    @Test
    public void testSearch() throws Exception {
        skiplist.add(111);
        skiplist.add(222);
        Assert.assertEquals(true, skiplist.search(111));
        Assert.assertEquals(true, skiplist.search(222));
    }

    @Test
    public void testErase() throws Exception {
        skiplist.erase(111);
        Assert.assertEquals(false, skiplist.search(111));
        skiplist.erase(222);
        Assert.assertEquals(false, skiplist.search(222));
    }
}