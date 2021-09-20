#ifndef FT_CUCKOO_VECTOR_HPP
#define FT_CUCKOO_VECTOR_HPP

#include <vector>
#include <string>
using std::vector;

namespace ft
{
    typedef unsigned short ut;

    class cuckoo_vector
    {
    private:
        vector<vector<ut>> buckets_;

        size_t cells_;
        size_t fingerprint_size_;

    public:
        cuckoo_vector(size_t cells, size_t fingerprint);

        // set buckets[flag][i] and return the oringe value
        size_t set(size_t flag, size_t i, size_t fingerprint);

        // return buckets[flag][i]
        size_t check(size_t flag, size_t i) const;

        //delete buckets[flag][i] if the fingerprint is matched
        bool del(size_t flag, size_t i);

        std::string to_string() const;
    };

    cuckoo_vector ::cuckoo_vector(size_t cells, size_t fingerprint_size) : cells_(cells), fingerprint_size_(fingerprint_size)
    {
        buckets_ = vector<vector<ut>>(2);
        for (auto &v : buckets_)
        {
            v = vector<ut>(cells, 0);
        }
    }

    size_t cuckoo_vector::set(size_t flag, size_t i, size_t fingerprint)
    {
        auto last_fp = buckets_[flag][i];

        // handling duplicate additions
        if (last_fp == fingerprint)
            return 0;

        buckets_[flag][i] = static_cast<ut>(fingerprint);

        return last_fp;
    }

    size_t cuckoo_vector ::check(size_t flag, size_t i) const
    {
        return buckets_[flag][i];
    }

    std::string cuckoo_vector::to_string() const
    {
        std::string s{};
        for (auto p : buckets_[0])
        {
            if (p != 0)
                s.append("1");
            else
                s.append("0");
        }
        s.append("\n");
        for (auto p : buckets_[1])
        {
            if (p != 0)
                s.append("1");
            else
                s.append("0");
        }
        return s;
    }

    bool cuckoo_vector::del(size_t flag, size_t i)
    {
        if (buckets_[flag][i] == 0)
        {
            return false;
        }
        else
        {
            buckets_[flag][i] = 0;
            return true;
        }
    }

} // namespace ft

#endif