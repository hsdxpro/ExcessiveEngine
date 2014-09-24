#pragma once

//http://www.opengl.org/wiki/Image_Format
enum eFormatType
{
  R8 = 0,
  R8_SIGNED,
  RG8,
  RG8_SIGNED,
  R16,
  R16_SIGNED,
  R3_G3_B2,
  RGB4,
  RGB5,
  RGB8,
  RGB8_SIGNED,
  RGB10,
  RGB12,
  RGB16_SIGNED,
  RGBA2,
  RGBA4,
  RGB5_A1,
  RGBA8,
  RGBA8_SIGNED,
  RGB10_A2,
  RGB10_A2_UNSIGNED_INT,
  RGBA12,
  RGBA16,
  SRGB8,
  SRGB8_A8,
  R16F,
  RG16F,
  RGB16F,
  RGBA16F,
  R32F,
  RG32F,
  RGB32F,
  RGBA32F,
  R11F_G11F_B10F,
  RGB9_E5,
  R8_INT,
  R8_UNSIGNED_INT,
  R16_INT,
  R16_UNSIGNED_INT,
  R32_INT,
  R32_UNSIGNED_INT,
  RG8_INT,
  RG8_UNSIGNED_INT,
  RG16_INT,
  RG16_UNSIGNED_INT,
  RG32_INT,
  RG32_UNSIGNED_INT,
  RGB8_INT,
  RGB8_UNSIGNED_INT,
  RGB16_INT,
  RGB16_UNSIGNED_INT,
  RGB32_INT,
  RGB32_UNSIGNED_INT,
  RGBA8_INT,
  RGBA8_UNSIGNED_INT,
  RGBA16_INT,
  RGBA16_UNSIGNED_INT,
  RGBA32_INT,
  RGBA32_UNSIGNED_INT,
  DEPTH16,
  DEPTH24,
  DEPTH32,
  DEPTH32F,
  DEPTH24_STENCIL8,
  DEPTH32F_STENCIL8,
  STENCIL1,
  STENCIL4,
  STENCIL8,
  STENCIL16
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
  eFormatType format;
  char* data; //w, h, d, the offsets and format determines the size!
};

//specifies texture sampler state
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
    virtual void destroy() = 0;
    virtual void setSamplerState(const rTextureSamplerData* data) = 0;
    virtual void update(const rTextureUpdateData* data) = 0;
    virtual void getSubData(const rTextureUpdateData* data) = 0; //the pointer should be updated
    virtual rTextureData getFormat() = 0;
    virtual void genMipChain() = 0;
};