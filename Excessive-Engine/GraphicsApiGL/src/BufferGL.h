#pragma once

#include "../../Module_Interfaces/GraphicsApi/IBuffer.h"
#include "GL/glew.h"

class BufferGL : public IBuffer
{
public:
	rBuffer adata;
	GLuint id;

	void destroy() override;
	rBuffer getDesc() override;
};