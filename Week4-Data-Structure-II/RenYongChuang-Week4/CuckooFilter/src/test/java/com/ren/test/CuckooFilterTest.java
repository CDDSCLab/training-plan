package com.ren.test;

import com.ren.cuckooFilter.CuckooFilter;
import com.ren.cuckooFilter.Keys;
import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;

public class CuckooFilterTest {

    private CuckooFilter cuckooFilter;

    @Before
    public void start(){
        cuckooFilter = new CuckooFilter(1000, 2, 100);
    }

    @Test
    public void cuckooFilterTest(){
        System.out.println(cuckooFilter.lookUp("wang"));
        cuckooFilter.addKey("wang");
        System.out.println(cuckooFilter.lookUp("wang"));
        cuckooFilter.delete("wang");
        System.out.println(cuckooFilter.lookUp("wang"));
    }
}
