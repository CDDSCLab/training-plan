package bloom_filter

import (
	"fmt"
	"github.com/spaolacci/murmur3"
	"hash/fnv"
	"sync"
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
	bf := New()
	count := bf.Count()
	assertEqual(t, count, uint(0), "Expected current size of elements to be 0.")
}

func TestConfigurationOptions(t *testing.T) {
	bf := New(
			Size(1 << 18),
			HashFuncs(fnv.New64(), murmur3.New64()))
	count := bf.Count()
	assertEqual(t, count, uint(0), "Expected current size of elements to be 0.")
}

func TestSet(t *testing.T) {
	bf := New()
	bf.Set([]byte("thomison"))
	assertEqual(t, bf.Count(), uint(1), "Expected current size of elements to be 1.")

	bf.Set([]byte("tangyiheng"))
}

func TestGet(t *testing.T) {
	bf := New()
	bf.Set([]byte("thomison"))
	bf.Set([]byte("tangyiheng"))

	res := bf.Get([]byte("thomison"))
	assertEqual(t, res, true, "Expected true.")
	res2 := bf.Get([]byte("uestc"))
	assertEqual(t, res2, false, "Expected false.")
}

func TestConcurrentSet(t *testing.T) {
	bf := New()
	var wg sync.WaitGroup
	wg.Add(threads)
	for i := 0; i < threads; i++ {
		go func() {
			defer wg.Done()
			bf.Set([]byte("tangyiheng"))
		}()
	}
	wg.Wait()
	assertEqual(t, bf.Count(), uint(threads), "Expected current size of elements to be 100")
}

func BenchmarkSet(b *testing.B) {
	bf := New()
	buf := make([]byte, 4)
	for i := 0; i < b.N; i++ {
		intToByte(buf, i)
		bf.Set(buf)
	}
}

func BenchmarkGet(b *testing.B) {
	bf := New()
	buf := make([]byte, 4)
	for i := 0; i < b.N; i++ {
		intToByte(buf, i)
		bf.Get(buf)
	}
}

// converts an int (32bit) to byte array
func intToByte(b []byte, i int)  {
	b[0] = byte(i)
	b[1] = byte(i >> 8)
	b[2] = byte(i >> 16)
	b[3] = byte(i >> 24)
}



