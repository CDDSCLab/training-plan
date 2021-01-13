package com.github.dataStructure;

import java.util.Random;

public class Skiplist {

    /**
     * 每一个节点的内容
     */
    static class Entry {
        Entry[] next;
        int val;
        int count;  //表示一个值出现次数
        int level;  //表示层级

        public Entry(int val, int level) {
            this.val = val;
            this.count = 1;
            this.level = level;
            this.next = new Entry[level];
        }
    }

    private static final int MAX_LEVEL = 64;

    private Entry head;

    private Random random;

    public Skiplist() {
        this.head = new Entry(Integer.MIN_VALUE, MAX_LEVEL);
        this.random = new Random();
    }

    /**
     * 根据一个target去查找跳表中是否含有这个值
     * @param target
     * @return
     */
    public boolean search(int target) {
        //todo--->
    }

    /**
     * 将一个元素加入跳表中
     * @param num
     */
    public void add(int num) {
        //todo--->
    }

    /**
     * 将跳表中的某一个元素删除
     * @param num
     * @return
     */
    public boolean erase(int num) {
        //todo--->
    }

    private Entry newEntry(int num) {
        int level = randomLevel();
        return new Entry(num, level);
    }

    /**
     * 随机生成一个 level
     * @return
     */
    private int randomLevel() {
        //todo--->
    }
}