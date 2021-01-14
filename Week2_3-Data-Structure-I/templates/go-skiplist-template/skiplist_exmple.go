package skiplist

import "math/rand"

// Node represent a node of skiplist
type Node struct {
	val         int
	Right, Down *Node
}

// NewNode create a skiplist node
func NewNode(v int, r, d *Node) *Node {
	return &Node{val: v, Right: r, Down: d}
}

// ExampleSkiplist represent a skiplist data structures used to test
type ExampleSkiplist struct {
	level int
	Head  *Node
}

// eNewSkiplist create a skiplist
func eNewSkiplist() ExampleSkiplist {
	return ExampleSkiplist{level: 1, Head: NewNode(0, nil, nil)}
}

// eSearch find if the target item in this skiplist
func (e *ExampleSkiplist) eSearch(target int) bool {
	cur := e.Head
	for cur != nil {
		for cur.Right != nil && cur.Right.val < target {
			cur = cur.Right
		}
		if cur.Right != nil && cur.Right.val == target {
			return true
		}
		cur = cur.Down
	}
	return false
}

// eAdd add a item in the skiplist
func (e *ExampleSkiplist) eAdd(num int) {
	rLevel := 1
	for rLevel <= e.level && ((rand.Int31() & 1) == 0) {
		rLevel++
	}

	if rLevel > e.level {
		e.level = rLevel
		e.Head = NewNode(num, nil, e.Head)
	}
	cur := e.Head
	var last *Node = nil
	for l := e.level; l >= 1; l-- {
		for cur.Right != nil && cur.Right.val < num {
			cur = cur.Right
		}
		if l <= rLevel {
			cur.Right = NewNode(num, cur.Right, nil)
			if last != nil {
				last.Down = cur.Right
			}
			last = cur.Right
		}
		cur = cur.Down
	}
}

// eDelete delete a item from skiplist if
func (e *ExampleSkiplist) eDelete(num int) bool {
	cur := e.Head
	var seen = false
	for l := e.level; l >= 1; l-- {
		for cur.Right != nil && cur.Right.val < num {
			cur = cur.Right
		}
		if cur.Right != nil && cur.Right.val == num {
			seen = true
			cur.Right = cur.Right.Right
		}
		cur = cur.Down
	}
	return seen
}

// 作者：jayustb
// 链接：https://leetcode-cn.com/problems/design-skiplist/solution/zui-jian-dan-de-golangtiao-biao-shi-xian-by-jayust/
// 来源：力扣（LeetCode）
