package com.ren.hashJoin;

import org.junit.Test;
import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;

import java.util.concurrent.TimeUnit;

@BenchmarkMode({Mode.AverageTime, Mode.Throughput})
@Warmup(iterations = 2)
@Measurement(iterations = 2, time = 1)
@OutputTimeUnit(TimeUnit.MICROSECONDS)
@Fork(value = 2)
//@Threads(8)
@State(Scope.Benchmark)
@OperationsPerInvocation
public class HashJoinTest {
//    private String pathA = "E:\\研究生\\分布式数据库\\任务\\第七次任务\\impl\\t\\r1.tbl";
    private String pathA = "E:\\研究生\\分布式数据库\\任务\\第七次任务\\impl\\homework-week4\\r8.tbl";
    private String pathB = "E:\\研究生\\分布式数据库\\任务\\第七次任务\\impl\\homework-week4\\r7.tbl";

    @Benchmark
//    @Test
    public void test01() {
        HashJoin hashJoin = new HashJoin(pathA, pathB,  0, 0);
        hashJoin.HashJoinStart();
    }

    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
                .include(HashJoinTest.class.getSimpleName())
                .build();

        new Runner(opt).run();
    }

}
