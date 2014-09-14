#ifndef AlignedAllocator_h
#define AlignedAllocator_h

#include <memory>

//allocates aligned memory
template< class t, std::size_t alignment = 16 >
class AlignedAllocator : public std::allocator< t >
{
    AlignedAllocator& operator=( const AlignedAllocator& ); //no assignment
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

#ifdef WIN_BUILD
      void* pv = _aligned_malloc( n * sizeof( typename std::allocator<t>::value_type ), alignment );
#else
      void* pv = 0;

      if( posix_memalign( &pv, alignment, n * sizeof( typename std::allocator<t>::value_type ) ) )
        pv = 0;

#endif

      if( !pv )
      {
        throw std::bad_alloc();
      }

      return static_cast<typename std::allocator<t>::pointer>( pv );
    }

    void deallocate( typename std::allocator<t>::pointer p, typename std::allocator<t>::size_type n )
    {
#ifdef WIN_BUILD
      _aligned_free( p );
#else
      free( p );
#endif
    }
};

#endif
