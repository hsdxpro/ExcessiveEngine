#pragma once

#include "BufferGL.h"

class UniformBufferGL : public BufferGL, public IUniformBuffer
{
  public:
	  void destroy() override
    {
      BufferGL::destroy();
    }

	rBuffer GetDesc() override
    {
      return BufferGL::GetDesc();
    }
};