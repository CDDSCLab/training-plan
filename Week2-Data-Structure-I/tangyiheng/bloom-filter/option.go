package bloom_filter

import (
	"github.com/spaolacci/murmur3"
	"hash"
	"hash/fnv"
	"sync"
)

// Custom configuration for the options in bloom filter
// for example, the length of bitmap, the hash functions.

// type opiton func(*BloomFilter)

func HashFuncs(hashFns ...hash.Hash64) func(*BloomFilter) {
	return func(bf *BloomFilter) {
		//bf.hashFuncs = append(bf.hashFuncs, hashFns)
		for _, hashFn := range hashFns {
			bf.hashFuncs = append(bf.hashFuncs, hashFn)
		}
		bf.k = uint(len(bf.hashFuncs))
	}
}

func Size(size uint) func(*BloomFilter) {
	return func(bf *BloomFilter) {
		bf.m = size
	}
}

// Default configuration
func configuration(bf *BloomFilter) {
	if bf.hashFuncs == nil {
		bf.hashFuncs = []hash.Hash64{
			fnv.New64(),
			fnv.New64a(),
			murmur3.New64(),
		}
		bf.k = uint(len(bf.hashFuncs))
	}
	if bf.m == 0 {
		bf.m = 1 << 18
	}
	bf.n = uint(0)
	bf.bitmap = make([]byte, bf.m / 8 + 1)
	bf.mutex = &sync.RWMutex{}
}