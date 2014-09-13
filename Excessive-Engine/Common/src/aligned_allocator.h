#ifndef aligned_allocator_h
#define aligned_allocator_h

#include <memory>

template< class t, std::size_t alignment = 16 >
class aligned_allocator : public std::allocator< t >
{
    aligned_allocator& operator=( const aligned_allocator& ); //no assignment
  public:
    template< class u >
    struct rebind
    {
      typedef aligned_allocator< u, alignment > other;
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

#ifdef _WIN32
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
#ifdef _WIN32
      _aligned_free( p );
#else
      free( p );
#endif
    }
};

#endif
