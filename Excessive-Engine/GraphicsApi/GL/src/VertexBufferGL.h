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

	rBuffer getDesc()
	{
		return BufferGL::getDesc();
	}
};