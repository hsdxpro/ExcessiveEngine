#pragma once

#include "GL/glew.h"
#include "IBuffer.h"

class VertexBuffer : public IVertexBuffer
{
  public:
    rAllocData adata;
    GLuint id;
    
    void destroy();
    void update(char* data, unsigned size, unsigned offset); 
    void getSubData(char* data, unsigned size, unsigned offset);
    rAllocData getAllocData();
};