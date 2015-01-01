#pragma once

#include "../../Module_Interfaces/GraphicsApi/IBuffer.h"
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