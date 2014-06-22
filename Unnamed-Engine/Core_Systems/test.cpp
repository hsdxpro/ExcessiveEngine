#include "basic_types.h"
#include "assert.h"
#include "crc32.h"

i32 main( i32 argc, i8** args )
{
  crc32_init();
  my::log << my::lock << std::hex << crc32( "hello" ) << my::endl << my::unlock;
  ASSERT( true );
  ASSERT( false );
  static_assert( true, "blabla" );
  //static_assert( false, "blabla" );
  return 0;
}
