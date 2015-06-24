#pragma once

class IndexBufferGL : public BufferGL, public IIndexBuffer
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