#ifndef crc32_h
#define crc32_h

#include <string>
#include "BasicTypes.h"

const u32 polynomial = 0xedb88320;
u32 crc32lookup[256];

void crc32Init()
{
  for( u32 i = 0; i <= 0xff; ++i )
  {
    u32 crc = i;

    for( u32 j = 0; j < 8; ++j )
    {
      crc = ( crc >> 1 ) ^ ( -int( crc & 1 ) & polynomial );
    }

    crc32lookup[i] = crc;
  }
}

inline u32 crc32( const void* data, u32 length, u32 prev_crc32 = 0 )
{
  u32 crc = ~prev_crc32;
  char* current = (char*)data;

  while( length-- )
  {
    crc = ( crc >> 8 ) ^ crc32lookup[( crc & 0xff ) ^ *current++];
  }

  return ~crc;
}

inline u32 crc32( const std::string& str )
{
  return crc32( str.data(), str.size() );
}

#endif
