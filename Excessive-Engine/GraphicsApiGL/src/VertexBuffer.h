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
      Buffer::update(data, size, offset);
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