#pragma once

#include "ITexture.h"

class ITextureView
{
  public:
    virtual void destroy() = 0;
    virtual void setSamplerState(const rTextureSamplerData* data) = 0;
    virtual void update(const rTextureUpdateData* data) = 0;
    virtual void getSubData(const rTextureUpdateData* data) = 0; //the pointer should be updated
    virtual rTextureData getFormat() = 0;
    virtual void genMipChain() = 0;
};