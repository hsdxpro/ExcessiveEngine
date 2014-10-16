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
      /*void* ptr = glMapBufferRange( GL_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT );
      memcpy( ptr, data, size );
      glUnmapBuffer( GL_ARRAY_BUFFER );*/

      glBindBuffer( GL_ARRAY_BUFFER, id );
      glBufferSubData( GL_ARRAY_BUFFER, offset, size, data );
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