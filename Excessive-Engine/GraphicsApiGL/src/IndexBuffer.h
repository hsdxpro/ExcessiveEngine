#pragma once

#include "IBuffer.h"

class IndexBuffer : public Buffer, public IIndexBuffer
{
  public:
	  void destroy() override
    {
      Buffer::destroy();
    }

	void update(char* data, unsigned size, unsigned offset) override
    {
      /*void* ptr = glMapBufferRange( GL_ELEMENT_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT );
      memcpy( ptr, data, size );
      glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER );*/

      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id );
      glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, offset, size, data );
    }

	void getSubData(char* data, unsigned size, unsigned offset) override
    {
      Buffer::getSubData(data, size, offset);
    }

    rDesc getDesc() override
    {
      return Buffer::getDesc();
    }
};