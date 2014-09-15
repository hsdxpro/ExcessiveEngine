#ifndef crc32_h
#define crc32_h

#include <string>

const unsigned polynomial = 0xedb88320;
unsigned crc32lookup[256];

void crc32Init()
{
  for( unsigned i = 0; i <= 0xff; ++i )
  {
    unsigned crc = i;

    for( unsigned j = 0; j < 8; ++j )
    {
      crc = ( crc >> 1 ) ^ ( -int( crc & 1 ) & polynomial );
    }

    crc32lookup[i] = crc;
  }
}

inline unsigned crc32( const void* data, unsigned length, unsigned prev_crc32 = 0 )
{
  unsigned crc = ~prev_crc32;
  char* current = (char*)data;

  while( length-- )
  {
    crc = ( crc >> 8 ) ^ crc32lookup[( crc & 0xff ) ^ *current++];
  }

  return ~crc;
}

inline unsigned crc32( const std::string& str )
{
  return crc32( str.data(), str.size() );
}

#endif
