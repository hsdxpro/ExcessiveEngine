#pragma once

#include "../../GraphicsApi_Interface/interface/IBuffer.h"
#include "GL/glew.h"

class BufferGL : public IBuffer
{
public:
	rBuffer adata;
	GLuint id;

	void destroy() override;
	rBuffer getDesc() override;
};