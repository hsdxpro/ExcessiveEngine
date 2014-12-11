#pragma once

#include "IBuffer.h"
#include "BufferGL.h"

class VertexBufferGL : public BufferGL, public IVertexBuffer
{
  public:
    void destroy()
    {
      BufferGL::destroy();
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
		BufferGL::getSubData(data, size, offset);
    }

    rDesc getDesc()
    {
		return BufferGL::getDesc();
    }
};