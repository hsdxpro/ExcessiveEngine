#ifndef LinearAllocator_h
#define LinearAllocator_h

#include "custom_assert.h"

//this should handle allocations that you know they won't be freed
//like the pool and stack allocators etc.
//therefore it doesn't handle freeing
//you should also pay attention to only pass aligned memory
//of a single large block
//and you should only allocate aligned sizes
class LinearAllocator
{
    char* buffer;
    int size;
    int offset;
  public:
    LinearAllocator(char* b, int s) : buffer(b), size(s), offset(0)
    {
      ASSERT( (u32)b % 4 == 0 ); //only allocated memory!
    }

    void* alloc( int s )
    {
      ASSERT( s % 4 == 0 ); //only aligned allocations
      void* mem_to_return = buffer + offset + s;
      offset += s;
      return mem_to_return;
    }

    void free( void* b )
    {
      ASSERT( false ); //this shouldn't happen
    }
};

#endif
