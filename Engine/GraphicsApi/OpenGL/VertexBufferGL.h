#pragma once

#include "BufferGL.h"
#include "GraphicsApi/IBuffer.h"

class VertexBufferGL : public BufferGL, public IVertexBuffer
{
public:
	void Release()
	{
		BufferGL::Release();
	}

	rBuffer GetDesc()
	{
		return BufferGL::GetDesc();
	}
};