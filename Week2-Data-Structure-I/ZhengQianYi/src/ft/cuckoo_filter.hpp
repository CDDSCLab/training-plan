#ifndef FT_CUCKOO_FILTER_HPP
#define FT_CUCKOO_FILTER_HPP

#include "filter.hpp"
#include "cuckoo_vector.hpp"
#include "hasher.hpp"
#include "Rand_int.hpp"

#include <cmath>

namespace ft
{

    class cuckoo_filter : public filter
    {
    private:
        size_t cells_;
        size_t fingerprint_size_;
        size_t seed_;
        cuckoo_vector v_;
        HashType hash_name_;

    public:
        cuckoo_filter(HashType hash_name, size_t cells, size_t fingerprint_size);

        using filter::add;
        using filter::lookup;

        /// Adds an element to the Bloom filter.
        virtual void add(std::string const &o) override;

        /// Retrieves the count of an element.
        virtual bool lookup(std::string const &o) const override;

        /// delete
        void del(std::string const &o);

        std::string get_v() const;

        size_t get_cell()
        {
            return cells_;
        }
    };

    cuckoo_filter ::cuckoo_filter(HashType hash_name, size_t cells, size_t fingerprint_size) : cells_(cells), fingerprint_size_(fingerprint_size), v_(cells, fingerprint_size), hash_name_(hash_name)
    {
        Rand_int rnd{1, 10000000};
        seed_ = rnd();
    }

    void cuckoo_filter ::add(std::string const &o)
    {
        size_t fp = hashf(hash_name_, seed_, o) % (int)pow(2, (double)fingerprint_size_);

        size_t idx = fp % cells_;

        int flag = 0;

        auto last_fp = v_.set(flag, idx, fp);

        auto max_kick = cells_ * 2;

        while (max_kick-- > 0)
        {
            if (last_fp == 0)
            {
                break;
            }
            else
            {
                //把上次拿出来的再插入
                idx = (idx ^ hashf(hash_name_, seed_, last_fp)) % cells_;
                flag = (flag + 1) % 2;
                last_fp = v_.set(flag, idx, last_fp);
            }
        }

        //空间已满
    }

    bool cuckoo_filter ::lookup(std::string const &o) const
    {

        size_t fp = hashf(hash_name_, seed_, o) % (int)pow(2, (double)fingerprint_size_);

        size_t idx_0 = fp % cells_;

        size_t idx_1 = (idx_0 ^ hashf(hash_name_, seed_, fp)) % cells_;

        bool hit = v_.check(0, idx_0);
        if (hit == false)
        {
            hit = v_.check(1, idx_1);
        }
        return hit;
    }

    void cuckoo_filter ::del(std::string const &o)
    {
        size_t fp = hashf(hash_name_, seed_, o) % (int)pow(2, (double)fingerprint_size_);

        size_t idx_0 = fp % cells_;

        size_t idx_1 = (idx_0 ^ hashf(hash_name_, seed_, fp)) % cells_;

        if (v_.del(0, idx_0) == false)
        {
            v_.del(1, idx_1);
        }
    }

    std::string cuckoo_filter::get_v() const
    {
        return v_.to_string();
    }

} // namespace ft

#endif