#ifndef FT_RAND_INT_HPP
#define FT_RAND_INT_HPP

#include<random>

namespace ft{
class Rand_int
{
private:
    std::default_random_engine re;
    std::uniform_int_distribution<>dist;
public:
    Rand_int(int low, int high):dist{low,high}{};
    int operator()(){return dist(re);}
    void seed(int s){re.seed(s);}
};

} // namespace ft


#endif