#pragma once

#include "IBuffer.h"
#include "Buffer.h"

class VertexBuffer : public Buffer, public IVertexBuffer
{
  public:
    void destroy()
    {
      Buffer::destroy();
    }

    void update(char* data, unsigned size, unsigned offset)
    {
      //Buffer::update(data, size, offset); //TODO
      glBindBuffer( GL_ARRAY_BUFFER, id );
      /*void* ptr = glMapBufferRange( GL_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT );
      memcpy( ptr, data, size );
      glUnmapBuffer( GL_ARRAY_BUFFER );*/
      glBufferSubData( GL_ARRAY_BUFFER, 0, size, data );
    }

    void getSubData(char* data, unsigned size, unsigned offset)
    {
      Buffer::getSubData(data, size, offset);
    }

    rAllocData getAllocData()
    {
      return Buffer::getAllocData();
    }
};