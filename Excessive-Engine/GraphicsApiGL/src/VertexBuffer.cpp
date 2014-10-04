#include "VertexBuffer.h"

#include <string>

void VertexBuffer::destroy()
{
  glDeleteBuffers( 1, &id );
  id = 0;
}

void VertexBuffer::update(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    void* ptr = glMapNamedBufferRange( id, offset, size, GL_MAP_WRITE_BIT );
    memcpy( ptr, data, size );
    glUnmapNamedBuffer( id );
  }
}

void VertexBuffer::getSubData(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    glGetNamedBufferSubData( id, offset, size, data );
  }
}

rAllocData VertexBuffer::getAllocData()
{
  return adata;
}