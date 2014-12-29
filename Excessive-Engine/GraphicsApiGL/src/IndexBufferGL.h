#pragma once

class IndexBufferGL : public BufferGL, public IIndexBuffer
{
  public:
	  void destroy() override
    {
      BufferGL::destroy();
    }

	rBuffer getDesc() override
    {
      return BufferGL::getDesc();
    }
};