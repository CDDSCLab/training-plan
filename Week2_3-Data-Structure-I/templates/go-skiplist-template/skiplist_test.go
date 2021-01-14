package skiplist

import (
	"math/rand"
	"testing"
	"time"
)

func TestSkipListForDebug(t *testing.T) {
	skiplist := NewSkiplist()
	skiplist.Add(1)
	skiplist.Add(2)
	skiplist.Add(3)
	if skiplist.Search(0) {
		t.Fail()
	}
	skiplist.Add(4)
	if !skiplist.Search(1) {
		t.Fail()
	}
	if skiplist.Delete(0) {
		t.Fail()
	}
	if !skiplist.Delete(1) {
		t.Fail()
	}
	if skiplist.Delete(1) {
		t.Fail()
	}
}

func randInt(min, max int) int {
	rand.Seed(time.Now().UnixNano())
	return min + rand.Intn(max-min)
}

func TestSkipList(t *testing.T) {
	const numOfTask = 1000
	contrastSkiplist := eNewSkiplist()
	skiplist := NewSkiplist()
	for i := 0; i < numOfTask; i++ {
		num := randInt(0, numOfTask>>3)
		switch randInt(0, 3) {
		case 0:
			skiplist.Add(num)
			contrastSkiplist.eAdd(num)
		case 1:
			if skiplist.Search(num) != contrastSkiplist.eSearch(num) {
				t.Fail()
			}
		case 2:
			if skiplist.Delete(num) != contrastSkiplist.eDelete(num) {
				t.Fail()
			}
		}
	}
}
