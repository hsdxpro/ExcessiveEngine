#pragma once

#include "IBuffer.h"
#include "GL/glew.h"

class BufferGL : public IBuffer
{
  public:
	rBuffer adata;
    GLuint id;

    void destroy() override;
    void update(void* data, unsigned size, unsigned offset) override; 
    void getSubData(void* data, unsigned size, unsigned offset) override;
    rDesc getDesc() override;
};