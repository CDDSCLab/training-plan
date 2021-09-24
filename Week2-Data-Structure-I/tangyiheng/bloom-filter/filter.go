package bloom_filter

import (
	"fmt"
	"hash"
	"sync"
)

/*
	Bloom Filter
		- supported custom configuration.
 */

type Filter interface {
	Set(item []byte)		// add the item into bitmap
	Get(item []byte) bool	// test the item if the bitmap contains it
}

type BloomFilter struct {
	//bitmap []bool			// the bitmap of bloom filter
	bitmap []byte

	n uint					// the number of items in bitmap
	m uint					// the actual length of bitmap

	k uint					// the number of hash function
	hashFuncs []hash.Hash64	// the hash functions
	mutex *sync.RWMutex		// use read-write lock to make sure threads safety
}

// New - Return a new bloom filter
func New(opts ...func(*BloomFilter)) *BloomFilter {
	bf := new(BloomFilter)
	// set the custom config
	for _, opt := range opts {
		opt(bf)
	}
	// set the default config
	configuration(bf)

	return bf
}

// Set - Adds the item into the bitmap of bloom filter
func (bf *BloomFilter) Set(item []byte) {
	bf.mutex.Lock()
	defer bf.mutex.Unlock()
	// get hash values according to the number of hash functions
	hashes := bf.hashValues(item)
	for i := uint(0); i < bf.k; i++ {
		position := uint(hashes[i]) % bf.m
		byteIndex := position / 8
		if byteIndex >= uint(len(bf.bitmap)) {
			return
		}
		bitIndex := position % 8
		bf.bitmap[byteIndex] |= 1 << bitIndex
	}
	bf.n++
}

// Get - Check the item if it in the bitmap of bloom filter
func (bf *BloomFilter) Get(item []byte) bool {
	bf.mutex.RLock()
	defer bf.mutex.RUnlock()
	// get hash values according to the number of hash functions
	hashes := bf.hashValues(item)
	result := true
	for i := uint(0); i < bf.k; i++ {
		position := uint(hashes[i]) % bf.m
		byteIndex := position / 8
		if byteIndex >= uint(len(bf.bitmap)) {
			return false
		}
		bitIndex := position % 8
		if bf.bitmap[byteIndex] & (1<<bitIndex) == 0 {
			result = false
			break
		}
	}
	return result
}

// Count returns the total number of elements currently in the Bloom Filter.
func (bf *BloomFilter) Count() uint {
	return bf.n
}

// Generate the hash value of item over three hash functions
func (bf *BloomFilter) hashValues(item []byte) []uint64 {
	var result []uint64
	for _, hashFunc := range bf.hashFuncs {
		_, err := hashFunc.Write(item)
		if err != nil {
			fmt.Println(err)
		}
		result = append(result, hashFunc.Sum64())
		hashFunc.Reset()
	}
	return result
}
