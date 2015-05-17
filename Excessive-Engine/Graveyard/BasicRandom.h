#ifndef BasicRandom_h
#define BasicRandom_h

#include <time.h>

class BasicRandom
{
  public:
    static void init()
    {
      srand( ( u32 )time( 0 ) );
    }

    static float get()
    {
      return (float)rand();
    }
};

#endif
