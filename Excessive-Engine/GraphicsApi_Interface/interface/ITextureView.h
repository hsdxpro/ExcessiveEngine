#ifndef ITextureView_h
#define ITextureView_h

#include "ITexture.h"

//specifices what type of texture you'd like to allocate
struct rTextureViewData
{
  unsigned start_level; //mipmap levels
  unsigned num_levels;
  unsigned start_layer;
  unsigned num_layers;
  rFormatData format; //texture format
  ITexture* base_tex;
};

class ITextureView
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void setView(const rTextureViewData& data) = 0;
    virtual void setSamplerState(const rTextureSamplerData& data) = 0;
    virtual void update(const rTextureUpdateData& data) = 0;
    virtual void getSubData(const rTextureUpdateData& data) = 0; //the pointer should be updated
    virtual rTextureData getFormat() = 0;
};

#endif