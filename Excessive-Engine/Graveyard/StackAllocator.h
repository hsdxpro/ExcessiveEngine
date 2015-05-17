#ifndef StackAllocator_h
#define StackAllocator_h

#include "custom_assert.h"

typedef u32 marker;

template< u32 alignment >
class StackAllocator
{
    char* stack; //we assume this address is aligned
    u32 size;
    u32 top;
    
    static_assert( alignment > 1, "Alignment must be >1" );

    u32 getAligned( u32 s )
    {
		  return s + (alignment - s % alignment);
    }

  public:
    StackAllocator() : stack(0), size(0), top(0) {}
    StackAllocator( char* b, u32 s ) : stack( b ), size( s ), top( 0 ) {}

    void* alloc( u32 s )
    {
      ASSERT( stack && size > 0 );
      ASSERT( top + s < size );
      u32 aligned_size = getAligned( s );
      char* mem = stack + top;
      top += aligned_size;
      return mem;
    }

    marker getMarker()
    {
      return top;
    }

    void freeToMarker( marker m )
    {
      top = m;
    }

    void clear() 
    {
      top = 0;
    }
};

#endif
