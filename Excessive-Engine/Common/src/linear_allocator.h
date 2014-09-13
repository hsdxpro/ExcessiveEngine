#ifndef linear_allocator_h
#define linear_allocator_h

#include "basic_types.h"

//this should handle allocations that you know they won't be freed
//like the pool and stack allocators etc.
//therefore it doesn't handle freeing
//you should also pay attention to only pass aligned memory
//of a single large block
//and you should only allocate aligned sizes
class linear_allocator
{
    i8* buffer;
    i32 size;
    i32 offset;
  public:
    linear_allocator(i8* b, i32 s) : buffer(b), size(s), offset(0)
    {
      ASSERT( (u32)b % 4 == 0 ); //only allocated memory!
    }

    void* alloc( i32 s )
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
