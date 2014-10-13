#include "Buffer.h"

#include "custom_assert.h"
#include <string>

void Buffer::destroy()
{
  glDeleteBuffers( 1, &id );
  id = 0;
}

void Buffer::update(char* data, unsigned size, unsigned offset)
{
  //TODO
  /*ASSERT( data )
  {
    void* ptr = glMapBufferRange( id, offset, size, GL_MAP_WRITE_BIT );
    memcpy( ptr, data, size );
    glUnmapNamedBuffer( id );
  }*/
}

void Buffer::getSubData(char* data, unsigned size, unsigned offset)
{
  //TODO
  /*ASSERT( data )
  {
    glGetNamedBufferSubData( id, offset, size, data );
  }*/
}

rAllocData Buffer::getAllocData()
{
  return adata;
}