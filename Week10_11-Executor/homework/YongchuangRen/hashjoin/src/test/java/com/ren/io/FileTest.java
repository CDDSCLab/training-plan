package com.ren.io;

import org.junit.Test;

public class FileTest {

    @Test
    public void test01(){
        File file = new File("E:\\研究生\\分布式数据库\\任务\\第七次任务\\impl\\t\\r0.tbl");
        file.readLine();
        String tem;
        while ((tem = file.readLine()) != null) {
            System.out.println(tem);
        }
    }
}
