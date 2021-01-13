package com.ren.hashJoin;

import com.ren.io.File;

import java.util.ArrayList;
import java.util.List;

public class HashJoin {

    private int offsetLeft, offsetRight;
    private File fileLeft, fileRight;
    private List<String> listLeft;
    private Entry[] hashTable;

    private static final int TABLE_NUM = 2500;
    private String line;

    public HashJoin(String pathLeft, String pathRight, int offsetLeft, int offsetRight) {
        this.offsetLeft = offsetLeft;
        this.offsetRight = offsetRight;
        this.fileLeft = new File(pathLeft);
        this.fileRight = new File(pathRight);
        this.listLeft = new ArrayList<String>(500);
        this.hashTable = new Entry[TABLE_NUM];

        hashJoin();
    }

    private void hashJoin(){
        //1. 扫面左表，期间完成hashTable的建立以及使用List存储左表数据
        hashLeft();
        //2. 扫描右表，期间完成hash函数映射，若匹配成功，取出左表List中对应数据，与有表数据结合输出到新文件中
        hashRight();

    }

    private void hashRight(){
        int num = 0;
        String line;
        while ((line = fileRight.readLine()) != null) {
            String[] split = line.split(",");
            int offset = split[offsetRight].hashCode() % TABLE_NUM;

            Entry tem = hashTable[offset];
            while (tem != null) {
                if (tem.getValue().equals(split[offsetRight])) {
//                    System.out.println(listLeft.get(tem.getNum()) + " join " + line);
                    num ++;
                }

                tem = tem.getNext();
            }
        }
//        System.out.println("一共有数据" + num);
    }

    private void hashLeft(){
        short num = 0;
        String line;
        while ((line = fileLeft.readLine()) != null){
            //将左表从外存上保存在一个集合中，用于join时查找
            listLeft.add(line);
            //对左表主键进行hash并存储到对应的hashTable中
            String[] split = line.split(",");
            int offset = split[offsetLeft].hashCode() % TABLE_NUM;
            offset = offset > 0? offset : -offset;
            Entry entry = new Entry(num, split[offsetLeft], null);
            if (hashTable[offset] == null){
                hashTable[offset] = entry;
            }else {
                Entry tem = hashTable[offset];
                while (tem.getNext() != null){
                    tem = tem.getNext();
                }
                tem.setNext(entry);
            }
            num++;
        }

    }



}
