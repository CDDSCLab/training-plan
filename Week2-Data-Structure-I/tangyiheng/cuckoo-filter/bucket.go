package cuckoo_filter

import (
	"bytes"
	"fmt"
	"github.com/spaolacci/murmur3"
	"hash"
	"math/rand"
)

/**
	bucket which store fingerprints
 */

type fingerprint []byte

type bucket []fingerprint

// Calculate fingerprint of the item according to size of fingerprint and hash function
func fprint(item []byte, fpSize uint, hashFunc hash.Hash) fingerprint {
	hashFunc.Reset()
	hashFunc.Write(item)
	h := hashFunc.Sum(nil)
	fp := make(fingerprint, fpSize)
	copy(fp, h)

	return fp
}

// hash function - convert a string to an integer
func hashfp(fp fingerprint) uint {
// DJBhash
	//var h uint = 5381
	//for i := range fp {
	//	h = ((h << 5) + h) + uint(fp[i])
	//}
	//return h
// murmur 3
	hashFunc := murmur3.New64()
	hashFunc.Reset()
	_, err := hashFunc.Write(fp)
	if err != nil {
		fmt.Println(err)
	}
	return uint(hashFunc.Sum64())
}

// Determine if two fingerprints are the same
func match(a, b fingerprint) bool {
	return bytes.Equal(a, b)
}

// insert the fingerprint into the bucket
func (b bucket) insert(fp fingerprint) bool {
	for i, f := range b {
		if f == nil {	// bucket has an empty entry for fingerprint
			b[i] = fp
			return true
		}
	}
	return false
}

// lookup the fingerprint if existed in bucket
func (b bucket) lookup(fp fingerprint) bool {
	for _, f := range b {
		if match(f, fp) {
			return true
		}
	}
	return false
}

// delete the fingerprint from the bucket
func (b bucket) delete(fp fingerprint) bool {
	for i, f := range b {
		if match(f, fp) {
			b[i] = nil
			return true
		}
	}
	return false
}

// pop the fingerprint existed in bucket randomly
// put the new fingerprint into bucket at the same time
func (b bucket) swap(fp fingerprint) fingerprint {
	i := rand.Intn(len(b))
	b[i], fp = fp, b[i]
	return fp
}