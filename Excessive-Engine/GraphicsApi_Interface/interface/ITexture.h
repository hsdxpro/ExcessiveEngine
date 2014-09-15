#ifndef ITexture_h
#define ITexture_h

#include <vector>

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
  char* data;
};

struct rTextureSamplerData
{
  bool is_mipmapped;
  bool is_bilinear;
  bool is_anisotropic;
  bool is_clamped; //clamped to edge OR repeated
};

class ITexture
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void allocate(const rTextureData& data) = 0;
    virtual void setSamplerState(const rTextureSamplerData& data) = 0;
    virtual void update(const rTextureUpdateData& data) = 0;
    virtual void getSubData(const rTextureUpdateData& data) = 0; //the pointer should be updated
    virtual rTextureData getFormat() = 0;
    virtual void genMipChain() = 0;
};

#endif