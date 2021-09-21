/**
 * https://github.com/weberr13/Murmur
 */

#pragma once
#include "stdint.h"
#include <vector>
class MurMurHash {
public:
   static uint32_t MurMur3_32(std::vector<uint8_t>& input);
   static inline uint32_t rotl32(uint32_t x, int8_t r) {
      return (x << r) | (x >> (32 - r));
   }
private:
   static const uint32_t c1 = 0xcc9e2d51;
   static const uint32_t c2 = 0x1b873593;
   static const uint32_t m = 5;
   static const uint32_t n = 0xe6546b64;
   static const uint32_t final1 = 0x85ebca6b;
   static const uint32_t final2 = 0xc2b2ae35;
};

