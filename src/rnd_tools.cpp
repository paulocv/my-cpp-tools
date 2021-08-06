
#include <random>  // mt19937
#include <chrono>  // system_clock
#include <numeric> // std::accumulate

#include "rnd_tools.hpp"


namespace rnd{

    // Mersenne Twister generator and some distributions tod be used
    std::mt19937 gen{get_seed_from_clock()};
    std::uniform_real_distribution<probab_t> uniform_dist;

    // --- Seed handling
    seed_t get_seed_from_clock(){
        return std::chrono::system_clock::now().time_since_epoch().count();
    }

    // Use a given seed
    void seed_generator(seed_t s){
        gen.seed(s);
    }

    // Random seed from clock
    void seed_generator(){
        gen.seed(get_seed_from_clock());
    }

    // --- Random generation
    /// Random float in [0, 1) of probab_t.
    probab_t get_randf(){
        return uniform_dist(gen);
    }

    // Random integer 0 <= n < imax  (not included)
    int get_randi(int imax){
        return floor(get_randf() * imax);
    }

    // Random integer 0 <= n < imax  (not included)
    long int get_randi(long int imax){
        return floor(get_randf() * imax);
    }

    // Random integer with arbitrary weights (no need to add to 1)
    // Naive implementation, without any optimization attempt
    int get_weighted_randi(int imax, const std::vector<double>& weights){

        double weight_sum {std::accumulate(weights.cbegin(), weights.cend(), 0.0)};
        double rnd_real {weight_sum * get_randf()};

        for (int i {0}; i < imax; i++){
            rnd_real -= weights[i];
            if (rnd_real < 0.0){
                return i;
            }
        }
        // Should not be reached
        return -1;
    }

    // TODO: choice function, "fast" random integer (without assuming repeated probabilities).
}