#include "Buffer.h"

#include <string>

void Buffer::destroy()
{
  glDeleteBuffers( 1, &id );
  id = 0;
}

void Buffer::update(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    void* ptr = glMapNamedBufferRange( id, offset, size, GL_MAP_WRITE_BIT );
    memcpy( ptr, data, size );
    glUnmapNamedBuffer( id );
  }
}

void Buffer::getSubData(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    glGetNamedBufferSubData( id, offset, size, data );
  }
}

rAllocData Buffer::getAllocData()
{
  return adata;
}