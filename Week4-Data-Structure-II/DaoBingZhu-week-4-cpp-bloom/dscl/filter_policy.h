//
// Created by IcePig on 2020-07-27.
//

#ifndef FILTER_POLICY_H
#define FILTER_POLICY_H

#include <string>
#include <iostream>

class FilterPolicy {
public:
    virtual ~FilterPolicy();


    // const = 0 纯虚函数
    // Return the name of this policy.  Note that if the filter encoding
    // changes in an incompatible way, the name returned by this method
    // must be changed.  Otherwise, old incompatible filters may be
    // passed to methods of this type.
    virtual const char *Name() const = 0;

    // keys[0,n-1] contains a list of keys (potentially with duplicates)
    // that are ordered according to the user supplied comparator.
    // Append a filter that summarizes keys[0,n-1] to *dst.
    //
    // Warning: do not change the initial contents of *dst.  Instead,
    // append the newly constructed filter to *dst.
    // 传入存储在keys的n个key，使用dst记录key是否存在.
    virtual void AddKey(const std::string *keys, int n) const = 0;

    // delete key
    virtual bool DeleteKey(const std::string &key) const {
        return false;
    }

    // "filter" contains the data appended by a preceding call to
    // CreateFilter() on this class.  This method must return true if
    // the key was in the list of keys passed to CreateFilter().
    // This method may return true or false if the key was not on the
    // list, but it should aim to return false with a high probability.
    // 通过CreateFilter产出的dst(filter)，计算key是否存在
    virtual bool KeyMayMatch(const std::string &key) const = 0;

    virtual void DumpData() const = 0;
};

// Return a new filter policy that uses a bloom filter with approximately
// the specified number of bits per key.  A good value for bits_per_key
// is 10, which yields a filter with ~ 1% false positive rate.
//
// Callers must delete the result after any database that is using the
// result has been closed.
//
// Note: if you are using a custom comparator that ignores some parts
// of the keys being compared, you must not use NewBloomFilterPolicy()
// and must provide your own FilterPolicy that also ignores the
// corresponding parts of the keys.  For example, if the comparator
// ignores trailing spaces, it would be incorrect to use a
// FilterPolicy (like NewBloomFilterPolicy) that does not ignore
// trailing spaces in keys.
const FilterPolicy *NewBloomFilterPolicy(int bits_per_key, int key_num);

const FilterPolicy *NewCountBloomFilterPolicy(int buckets_per_key, int key_num);

const FilterPolicy *NewCuckooFilterPolicy();

#endif //FILTER_POLICY_H
