#ifndef HASH_H
#define HASH_H

#if defined(_MSC_VER) && (_MSC_VER < 1600)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

// Other compilers

#else	// defined(_MSC_VER)

#include <stdint.h>

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------

uint32_t MurmurHash1        ( const void * key, int len, uint32_t seed );
uint32_t MurmurHash1Aligned ( const void * key, int len, uint32_t seed );

uint32_t MurmurHash2        ( const void * key, int len, uint32_t seed );
uint64_t MurmurHash64A      ( const void * key, int len, uint64_t seed );
uint64_t MurmurHash64B      ( const void * key, int len, uint64_t seed );
uint32_t MurmurHash2A       ( const void * key, int len, uint32_t seed );
uint32_t MurmurHashNeutral2 ( const void * key, int len, uint32_t seed );
uint32_t MurmurHashAligned2 ( const void * key, int len, uint32_t seed );

void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x86_128 ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );


#endif