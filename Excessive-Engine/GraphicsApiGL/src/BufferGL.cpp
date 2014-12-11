#include "BufferGL.h"

#include "custom_assert.h"
#include <string>

void BufferGL::destroy()
{
  glDeleteBuffers( 1, &id );
  id = 0;
}

void BufferGL::update(char* data, unsigned size, unsigned offset)
{
  //TODO
  /*ASSERT( data )
  {
    void* ptr = glMapBufferRange( id, offset, size, GL_MAP_WRITE_BIT );
    memcpy( ptr, data, size );
    glUnmapNamedBuffer( id );
  }*/
}

void BufferGL::getSubData(char* data, unsigned size, unsigned offset)
{
  //TODO
  /*ASSERT( data )
  {
    glGetNamedBufferSubData( id, offset, size, data );
  }*/
}

auto BufferGL::getDesc() -> rDesc
{
  return adata;
}