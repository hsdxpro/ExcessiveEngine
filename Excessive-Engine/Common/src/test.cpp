#include "custom_assert.h"
#include "HashedString.h"

#include "Spline.h"

#include "intersection.h"

#include "ObjectManager.h"

#include "Random.h"
#include "BasicRandom.h"
#include "MersenneTwister.h"

#include "AlignedAllocator.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"

#include "json/json.h"

#include <iostream>
#include <memory>
using namespace std;

std::string json_str = "{ \"encoding\" : \"UTF-8\", \"plug-ins\" : [ \"python\", \"c++\", \"ruby\" ], \"indent\" : { \"length\" : 3, \"use_space\": true } }";

void json_example()
{
  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  bool parsingSuccessful = reader.parse( json_str, root );
  if ( !parsingSuccessful )
  {
      // report to the user the failure and their locations in the document.
      std::cout  << "Failed to parse configuration\n"
                 << reader.getFormattedErrorMessages();
      return;
  }

  // Get the value of the member of root named 'encoding', return 'UTF-8' if there is no
  // such member.
  std::string encoding = root.get("encoding", "UTF-8" ).asString();
  // Get the value of the member of root named 'encoding', return a 'null' value if
  // there is no such member.
  const Json::Value plugins = root["plug-ins"];
  for ( int index = 0; index < plugins.size(); ++index )  // Iterates over the sequence elements.
     //loadPlugIn( plugins[index].asString() );
     my::log << my::lock << plugins[index].asString() << my::endl << my::unlock;
   
  //setIndentLength( root["indent"].get("length", 3).asInt() );
  //setIndentUseSpace( root["indent"].get("use_space", true).asBool() );

  // ...
  // At application shutdown to make the new configuration document:
  // Since Json::Value has implicit constructor for all value types, it is not
  // necessary to explicitly construct the Json::Value object:
  //root["encoding"] = getCurrentEncoding();
  //root["indent"]["length"] = getCurrentIndentLength();
  //root["indent"]["use_space"] = getCurrentIndentUseSpace();

  Json::StyledWriter writer;
  // Make a new JSON document for the configuration. Preserve original comments.
  std::string outputConfig = writer.write( root );

  // You can also use streams.  This will put the contents of any JSON
  // stream at a particular sub-value, if you'd like.
  //std::cin >> root["subtree"];

  // And you can write to a stream, using the StyledWriter automatically.
  std::cout << root;
}

int main( int argc, char** args )
{
  crc32Init();
  stringID id = HashedString::hash( "hello" );
  my::log << my::lock << 
  std::hex << id << my::endl <<
  HashedString::getStr( id ) << my::endl <<
  my::unlock;
  ASSERT( true );
  //ASSERT( false );
  static_assert( true, "blabla" );
  //static_assert( false, "blabla" );

  splinetype();

  Shape::setUpIntersection();

  om::ObjectManager<int> objman;
  om::id_type ids[5];
  for( int c = 0; c < 5; ++c )
    ids[c] = objman.add( c );

  objman.remove( ids[2] );

  objman.add( 12 );

  typedef RandomWrapper<MersenneTwister> random_type;
  random_type::init();
  my::log << my::lock << random_type::get( 0.1, 0.9 ) << my::endl << my::unlock;

  AlignedAllocator<char, 16> aa;
  unsigned base_size = 512 * 1024 * 1024;
  auto base_ptr = aa.allocate( base_size );
  char* ptr = (char*)base_ptr;
  unsigned mod = (unsigned)ptr % 4;

  LinearAllocator linall( (char*)base_ptr, base_size );
  unsigned stack_size = 256 * 1024 * 1024;
  void* stack_mem = linall.alloc( stack_size ); //allocate 256 mb of memory for the stack

  StackAllocator<16> stack( (char*)stack_mem, stack_size );

  void* mem = stack.alloc( 255 ); //allocate 256 bytes
  marker mark = stack.getMarker();
  stack.alloc( 254 );
  stack.alloc( 253 );
  stack.alloc( 252 );
  stack.alloc( 239 );

  stack.freeToMarker( mark );

  stack.clear();

  json_example();


  ////////////////////////////////////////////////////////////////////////////
  // ez itt peti cucca, töröld ki ha nagyon nem tetszik
  cout << sizeof(std::unique_ptr<int>);


  ////////////////////////////////////////////////////////////////////////////

  system( "PAUSE" );

  return 0;
}
