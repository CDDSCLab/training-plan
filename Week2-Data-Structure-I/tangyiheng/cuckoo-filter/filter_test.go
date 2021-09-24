package cuckoo_filter

import (
	"fmt"
	"hash/fnv"
	"testing"
)

const (
	tests = 1000000 		// number of elements to test, default 1 million
	fpRate = 0.001			// acceptable false positive rate, default 0.1%
	threads = 100			// number of goroutine, default
)

// achieve an assert equal function
func assertEqual(t *testing.T, a interface{}, b interface{}, message string) {
	if a == b {
		return
	}
	if len(message) == 0 {
		message = fmt.Sprintf("%v != %v", a, b)
	}
	// show message
	t.Fatal(message)
}

func TestInitialization(t *testing.T) {
	cf := New()
	assertEqual(t, cf.Count(), uint(0), "Expected current size of elements to be 0.")
}

func TestConfigurationOptions(t *testing.T) {
	cf := New(
		HashFunc(fnv.New64()),
		BucketNumber(1 << 18),
		BucketSize(4),
		FingerprintSize(3),
		MaxNumKicks(500),
	)
	assertEqual(t, cf.Count(), uint(0), "Expected current size of elements to be 0.")
}

func TestInsert(t *testing.T) {
	cf := New()

	cf.Insert([]byte("thomison"))
	assertEqual(t, cf.Count(), uint(1), "Expected current size of elements to be 1.")

	cf.Insert([]byte("tangyiheng"))
	assertEqual(t, cf.Count(), uint(2), "Expected current size of elements to be 2.")
}

//func TestConcurrentInsert(t *testing.T) {
//	cf := New()
//	var wg sync.WaitGroup
//	wg.Add(threads)
//	buf := make([]byte, 4)
//	parms := make([]uint, threads)
//	for i := 0; i < threads; i++ {
//		parms[i] = i;
//	}
//	for i := 0; i < threads; i++ {
//		go func() {
//			defer wg.Done()
//			intToByte(buf, parms[])
//			cf.Insert(buf)
//		}()
//	}
//	wg.Wait()
//	fmt.Println(cf.Count())
//	assertEqual(t, cf.Count(), uint(threads), "Expected current size of elements to be 100.")
//}

func BenchmarkInsert(b *testing.B) {
	cf := New()
	buf := make([]byte, 4)
	for i := 0; i < b.N; i++ {
		intToByte(buf, i)
		cf.Insert(buf)
	}
}

func BenchmarkLookup(b *testing.B) {
	cf := New()
	buf := make([]byte, 4)
	for i := 0; i < b.N; i++ {
		intToByte(buf, i)
		cf.Lookup(buf)
	}
}


func intToByte(b []byte, i int)  {
	b[0] = byte(i)
	b[1] = byte(i >> 8)
	b[2] = byte(i >> 16)
	b[3] = byte(i >> 24)
}