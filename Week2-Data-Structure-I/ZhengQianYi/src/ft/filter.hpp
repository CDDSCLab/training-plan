#ifndef FT_FILTER_HPP
#define FT_FILTER_HPP

#include <string>
namespace ft
{

    class filter
    {
    private:
        /* data */
    public:

        /// Adds an element to the Bloom filter.
        template <typename T>
        void add(T const &x)
        {
            add(wrap(x));
        }
        /// Adds an element to the Bloom filter.
        virtual void add(std::string const &o) = 0;

        /// Retrieves the count of an element.
        template <typename T>
        bool lookup(T const &x) const
        {
            return lookup(wrap(x));
        }
        /// Retrieves the count of an element.
        virtual bool lookup(std::string const &o) const = 0;
    };

} // namespace ft

#endif