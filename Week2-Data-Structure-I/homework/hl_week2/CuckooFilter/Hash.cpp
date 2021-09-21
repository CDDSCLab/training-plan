#include <iostream>
#include <vector>
#include "Hash.h"
#include <ctime>
#include <string>

using namespace std;

vector<uint32_t> Hash::Murmurhash(string str)
{
    vector<uint32_t> hash_results;
    for(int i=0;i<k;i++)
    {
        vector<uint8_t> input_str(str.begin(),str.end());
        uint32_t hash_result=MurMur3_32(input_str,seeds[i]);
        hash_results.push_back(hash_result);
    }
    return hash_results;//
};

uint32_t Hash::MurMur3_32(std::vector<uint8_t>& input, uint32_t seed) {
   if (input.empty()) {
      return 0;
   }
   
   const int nBlocks = input.size() / 4;
   const uint32_t* blocks = reinterpret_cast<const uint32_t *>(&input[0]);
   const uint8_t* tail = &input[nBlocks*4];
   
   uint32_t hash = seed;  //seed

   uint32_t k;
   for (int i = 0; i < nBlocks ; ++i) {
      k = blocks[i];
      
      k *= c1;
      k = rotl32(k,15);
      k *= c2;
      
      hash ^= k;
      hash = rotl32(hash,13);
      hash = (hash * m) + n;
   }
   
   k = 0;
   switch (input.size() & 3) {
      case 3: 
         k ^= tail[2] << 16;
      case 2: // intentionally inclusive of above
         k ^= tail[1] << 8;
      case 1: // intentionally inclusive of above
         k ^= tail[0];
         k *= c1;
         k = rotl32(k,15);
         k *= c2;
         hash ^= k;
   }
   
   hash ^= input.size();
   hash ^= hash >> 16;
   hash *= final1;
   hash ^= hash >> 13;
   hash *= final2;
   hash ^= hash  >> 16;
           
   return hash;
}