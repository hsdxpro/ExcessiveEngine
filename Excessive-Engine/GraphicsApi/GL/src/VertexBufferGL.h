#pragma once

#include "BufferGL.h"
#include "../../Interfaces/IBuffer.h"

class VertexBufferGL : public BufferGL, public IVertexBuffer
{
public:
	void destroy()
	{
		BufferGL::destroy();
	}

	rBuffer GetDesc()
	{
		return BufferGL::GetDesc();
	}
};