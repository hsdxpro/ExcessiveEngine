#pragma once

#include <vector>

//specifies the desired texture format
struct rFormatData
{
  unsigned red_bits;
  unsigned green_bits;
  unsigned blue_bits;
  unsigned alpha_bits;
  bool is_signed;
  bool is_normalized;
  bool is_float;
  bool is_srgb;
  bool is_depth;
  bool is_stencil;
};

//specifices what type of texture you'd like to allocate
struct rTextureData
{
  unsigned num_levels; //mipmap levels
  unsigned width;
  unsigned height;
  unsigned depth;
  rFormatData format; //texture format
  bool is_layered;
  bool is_cubemap;
};

//specifies how you'd like to update the texture
struct rTextureUpdateData
{
  unsigned level;
  unsigned x_offset;
  unsigned y_offset;
  unsigned z_offset;
  unsigned width;
  unsigned height;
  unsigned depth;
  rFormatData format;
  std::vector< char > data;
};

//specifies texture sampler state
struct rTextureSamplerData
{
  bool is_mipmapped;
  bool is_bilinear;
  bool is_anisotropic;
  bool is_clamped; //clamped to edge OR repeated
};

//TODO replace texture formats w/ enums
class ITexture
{
  public:
    virtual void destroy() = 0;
    virtual void setSamplerState(const rTextureSamplerData& data) = 0;
    virtual void update(const rTextureUpdateData& data) = 0;
    virtual void getSubData(const rTextureUpdateData& data) = 0; //the pointer should be updated
    virtual rTextureData getFormat() = 0;
    virtual void genMipChain() = 0;
};