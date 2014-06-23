#ifndef stack_allocator_h
#define stack_allocator_h

#include "basic_types.h"
#include "assert.h"

typedef u32 marker;

template< u32 alignment >
class stack_allocator
{
    i8* stack; //we assume this address is aligned
    u32 size;
    u32 top;
    
    static_assert( alignment > 1, "Alignment must be >1" );

    u32 get_aligned( u32 s )
    {
		return s + (alignment - s % alignment);
    }

  public:
    stack_allocator( i8* b, u32 s ) : stack( b ), size( s ), top( 0 ) {}

    void* alloc( u32 s )
    {
      ASSERT( top + s < size );
      u32 aligned_size = get_aligned( s );
      i8* mem = stack + top;
      top += aligned_size;
      return mem;
    }

    marker get_marker()
    {
      return top;
    }

    void free_to_marker( marker m )
    {
      top = m;
    }

    void clear() 
    {
      top = 0;
    }
};

#endif
