#ifndef AlignedAllocator_h
#define AlignedAllocator_h

#include <memory>

//allocates aligned memory
template< class t, std::size_t alignment = 16 >
class AlignedAllocator : public std::allocator< t >
{
    AlignedAllocator& operator=( const AlignedAllocator& ); //no assignment

    static_assert( alignment > 1, "only works if alignment > 1, else use regular heap allocator (malloc)" );

  public:
    template< class u >
    struct rebind
    {
      typedef AlignedAllocator< u, alignment > other;
    } ;

    typename std::allocator<t>::pointer allocate( typename std::allocator<t>::size_type n, typename std::allocator<t>::const_pointer hint = 0 )
    {
      if( !n )
      {
        return 0;
      }

      if( n > std::allocator<t>::max_size() )
      {
        throw std::length_error( "aligned_allocator<t>::allocate() - Integer overflow." );
      }

      u32 extended_size = n * sizeof( typename std::allocator<t>::value_type ) + alignment;
      u32 unaligned_address = (u32)malloc(extended_size); //use whatever unaligned allocator is available
      
      if( !unaligned_address )
      {
        throw std::bad_alloc();
      }

      u32 mask = alignment - 1;
      u32 misalignment = unaligned_address & mask;
      u32 adjustment = alignment - misalignment;

      u32 aligned_address = unaligned_address + adjustment;

      u32* adjustment_ptr = (u32*)(aligned_address - 4);
      *adjustment_ptr = adjustment;

      return static_cast<typename std::allocator<t>::pointer>( (void*)aligned_address );
    }

    void deallocate( typename std::allocator<t>::pointer p, typename std::allocator<t>::size_type n )
    {
      u32 aligned_address = (u32)p;
      u32* adjustment_ptr = (u32*)(aligned_address - 4);
      u32 adjustment = *adjustment_ptr;
      u32 unaligned_address = aligned_address - adjustment;

      free((void*)unaligned_address);
    }
};

#endif
