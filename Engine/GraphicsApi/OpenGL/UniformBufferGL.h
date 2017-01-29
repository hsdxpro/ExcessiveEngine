#pragma once

#include "BufferGL.h"

class UniformBufferGL : public BufferGL, public IUniformBuffer
{
  public:
	void Release() override
    {
      BufferGL::Release();
    }

	rBuffer GetDesc() override
    {
      return BufferGL::GetDesc();
    }
};