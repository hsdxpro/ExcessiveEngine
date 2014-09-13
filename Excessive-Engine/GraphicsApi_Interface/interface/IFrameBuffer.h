#ifndef IFrameBuffer_h
#define IFrameBuffer_h

#include "ITexture.h"

class IFrameBuffer
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void attachTexture(ITexture* tex) = 0;
    virtual void attachTextureLayered(ITexture* tex, u32 layer) = 0;
    virtual void use();

    virtual void getError() = 0;
};

#endif