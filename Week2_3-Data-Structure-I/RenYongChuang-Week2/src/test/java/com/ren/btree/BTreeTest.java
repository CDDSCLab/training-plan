package com.ren.btree;

import org.junit.Before;
import org.junit.Test;

public class BTreeTest {

    private BTree bTree;

    @Before
    public void setUp(){
        bTree = BTree.newInstance();
    }

    @Test
    public void add(){
        Entry entry1 = new Entry(1, "赵");
        Entry entry2 = new Entry(2, "钱");
        Entry entry3 = new Entry(3, "孙");
        Entry entry4 = new Entry(4, "李");
        Entry entry5 = new Entry(5, "周");
        Entry entry6 = new Entry(6, "吴");
        Entry entry7 = new Entry(7, "郑");
        Entry entry8 = new Entry(8, "王");
        Entry entry9 = new Entry(9, "任");
        Entry entry10 = new Entry(10, "田");
        bTree.insertKey(entry1);
        bTree.insertKey(entry2);
        bTree.insertKey(entry3);
        bTree.insertKey(entry4);
        bTree.insertKey(entry5);
        bTree.insertKey(entry6);
        bTree.insertKey(entry7);
        bTree.insertKey(entry8);
        bTree.insertKey(entry9);
        bTree.insertKey(entry10);
        System.out.println("打印B树：" + bTree.printTree(bTree));
        System.out.println(bTree.getTree(bTree));
    }

    @Test
    public void search(){
        for(int i = 1 ; i <= 10; i++)
        {
            Entry entry = new Entry(i, "i=" + i);
            bTree.insertKey(entry);
        }
        System.out.println(bTree.getTree(bTree));
        //找一个存在的数据
        System.out.println(bTree.findKey(1));
        //找一个不存在的数据
        System.out.println(bTree.findKey(12));
    }

    @Test
    public void delete(){
        for(int i = 1 ; i <= 10; i++)
        {
            Entry entry = new Entry(i, "i=" + i);
            bTree.insertKey(entry);
        }
        System.out.println(bTree.getTree(bTree));
        bTree.removeKey(8);
        System.out.println("删除了8：" + "\n" + bTree.getTree(bTree));
        bTree.removeKey(6);
        System.out.println("删除了6：" + "\n" + bTree.getTree(bTree));
        System.out.println("打印B树：" + bTree.printTree(bTree));
    }

}
