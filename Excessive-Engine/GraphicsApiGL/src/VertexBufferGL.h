#pragma once

#include "../../GraphicsApi_Interface/interface/IBuffer.h"
#include "BufferGL.h"

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