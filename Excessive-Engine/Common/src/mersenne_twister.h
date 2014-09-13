#ifndef mersenne_twister_h
#define mersenne_twister_h

#include <random>
#include <chrono>

class mersenne_twister
{
    static std::mt19937 generator;
    static std::uniform_int_distribution<int32_t> dist;
  public:
    static void init()
    {
      u32 seed = std::chrono::system_clock::now().time_since_epoch().count();
      generator = std::mt19937(seed);
      dist = std::uniform_int_distribution<i32>( 0, RAND_MAX );
    }

    static f32 get()
    {
      return (f32)dist(generator);
    }
};

std::mt19937 mersenne_twister::generator;
std::uniform_int_distribution<int32_t> mersenne_twister::dist;

#endif
