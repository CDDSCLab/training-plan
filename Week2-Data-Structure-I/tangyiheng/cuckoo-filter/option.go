package cuckoo_filter

import (
	"hash"
	"hash/fnv"
	"sync"
)

func HashFunc(hash hash.Hash) func(*CuckooFilter) {
	return func(cf *CuckooFilter) {
		cf.hashFn = hash
	}
}

func BucketNumber(number uint) func(*CuckooFilter) {
	return func(cf *CuckooFilter) {
		cf.m = number	
	}
}

func BucketSize(size uint) func(*CuckooFilter) {
	return func(cf *CuckooFilter) {
		cf.b = size
	}
}

func FingerprintSize(size uint) func(*CuckooFilter) {
	return func(cf *CuckooFilter) {
		cf.f = size
	}
}

func MaxNumKicks(number uint) func(*CuckooFilter) {
	return func(cf *CuckooFilter) {
		cf.maxNumKicks = number
	}
}

func configuration(cf *CuckooFilter) {
	if cf.hashFn == nil {
		cf.hashFn = fnv.New64()
	}
	if cf.b == 0 {
		cf.b = 4 // bucket size
	}
	if cf.m == 0 {
		cf.m = (1 << 18) / uint(cf.b) // bucket number
	}
	if cf.f == 0 {
		cf.f = 3 // fingerprint size
	}
	if cf.maxNumKicks == 0 {
		cf.maxNumKicks = 500
	}
	cf.mutex = &sync.RWMutex{}
}
