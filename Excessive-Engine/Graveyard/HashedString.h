#ifndef HashedString_h
#define HashedString_h

#include "crc32.h"
#include <map>

typedef u32 stringID;

class HashedString
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

  static std::string getStr( stringID sid )
  {
    auto it = hashtable.find( sid );
    
    if( it == hashtable.end() )
      return "";

    return it->second;
  }
};

std::map<stringID, std::string> HashedString::hashtable;

#endif
