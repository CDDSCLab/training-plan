package com.ren.io;

import com.ren.hashJoin.HashJoin;
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
//@OperationsPerInvocation
public class HashJoinTest
{
    @Benchmark
    public void test01(){
        String pathLeft = "E:\\研究生\\分布式数据库\\任务\\第七次任务\\impl\\t\\r1.tbl";
        String pathReight = "E:\\研究生\\分布式数据库\\任务\\第七次任务\\impl\\t\\r0.tbl";
        HashJoin hashJoin = new HashJoin(pathLeft, pathReight, 0, 0);
    }

    public static void main(String[] args) throws RunnerException {
        Options opt = new OptionsBuilder()
                .include(HashJoinTest.class.getSimpleName())
                .build();

        new Runner(opt).run();
    }

}
