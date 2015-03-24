#ifndef PoolAllocator_h
#define PoolAllocator_h

#include "custom_assert.h"

//TODO
template< class t >
class PoolAllocator
{
  char* pool; //we assume this address is aligned
  u32 size; //size of the whole pool
  u32 first_free_element;

  static_assert( sizeof(t) >= sizeof(u32), "Element size must be at least 4 bytes" );

  public:
  PoolAllocator( char* p, u32 s ) : pool(p), size(s)
  {
    first_free_element = 0;

    for( u32 c = first_free_element; c < size; c += sizeof(t) )
    {
      *(u32*)(pool+c) = c + sizeof(t) < size ? c + sizeof(t) : -1;
    }
  }

  t* alloc()
  {
    u32 next = *(u32*)(pool+first_free_element);
    ASSERT(next != -1);
    void* ret = pool + first_free_element;
    first_free_element = next;
    return (t*)ret;
  }

  //you should only free stuff that you allocated...
  void free( t* p )
  {
    *(u32*)p = first_free_element;
    first_free_element = (char*)p - pool;
  }
};

#endif
