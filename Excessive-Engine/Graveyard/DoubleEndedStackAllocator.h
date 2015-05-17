#ifndef DoubleEndedStackAllocator_h
#define DoubleEndedStackAllocator_h

#include "custom_assert.h"

typedef u32 marker;

template< u32 alignment >
class DoubleEndedStackAllocator
{
    char* stack; //we assume this address is aligned
    u32 size;
    u32 bottom;
    u32 top;
    
    static_assert( alignment > 1, "Alignment must be >1" );

    u32 getAligned( u32 s )
    {
		  return s + (alignment - s % alignment);
    }

  public:
    DoubleEndedStackAllocator() : stack(0), size(0), bottom(0), top(0) {}
    DoubleEndedStackAllocator( char* b, u32 s ) : stack( b ), size( s ), bottom( 0 ), top( size-size%alignment-1 ) {}

    void* allocBottom( u32 s )
    {
      ASSERT( stack && size > 0 );
      ASSERT( bottom + s < size && bottom + s < top );
      u32 aligned_size = getAligned( s );
      char* mem = stack + bottom;
      bottom += aligned_size;
      return mem;
    }

    void* allocTop( u32 s )
    {
      ASSERT( stack && size > 0 );
      ASSERT( top - s > 0 && top - s > bottom );
      u32 aligned_size = getAligned( s );
      char* mem = stack + top;
      top -= aligned_size;
      return mem;
    }

    marker getMarkerBottom()
    {
      return bottom;
    }

    marker getMarkerTop()
    {
      return top;
    }

    void freeToMarkerBottom( marker m )
    {
      bottom = m;
    }

    void freeToMarkerTop( marker m )
    {
      top = m;
    }

    void clearBottom() 
    {
      bottom = 0;
    }

    void clearTop() 
    {
      top = size-size%alignment-1;
    }
};

#endif