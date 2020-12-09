package com.ren.hashJoin;

import com.ren.io.FileReader;

import javax.sound.midi.Soundbank;
import java.sql.SQLOutput;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class HashJoin {

    private int offsetA, offsetB;
    private FileReader fileA, fileB;
    private List<String> listA00, listA01, listA02, listA03;
    private List<String> listB00, listB01, listB02, listB03;

    private ExecutorService executorService = Executors.newCachedThreadPool();
    private static CyclicBarrier barrierIO = new CyclicBarrier(2);
//    private CyclicBarrier barrierHashJoin = new CyclicBarrier(4);
    private static CountDownLatch countDownLatch = new CountDownLatch(4);

    //每张表划分的集合数
    private final int LIST_NUM = 4;
    //每个集合初始化大小
    private final int CAPACITY = 2 << 7;

    /**
     * 初始化参数
     */
    public HashJoin(String pathA, String pathB, int offsetA, int offsetB) {

        this.offsetA = offsetA;
        this.offsetB = offsetB;
        this.fileA = new FileReader(pathA);
        this.fileB = new FileReader(pathB);

        this.listA00 = new ArrayList<String>(CAPACITY);
        this.listA01 = new ArrayList<String>(CAPACITY);
        this.listA02 = new ArrayList<String>(CAPACITY);
        this.listA03 = new ArrayList<String>(CAPACITY);
        this.listB00 = new ArrayList<String>(CAPACITY);
        this.listB01 = new ArrayList<String>(CAPACITY);
        this.listB02 = new ArrayList<String>(CAPACITY);
        this.listB03 = new ArrayList<String>(CAPACITY);
    }


    public void HashJoinStart(){
        //1.对两个表进行扫描载入内存，同时分区并比较大小
        executorService.execute(new Runnable() {
            @Override
            public void run() {
                AIOSplit();
                try {
                    barrierIO.await();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (BrokenBarrierException e) {
                    e.printStackTrace();
                }
            }
        });
        BIOSplit();
        try {
            barrierIO.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (BrokenBarrierException e) {
            e.printStackTrace();
        }

        fileA.close();
        fileB.close();

        //2.进行并发hashJoin，需要比较大小表
        executorService.execute(new Runnable() {
            @Override
            public void run() {
                if (listA00.size() < listB00.size()){
                    new HashTable(listA00, listB00, offsetA, offsetB);
                }else {
                    new HashTable(listB00, listA00, offsetB, offsetA);
                }
                countDownLatch.countDown();
            }
        });

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                if (listA01.size() < listB01.size()){
                    new HashTable(listA01, listB01, offsetA, offsetB);
                }else {
                    new HashTable(listB01, listA01, offsetB, offsetA);
                }
                countDownLatch.countDown();
            }
        });

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                if (listA02.size() < listB02.size()){
                    new HashTable(listA02, listB02, offsetA, offsetB);
                }else {
                    new HashTable(listB02, listA02, offsetB, offsetA);
                }
                countDownLatch.countDown();
            }
        });

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                if (listA03.size() < listB03.size()){
                    new HashTable(listA03, listB03, offsetA, offsetB);
                }else {
                    new HashTable(listB03, listA03, offsetB, offsetA);
                }
                countDownLatch.countDown();
            }
        });

        try {
            countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        executorService.shutdown();

    }

    //对A文件进行分区
    private void AIOSplit(){
        String line = fileA.readLine();
        while (line != null){
            String[] split = line.split(",");
            int result = Integer.parseInt(split[offsetA]);
            switch (result % LIST_NUM){
                case 0:
                    listA00.add(line);
                    break;
                case 1:
                    listA01.add(line);
                    break;
                case 2:
                    listA02.add(line);
                    break;
                case 3:
                    listA03.add(line);
                    break;
            }

            line = fileA.readLine();
        }
    }

    //对文件进行分区
    private void BIOSplit(){
        String line = fileB.readLine();
        while (line != null){
            String[] split = line.split(",");
            int result = Integer.parseInt(split[offsetB]);
            switch (result % LIST_NUM){
                case 0:
                    listB00.add(line);
                    break;
                case 1:
                    listB01.add(line);
                    break;
                case 2:
                    listB02.add(line);
                    break;
                case 3:
                    listB03.add(line);
                    break;
            }
            line = fileB.readLine();
        }
    }






}
