#pragma once

#include "IBuffer.h"

class UniformBuffer : public Buffer, public IUniformBuffer
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