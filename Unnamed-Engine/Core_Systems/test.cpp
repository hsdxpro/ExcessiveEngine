#include "basic_types.h"
#include "assert.h"
#include "hashed_string.h"

#include "spline.h"

#include "intersection.h"

#include "object_manager.h"

#include "random.h"
#include "basic_random.h"
#include "mersenne_twister.h"

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

  return 0;
}
