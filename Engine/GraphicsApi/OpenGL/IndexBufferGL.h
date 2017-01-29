#pragma once

class IndexBufferGL : public BufferGL, public IIndexBuffer
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