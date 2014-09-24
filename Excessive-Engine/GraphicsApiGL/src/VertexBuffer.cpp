#include "VertexBuffer.h"

void VertexBuffer::destroy()
{
  glDeleteBuffers( 1, &id );
  id = 0;
}

void VertexBuffer::update(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    void* ptr = glMapNamedBufferRange( id, GL_ARRAY_BUFFER, offset, size, GL_WRITE_ONLY);
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