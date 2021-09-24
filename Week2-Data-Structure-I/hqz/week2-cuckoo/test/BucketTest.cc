#include"bucket.h"
#include <catch2/catch_test_macros.hpp>

using namespace cuckoo;

TEST_CASE( "bucket usage", "[bucket]" ) {
    Bucket b1;
    SECTION( "initialization" ) {
        REQUIRE( b1.getExpo() == 16 );
        std::vector<container> *c = b1.getBucket();
        REQUIRE( c->size() == 1<<16 );
        REQUIRE( (*c)[0]._occupy == 0 );
        REQUIRE( (*c)[0]._fp[0] == 0 );
        REQUIRE( (*c)[0]._fp[1] == 0 );
    }
    SECTION( "functuons" ) {
        unsigned int index[4] = {0, 0, 0, 0};
        REQUIRE( true );
        REQUIRE( index[3] == 0 );
        fingerprint f = 114514;
        REQUIRE( b1.avalable(index, 0) );
        REQUIRE( b1.record(index, 0, f) == S_OK );
        REQUIRE( b1.avalable(index, 0) == false );
        REQUIRE( b1.fetch(index, 0) == 114514 );
        REQUIRE( b1.del(index, 0) == S_OK );
        REQUIRE( b1.avalable(index, 0) );
    }
}