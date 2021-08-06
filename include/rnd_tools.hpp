
#ifndef RND_TOOLS_HPP
#define RND_TOOLS_HPP

#include <random>

namespace rnd{
    // Type aliases
    using probab_t  = double;
    using seed_t    = std::mt19937::result_type;

    // Declarations
    seed_t get_seed_from_clock();

    void seed_generator(seed_t s);
    void seed_generator();

    probab_t get_randf();

    int get_randi(int);
    long int get_randi(long int);
    
    int get_weighted_randi(int, const std::vector<double>&);
}


#endif