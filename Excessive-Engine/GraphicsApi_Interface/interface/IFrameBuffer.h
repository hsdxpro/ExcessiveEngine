#ifndef IFrameBuffer_h
#define IFrameBuffer_h

#include "ITextureView.h"

//this specifies how we'd like to use 1 texture
//in this frame buffer object (render target)
struct rTargetData
{
  ITextureView* target_texture;
  unsigned target_index;
  unsigned target_level;
  unsigned target_layer;
};


class IFrameBuffer
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void setTargets(rTargetData* data) = 0;
    virtual void use();

    virtual bool getError() = 0;
};

#endif