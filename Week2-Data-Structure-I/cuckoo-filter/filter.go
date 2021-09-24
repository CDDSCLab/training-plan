package cuckoo_filter

import (
	"hash"
	"math/rand"
	"sync"
)

/**
	Cuckoo Filter
		- supported custom configuration.
		- supported insert and delete dynamically
		- higher lookup performance
		- lower space overhead
	there are two hash functions:
		one for generating fingerprint
		another for converting the item to an integer
 */

type Filter interface {
	Insert(item []byte)	bool	// Insert item into cuckoo filter
	Lookup(item	[]byte)	bool	// Lookup item if contains in cuckoo filter
	Delete(item []byte)	bool	// Delete item from cuckoo filter
	Count()	uint				// Count the number of current elements in cuckoo filter
}

type CuckooFilter struct {
	buckets []bucket			// buckets where fingerprints are stored
	hashFn hash.Hash			// the hash function which used in cuckoo filter
	m uint						// the number of buckets
	b uint						// the size of a bucket
	f uint						// the size of a fingerprint - contains the number of hash value bits
	//k uint						// the number of hash functions
	n uint						// the number of items current in cuckoo filter
	maxNumKicks uint			// the maximum number we kick items out items from buckets
	mutex *sync.RWMutex			// use read-write lock to make sure threads safety
}

func New(fns ...func(*CuckooFilter)) *CuckooFilter {
	cf := new(CuckooFilter)
	// custom config
	for _, fn := range fns {
		fn(cf)
	}
	// default config
	configuration(cf)

	cf.buckets = make([]bucket, cf.m)
	for i := range cf.buckets {
		cf.buckets[i] = make([]fingerprint, cf.b)
	}
	return cf
}

// Insert - Insert item into cuckoo filter
func (cf *CuckooFilter) Insert(item []byte) bool {
	cf.mutex.Lock()
	defer cf.mutex.Unlock()
	//
	f := fprint(item, cf.f, cf.hashFn)
	i1 := hashfp(item) % cf.m
	i2 := (i1 ^ hashfp(f)) % cf.m

	if cf.buckets[i1].insert(f) {
		cf.n++
		return true
	} else if cf.buckets[i2].insert(f) {
		cf.n++
		return true
	} else {
		i := [2]uint{i1, i2}[rand.Intn(2)]
		// when bucket is full, kick out item randomly until the bucket has empty space
		for n := uint(0); n < cf.maxNumKicks; n++ {
			f := cf.buckets[i].swap(f)
			i = (i ^ hashfp(f)) % cf.m

			if cf.buckets[i].insert(f) {
				cf.n++
				return true
			}
		}
	}
	return false
}

// Lookup - Lookup item if contains in cuckoo filter
func (cf *CuckooFilter) Lookup(item []byte) bool {
	cf.mutex.RLock()
	defer cf.mutex.RUnlock()
	//
	f := fprint(item, cf.f, cf.hashFn)
	i1 := hashfp(item) % cf.m
	i2 := i1 ^ hashfp(f) % cf.m

	if cf.buckets[i1].lookup(f) || cf.buckets[i2].lookup(f) {
		return true
	}
	return false
}

// Delete - Delete item from cuckoo filter
func (cf *CuckooFilter) Delete(item []byte)	bool {
	cf.mutex.Lock()
	defer cf.mutex.Unlock()
	//
	f := fprint(item, cf.f, cf.hashFn)
	i1 := hashfp(item) % cf.m
	i2 := i1 ^ hashfp(f) % cf.m

	if cf.buckets[i1].delete(f) || cf.buckets[i2].delete(f) {
		cf.n--
		return true
	}
	return false
}

// Count - Count the number of current elements in cuckoo filter
func (cf *CuckooFilter) Count() uint {
	return cf.n
}

