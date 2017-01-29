#pragma once

#include "GL/glew.h"
#include "GraphicsApi/IBuffer.h"

class BufferGL : public IBuffer
{
public:
	rBuffer adata;
	GLuint id;

	void Release() override;
	rBuffer GetDesc() override;
};