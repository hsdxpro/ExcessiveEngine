#ifndef hashed_string_h
#define hashed_string_h

#include "basic_types.h"
#include "crc32.h"
#include <map>

typedef u32 stringID;

class hashed_string
{
  static std::map<stringID, std::string> hashtable;
public:
  static stringID hash( const std::string& str )
  {
    stringID sid = crc32( str );

    auto it = hashtable.find( sid );

    if( it == hashtable.end() )
    {
      hashtable[sid] = str;
    }

    return sid;
  }  

  static std::string get_str( stringID sid )
  {
    auto it = hashtable.find( sid );
    
    if( it == hashtable.end() )
      return "";

    return it->second;
  }
};

std::map<stringID, std::string> hashed_string::hashtable;

#endif
