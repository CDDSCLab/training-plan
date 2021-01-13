package com.github.dataStructure;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.util.Random;

public class SkipListTest {

    private SkipList skiplist;

    @Before
    public void setUp() throws Exception {
        skiplist = new SkipList();
    }

    @Test
    public void testInsert() throws Exception {
        Assert.assertEquals("Luffy", skiplist.insert(1, "Luffy"));
        Assert.assertEquals("Zoro", skiplist.insert(2, "Zoro"));
        Assert.assertEquals("Nami", skiplist.insert(3, "Nami"));
        Assert.assertEquals("Usopp", skiplist.insert(4, "Usopp"));
        Assert.assertEquals("Sanji", skiplist.insert(5, "Sanji"));
        Assert.assertEquals("Chopper", skiplist.insert(6, "Chopper"));
        Assert.assertEquals("Robin", skiplist.insert(7, "Robin"));
        Assert.assertEquals("Franky", skiplist.insert(8, "Franky"));
        Assert.assertEquals("Brook", skiplist.insert(9, "Brook"));
        Assert.assertEquals("Jinbei", skiplist.insert(10, "Jinbei"));
    }

    @Test
    public void testSearch() throws Exception {
        Assert.assertEquals("Luffy", skiplist.insert(1, "Luffy"));
        Assert.assertEquals("Zoro", skiplist.insert(2, "Zoro"));
        Assert.assertEquals("Nami", skiplist.insert(3, "Nami"));
        Assert.assertEquals("Usopp", skiplist.insert(4, "Usopp"));
        Assert.assertEquals("Sanji", skiplist.insert(5, "Sanji"));
        Assert.assertEquals("Chopper", skiplist.insert(6, "Chopper"));
        Assert.assertEquals("Robin", skiplist.insert(7, "Robin"));
        Assert.assertEquals("Franky", skiplist.insert(8, "Franky"));
        Assert.assertEquals("Brook", skiplist.insert(9, "Brook"));
        Assert.assertEquals("Jinbei", skiplist.insert(10, "Jinbei"));
        Assert.assertEquals(true, skiplist.search(10));
        Assert.assertEquals(false, skiplist.search(11));
    }

    @Test
    public void testErase() throws Exception {
        Assert.assertEquals("Luffy", skiplist.insert(1, "Luffy"));
        Assert.assertEquals("Zoro", skiplist.insert(2, "Zoro"));
        Assert.assertEquals("Nami", skiplist.insert(3, "Nami"));
        Assert.assertEquals("Usopp", skiplist.insert(4, "Usopp"));
        Assert.assertEquals("Sanji", skiplist.insert(5, "Sanji"));
        Assert.assertEquals("Chopper", skiplist.insert(6, "Chopper"));
        Assert.assertEquals("Robin", skiplist.insert(7, "Robin"));
        Assert.assertEquals("Franky", skiplist.insert(8, "Franky"));
        Assert.assertEquals("Brook", skiplist.insert(9, "Brook"));
        Assert.assertEquals("Jinbei", skiplist.insert(10, "Jinbei"));
        Assert.assertEquals("Jinbei", skiplist.erase(10));
        Assert.assertEquals(false, skiplist.search(10));
    }

    @Test
    public void testEffiency() throws Exception {
        long startTime = System.currentTimeMillis();
        Random random = new Random();
        for(int i = 0; i < 100000; i++) {
            skiplist.insert(random.nextInt(100000), random.nextDouble());
        }
        long timeLapse = System.currentTimeMillis() - startTime;
        System.out.println("Time Lapse: " + timeLapse + "ms");
    }
}