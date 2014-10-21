#pragma once

#include "IBuffer.h"
#include "GL/glew.h"

class Buffer : public IBuffer
{
  public:
    rDesc adata;
    GLuint id;

    void destroy() override;
    void update(char* data, unsigned size, unsigned offset) override; 
    void getSubData(char* data, unsigned size, unsigned offset) override;
    rDesc getDesc() override;
};