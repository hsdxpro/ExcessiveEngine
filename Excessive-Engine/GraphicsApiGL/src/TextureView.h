#pragma once

#include "GL/glew.h"
#include "ITextureView.h"
#include "IGapi.h"

class TextureView : public ITextureView
{
  public:
    GLuint id;
    GLenum target;
    eDimensions dim;
    rTextureData d;
    
    void destroy();
    void setSamplerState(const rTextureSamplerData* data);
    void update(const rTextureUpdateData* data);
    void getSubData(const rTextureUpdateData* data);
    rTextureData getFormat();
    void genMipChain();
};