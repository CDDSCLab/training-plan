#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "filter.h"
#include "hashs.h"

TEST_CASE("bloom_test"){
    SECTION("test different method to construct bloom_filter"){
        int bs_size=32;
        int hash_size=4;
        bloom_filter bf1(bs_size,hash_size);
        bf1.print();

        int n_data=8;
        bloom_filter bf2(n_data);
        bf2.print();

    }

    SECTION("test insert function"){
        int n_data=10;
        bloom_filter bf(n_data);
        REQUIRE(bf.insert("nihao")==1);
        REQUIRE(bf.insert("abcdefg")==1);
        REQUIRE(bf.insert("sdabkfei")==1);
        REQUIRE(bf.insert("123456789")==1);
        REQUIRE(bf.insert("999")==1);
        REQUIRE(bf.insert("123+123")==1);
        REQUIRE(bf.insert("111111111111111111111111111221")==1);
        REQUIRE(bf.insert("")==1);
    }

    SECTION("Test lookup function"){
        int n_data=10;
        bloom_filter bf(n_data);
        bf.insert("nihao");
        bf.insert("abcdefg");
        bf.insert("sdabkfei");
        bf.insert("123456789");
        bf.insert("999");
        bf.insert("123+123");
        bf.print();
        REQUIRE(bf.lookup("nihao")==1);
        REQUIRE(bf.lookup("999")==1);
        REQUIRE(bf.lookup("888")==0);

    }

    SECTION("Test del function"){
        int n_data=10;
        bloom_filter bf(n_data);
        bf.insert("nihao");
        bf.insert("abcdefg");
        bf.insert("sdabkfei");
        bf.insert("123456789");
        bf.insert("999");
        bf.insert("123+123");
        REQUIRE(bf.del("nihao")==-1);
        REQUIRE(bf.del("999")==-1);

    }

    getchar();
}

