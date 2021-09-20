#ifndef FT_BLOOM_FILTER_HPP
#define FT_BLOOM_FILTER_HPP

#include "filter.hpp"
#include "bitvector.hpp"
#include "hasher.hpp"
#include "Rand_int.hpp"

#include <string>

namespace ft
{

    class bloom_filter : public filter
    {
    public:
        /// the length of bits_
        size_t cells_;

        /// the bits for hash result
        bitvector bits_;

        /// the seeds for hashf
        std::vector<size_t> seeds_;

        /// whitch hash to use
        HashType hash_name_;
        ///

        /// Computes the number of cells based on a false-positive rate and capacity.
        static size_t m(double fp, size_t capcity);

        /// Computes the number of hash function
        static size_t k(size_t cells, size_t capacity);

        /// have tow way to init a bloom_filter

        /// by cells and k
        bloom_filter(HashType hashname, size_t cells, size_t k_hash);

        /// by fp and capcity
        bloom_filter(double fp, size_t capacity, HashType hashname);

        using filter::add;
        using filter::lookup;

        /// Adds an element to the Bloom filter.
        virtual void add(std::string const &o) override;

        /// Retrieves the count of an element.
        virtual bool lookup(std::string const &o) const override;

        /// get the cells num
        size_t get_cells() const;

        /// get the bits
        std::string get_bits() const;
    };

    size_t bloom_filter::m(double fp, size_t capacity)
    {
        auto ln2 = std::log(2);
        return std::ceil(-(capacity * std::log(fp) / ln2 / ln2));
    }

    size_t bloom_filter::k(size_t cells, size_t capacity)
    {
        auto frac = static_cast<double>(cells) / static_cast<double>(capacity);
        return std::ceil(frac * std::log(2));
    }

    bloom_filter ::bloom_filter(HashType hashname, size_t cells, size_t k_hash) : cells_(cells), bits_(cells), hash_name_(hashname)
    {
        Rand_int rnd{0, 100000};
        //initial the seeds
        auto t = k_hash;
        while (t--)
        {
            seeds_.push_back(rnd());
        }
    }

    bloom_filter ::bloom_filter(double fp, size_t capacity, HashType hashname) : cells_(m(fp, capacity)), bits_(m(fp, capacity)), hash_name_(hashname)
    {
        Rand_int rnd{0, 100000};
        auto t = k(cells_, capacity);
        while (t--)
        {
            seeds_.push_back(rnd());
        }
    }

    void bloom_filter ::add(std::string const &o)
    {
        for (const auto s : seeds_)
            bits_.set(hashf(hash_name_, s, o));
    }

    bool bloom_filter ::lookup(std::string const &o) const
    {
        for (const auto s : seeds_)
            if (bits_.check(hashf(hash_name_, s, o)) != true)
                return false;
        return true;
    }

    size_t bloom_filter::get_cells() const
    {
        return cells_;
    }

    std::string bloom_filter ::get_bits() const
    {
        return bits_.to_string();
    }

} // namespace ft

#endif