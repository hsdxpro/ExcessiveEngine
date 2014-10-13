#pragma once

#include "IBuffer.h"

class IndexBuffer : public Buffer, public IIndexBuffer
{
  public:
    void destroy()
    {
      Buffer::destroy();
    }

    void update(char* data, unsigned size, unsigned offset)
    {
      //Buffer::update(data, size, offset); //TODO
      /*glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id );
      void* ptr = glMapBufferRange( GL_ELEMENT_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT );
      memcpy( ptr, data, size );
      glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER );*/
      glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, size, data );
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