#pragma once

#include "GL/glew.h"
#include "../../Interfaces/IBuffer.h"

class BufferGL : public IBuffer
{
public:
	rBuffer adata;
	GLuint id;

	void destroy() override;
	rBuffer GetDesc() override;
};