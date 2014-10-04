#ifndef StackAllocator_h
#define StackAllocator_h

#include "custom_assert.h"

typedef unsigned marker;

template< unsigned alignment >
class StackAllocator
{
    char* stack; //we assume this address is aligned
    unsigned size;
    unsigned top;
    
    static_assert( alignment > 1, "Alignment must be >1" );

    unsigned getAligned( unsigned s )
    {
		return s + (alignment - s % alignment);
    }

  public:
    StackAllocator( char* b, unsigned s ) : stack( b ), size( s ), top( 0 ) {}

    void* alloc( unsigned s )
    {
      ASSERT( top + s < size );
      unsigned aligned_size = getAligned( s );
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
