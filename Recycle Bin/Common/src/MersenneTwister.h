#ifndef MersenneTwister_h
#define MersenneTwister_h

#include <random>
#include <chrono>

class MersenneTwister
{
    static std::mt19937 generator;
    static std::uniform_int_distribution<int> dist;
  public:
    static void init()
    {
      u32 seed = std::chrono::system_clock::now().time_since_epoch().count();
      generator = std::mt19937(seed);
      dist = std::uniform_int_distribution<int>( 0, RAND_MAX );
    }

    static float get()
    {
      return (float)dist(generator);
    }
};

std::mt19937 MersenneTwister::generator;
std::uniform_int_distribution<int> MersenneTwister::dist;

#endif
