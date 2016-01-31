#pragma once

#include "GL/glew.h"
#include "Gapi/IBuffer.h"

class BufferGL : public IBuffer
{
public:
	rBuffer adata;
	GLuint id;

	void destroy() override;
	rBuffer GetDesc() override;
};