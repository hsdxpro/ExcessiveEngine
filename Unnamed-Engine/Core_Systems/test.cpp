#include "basic_types.h"
#include "assert.h"
#include "hashed_string.h"

#include "spline.h"

#include "intersection.h"

#include "object_manager.h"

#include "random.h"
#include "basic_random.h"
#include "mersenne_twister.h"

#include "aligned_allocator.h"
#include "linear_allocator.h"
#include "stack_allocator.h"

i32 main( i32 argc, i8** args )
{
  crc32_init();
  stringID id = hashed_string::hash( "hello" );
  my::log << my::lock << 
  std::hex << id << my::endl <<
  hashed_string::get_str( id ) << my::endl <<
  my::unlock;
  ASSERT( true );
  //ASSERT( false );
  static_assert( true, "blabla" );
  //static_assert( false, "blabla" );

  splinetype();

  shape::set_up_intersection();

  om::object_manager<i32> objman;
  om::id_type ids[5];
  for( i32 c = 0; c < 5; ++c )
    ids[c] = objman.add( c );

  objman.remove( ids[2] );

  objman.add( 12 );

  typedef random_wrapper<mersenne_twister> random_type;
  random_type::init();
  my::log << my::lock << random_type::get( 0.1, 0.9 ) << my::endl << my::unlock;

  aligned_allocator<char, 16> aa;
  u32 base_size = 512 * 1024 * 1024;
  auto base_ptr = aa.allocate( base_size );
  i8* ptr = (i8*)base_ptr;

  linear_allocator linall( (i8*)base_ptr, base_size );
  u32 stack_size = 256 * 1024 * 1024;
  void* stack_mem = linall.alloc( stack_size ); //allocate 256 mb of memory for the stack

  stack_allocator<16> stack( (i8*)stack_mem, stack_size );

  void* mem = stack.alloc( 255 ); //allocate 256 bytes
  marker mark = stack.get_marker();
  stack.alloc( 254 );
  stack.alloc( 253 );
  stack.alloc( 252 );

  stack.free_to_marker( mark );

  stack.clear();

  return 0;
}
