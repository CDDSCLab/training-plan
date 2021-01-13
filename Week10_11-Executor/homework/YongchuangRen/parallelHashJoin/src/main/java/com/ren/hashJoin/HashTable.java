package com.ren.hashJoin;


import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class HashTable {

    private int offsetLeft, offsetRight;
    private List<String> listLeft, listRight;
    private Entry[] hashTable;
    private ExecutorService executorService = Executors.newCachedThreadPool();
    private CountDownLatch countDownLatch = new CountDownLatch(2);

    private static final int TABLE_NUM = 2 << 9;

    public HashTable(List listLeft, List listRight, int offsetLeft, int offsetRight) {
        this.offsetLeft = offsetLeft;
        this.offsetRight = offsetRight;
        this.listLeft = listLeft;
        this.listRight = listRight;
        this.hashTable = new Entry[TABLE_NUM];

        hashJoin();
    }

    private void hashJoin(){
        //1. 扫面左表，期间完成hashTable的建立以及使用List存储左表数据
        hashLeft();
        //2. 扫描右表，期间完成hash函数映射，若匹配成功，取出左表List中对应数据，与有表数据结合输出到新文件中
        hashRight();
    }

    private void hashRight() {
        int center = listRight.size() / 2;

        executorService.execute(() -> {
            for (int i = 0; i < center; i++){
                String right = listRight.get(i);
                String[] split = right.split(",");
                int offset = split[offsetRight].hashCode() % TABLE_NUM;
                offset = offset > 0? offset : -offset;
                Entry tem = hashTable[offset];
                while (tem != null) {
                    if (tem.getValue().equals(split[offsetRight])) {
//                        System.out.println(listLeft.get(tem.getNum()) + " join " + right);
                    }
                    tem = tem.getNext();
                }
            }
            countDownLatch.countDown();
        });

        executorService.execute(() -> {
            for (int i = center; i < listRight.size(); i++){
                String right = listRight.get(i);
                String[] split = right.split(",");
                int offset = split[offsetRight].hashCode() % TABLE_NUM;
                offset = offset > 0? offset : -offset;
                Entry tem = hashTable[offset];
                while (tem != null) {
                    if (tem.getValue().equals(split[offsetRight])) {
//                        System.out.println(listLeft.get(tem.getNum()) + " join " + right);
                    }
                    tem = tem.getNext();
                }
            }
            countDownLatch.countDown();
        });

        try {
            countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        executorService.shutdown();

        /**
        for (String right : listRight) {
            String[] split = right.split(",");
            int offset = split[offsetRight].hashCode() % TABLE_NUM;
            Entry tem = hashTable[offset];
            while (tem != null) {
                if (tem.getValue().equals(split[offsetRight])) {
                    System.out.println(listLeft.get(tem.getNum()) + " join " + right);
                }
                tem = tem.getNext();
            }
        }
         **/
    }

    private void hashLeft() {
        short num = 0;
        for (String left : listLeft) {
            String[] split = left.split(",");
            int offset = split[offsetLeft].hashCode() % TABLE_NUM;
            offset = offset > 0? offset : -offset;
            Entry entry = new Entry(num, split[offsetLeft], null);

            if (hashTable[offset] == null) {
                hashTable[offset] = entry;
            }else {
                Entry tem = hashTable[offset];
                while (tem.getNext() != null) {
                    tem = tem.getNext();
                }
                tem.setNext(entry);
            }
            num++;
        }
    }





}
