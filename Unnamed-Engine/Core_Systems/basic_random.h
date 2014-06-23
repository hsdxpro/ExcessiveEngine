#ifndef basic_random_h
#define basic_random_h

#include <time.h>

class basic_random
{
  public:
    static void init()
    {
      srand( ( u32 )time( 0 ) );
    }

    static f32 get()
    {
      return (f32)rand();
    }
};

#endif
