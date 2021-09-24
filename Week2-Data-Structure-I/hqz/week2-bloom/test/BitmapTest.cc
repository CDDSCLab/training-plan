#include "bitmap.h"
#include <catch2/catch_test_macros.hpp>
using namespace bloomfilter;

TEST_CASE( "bitmap usage", "[bitmap]" ) {
    
    SECTION( "initialization" ) {
        Bitmap b1;
        Bitmap b2(16);
        Bitmap b3(-1);
        Bitmap b4(128);
        int expo = 20;
        unsigned long s = 1 << 15;
        REQUIRE( b1.getExpo() == expo );
        REQUIRE( b1.getBit().size() == s );
        expo = 16;
        s = 1 << 11;
        REQUIRE( b2.getExpo() == expo );
        REQUIRE( b2.getBit().size() == s );
        expo = 20;
        s = 1 << 15;
        REQUIRE( b3.getExpo() == expo );
        REQUIRE( b3.getBit().size() == s );
        expo = 20;
        s = 1 << 15;
        REQUIRE( b4.getExpo() == expo );
        REQUIRE( b4.getBit().size() == s );
    }
    SECTION("fuctions") {
        // only two int at _bit
        unsigned int test[4];
        test[0] = 0x12345678;
        test[1] = 0x90ABCDEF;
        test[2] = 0x12345678;
        test[3] = 0x87654321;
        Bitmap b5(6);
        b5.bit_set(test);
        // 100001 = 33; i = 1, j = 1
        REQUIRE( b5.getBit()[1] == 2);
        bool t = b5.bit_query(test);
        REQUIRE( t == true );
        b5.bit_reset(test);
        REQUIRE( b5.getBit()[0] == 0);
        REQUIRE( b5.getBit()[1] == 0);
    }
}