#ifndef Hash_H
#define Hash_H
#include <cstddef>
#include <vector>
#include <string>

using namespace std;

class Hash
{
private:
    uint32_t k=2;  //使用k种不同的哈希
    
    uint32_t seeds[10]={0,15,21,33,47,89,66,55,44,22};

    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t m = 5;
    static const uint32_t n = 0xe6546b64;
    static const uint32_t final1 = 0x85ebca6b;
    static const uint32_t final2 = 0xc2b2ae35;
public:
    Hash(/* args */);
    ~Hash();
    vector<uint32_t> Murmurhash(string str);
    Hash(uint32_t k);


    static uint32_t MurMur3_32(std::vector<uint8_t>& input,uint32_t seed);
    static inline uint32_t rotl32(uint32_t x, int8_t r) {
      return (x << r) | (x >> (32 - r));
   }
};

Hash::Hash(/* args */)
{
}

Hash::Hash(uint32_t k)
{
    this->k=k;
}

Hash::~Hash()
{
}



#endif