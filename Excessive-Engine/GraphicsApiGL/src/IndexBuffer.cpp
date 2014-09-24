#include "IndexBuffer.h"

void IndexBuffer::destroy()
{
  glDeleteBuffers( 1, &id );
  id = 0;
}

void IndexBuffer::update(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    void* ptr = glMapNamedBufferRange( id, GL_ELEMENT_ARRAY_BUFFER, offset, size, GL_WRITE_ONLY);
    memcpy( ptr, data, size );
    glUnmapNamedBuffer( id );
  }
}

void IndexBuffer::getSubData(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    glGetNamedBufferSubData( id, offset, size, data );
  }
}

rAllocData IndexBuffer::getAllocData()
{
  return adata;
}