#pragma once

#include "IBuffer.h"
#include "GL/glew.h"

class BufferGL : public IBuffer
{
  public:
	rBuffer adata;
    GLuint id;

    void destroy() override;
    void update(void* data, u32 size, u32 offset) override; 
    void getSubData(void* data, u32 size, u32 offset) override;
    rBuffer getDesc() override;
};