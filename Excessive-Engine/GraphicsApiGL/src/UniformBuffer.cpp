#include "UniformBuffer.h"

#include <string>

void UniformBuffer::destroy()
{
  glDeleteBuffers( 1, &id );
  id = 0;
}

void UniformBuffer::update(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    void* ptr = glMapNamedBufferRange( id, offset, size, GL_WRITE_ONLY);
    memcpy( ptr, data, size );
    glUnmapNamedBuffer( id );
  }
}

void UniformBuffer::getSubData(char* data, unsigned size, unsigned offset)
{
  if( data )
  {
    glGetNamedBufferSubData( id, offset, size, data );
  }
}

rAllocData UniformBuffer::getAllocData()
{
  return adata;
}