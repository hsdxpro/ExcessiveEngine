#pragma once

#include "IBuffer.h"

class UniformBuffer : public Buffer, public IUniformBuffer
{
  public:
	  void destroy() override
    {
      Buffer::destroy();
    }

	void update(char* data, unsigned size, unsigned offset) override
    {
	    /*void* ptr = glMapBufferRange( GL_UNIFORM_BUFFER, offset, size, GL_MAP_WRITE_BIT );
      memcpy( ptr, data, size );
      glUnmapBuffer( GL_UNIFORM_BUFFER );*/

      glBindBuffer( GL_UNIFORM_BUFFER, id );
      glBufferSubData( GL_UNIFORM_BUFFER, offset, size, data );
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