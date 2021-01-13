//
// Created by IcePig on 2020-07-27.
//

#include <vector>
#include "filter_policy.h"
#include <string>
#include "gtest/gtest.h"
#include "util/coding.h"
#include "assert.h"

static const int kVerbose = 1;

static Slice Key(int i, char *buffer) {
    EncodeFixed32(buffer, i);
    return Slice(buffer, sizeof(uint32_t));
}
#define TEST_GOOGLE 0
#define MY_TEST_BLOOM 1
#define MY_TEST_COUNTBLOOM 1
#define TEST_NUM 10000

int main(int argc, char **argv) {
#if MY_TEST_BLOOM


    {
        const FilterPolicy *bloom_filter = NewBloomFilterPolicy(10, 100);
        const std::string keys[2] = {"hello", "world"};
        bloom_filter->AddKey(keys, 2);
        std::cout << bloom_filter->KeyMayMatch(keys[0]) << std::endl;
        std::cout << bloom_filter->KeyMayMatch(keys[1]) << std::endl;
        std::cout << bloom_filter->KeyMayMatch("ufo exists?") << std::endl;
        std::cout << bloom_filter->KeyMayMatch("nullptr") << std::endl;
        delete bloom_filter;
    }

    {
        const FilterPolicy *bloom_filter = NewBloomFilterPolicy(10, TEST_NUM);
        std::vector<std::string> keys;
        for (int i = 0; i < TEST_NUM; ++i) {
            keys.push_back(std::string(i, 'a'));
        }
        bloom_filter->AddKey(&keys[0], int(keys.size()));

        int fail_count = 0;
        for (int i = 0; i < TEST_NUM; ++i) {
            if (bloom_filter->KeyMayMatch(std::string(i, 'b'))) {
                fail_count++;
            }
        }

        std::cout << "try " << TEST_NUM << "times, fail:" << fail_count << std::endl;
        delete bloom_filter;
    }

    {
        const FilterPolicy *bloom_filter = NewBloomFilterPolicy(10, 10000);
        const std::string keys[3] = {"a", "b", "c"};
        bloom_filter->AddKey(keys, 3);
//        bloom_filter->DumpData();
        delete bloom_filter;
    }


    return 0;
#endif
#if MY_TEST_COUNTBLOOM
    const FilterPolicy *count_bloom_filter = NewCountBloomFilterPolicy(10, 100);
    const std::string keys[2] = {"hello", "world"};
    std::cout << "-----------------add-----------------" << std::endl;
    count_bloom_filter->AddKey(keys, 2);
    count_bloom_filter->AddKey(keys, 2);
    std::cout << "-----------------match-----------------" << std::endl;
    assert(count_bloom_filter->KeyMayMatch(keys[0]));
    assert(count_bloom_filter->KeyMayMatch(keys[1]));
    std::cout << "-----------------delete-----------------" << std::endl;
    assert(count_bloom_filter->DeleteKey(keys[0]));
    assert(count_bloom_filter->DeleteKey(keys[1]));
    std::cout << "-----------------match-----------------" << std::endl;
    assert(count_bloom_filter->KeyMayMatch(keys[0]));
    assert(count_bloom_filter->KeyMayMatch(keys[1]));
    std::cout << "-----------------delete-----------------" << std::endl;
    assert(count_bloom_filter->DeleteKey(keys[0]));
    assert(count_bloom_filter->DeleteKey(keys[1]));
    std::cout << "-----------------match-----------------" << std::endl;
    assert(!count_bloom_filter->KeyMayMatch(keys[0]));
    assert(!count_bloom_filter->KeyMayMatch(keys[1]));
#endif
#if TEST_GOOGLE
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
}