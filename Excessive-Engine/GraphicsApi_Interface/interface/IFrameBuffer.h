#pragma once

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

//dx: render target
class IFrameBuffer
{
  public:
    virtual void destroy() = 0;
    virtual void setTargets(const std::vector< rTargetData >& data) = 0;

    virtual bool getError() = 0;
};