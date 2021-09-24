#include "bloomfilter.h"
#include <catch2/catch_test_macros.hpp>
using namespace bloomfilter;

TEST_CASE( "bloomfilter usage", "[bloomfilter]" ) {
    Bloomfilter bf1;
    Bloomfilter bf2(16);
    SECTION( "initialization" ) {
        int expo = 20;
        REQUIRE( bf1.getExpo() == expo);
        REQUIRE( bf1.getBitmap()->getExpo() == expo);
        expo = 16;
        REQUIRE( bf2.getExpo() == expo);
        REQUIRE( bf2.getBitmap()->getExpo() == expo);
    }
    SECTION( "add & check" ) {
        vector<std::string> test_set1 = 
        {"Emma", "Olivia", "Ava", "Sophia", "Isabella", "Mia",
         "Charlotte", "Abigail", "Emily", "Harper", "Amelia", "Scarlett",
         "Evelyn", "Elizabeth", "Sofia", "Madison", "Avery", "Ella"};
        vector<std::string> test_set2 = 
        {"Noah", "Liam", "Mason", "Jacob", "William", "Ethan", "Aiden",
         "James", "Alexander", "Michael", "Benjamin", "Elijah", "Daniel"};
        for(auto &test_case : test_set1){
            REQUIRE( bf1.add(test_case) == S_OK );
        }
        for(auto &test_case : test_set1){
            REQUIRE( bf1.isContain(test_case) );
        }
        for(auto &test_case : test_set2){
            REQUIRE( !bf2.isContain(test_case) );
        }
    }
}