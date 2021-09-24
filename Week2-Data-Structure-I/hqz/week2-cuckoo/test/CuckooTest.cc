#include <catch2/catch_test_macros.hpp>
#include "cuckoo.h"
using namespace cuckoo;

TEST_CASE( "cuckoo filter usage", "[cuckoo]" ) {
    Cuckoofilter cf1;
    SECTION( "initialization" ) {
        REQUIRE( cf1.getExpo() == 16 );
        REQUIRE( cf1.getNumkick() == 16 );
        REQUIRE( cf1.getBucket().getBucket()->size() == 1<<16 );
    }
    SECTION( "functions" ) {
        REQUIRE( cf1.insert("aaa") == S_OK );
        REQUIRE( cf1.insert("bbb") == S_OK );
        REQUIRE( cf1.lookup("aaa") );
        REQUIRE( cf1.lookup("bbb") );
        REQUIRE( cf1.lookup("ccc") == false );
        REQUIRE( cf1.del("bbb") == S_OK );
        REQUIRE( cf1.del("ccc") == E_NOT_FOUND );
        REQUIRE( cf1.lookup("bbb") == false );
        REQUIRE( cf1.insert("aaa") == S_OK );
        REQUIRE( cf1.insert("aaa") == E_OUTOFKICK );
    }
}