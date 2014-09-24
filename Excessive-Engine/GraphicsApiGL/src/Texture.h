#pragma once

#include "ITexture.h"

#define MAX_TEXTURE_SLOTS 16

GLuint currently_bound_textures[MAX_TEXTURE_SLOTS];
unsigned max_anisotropy = 4; //TODO we'd need a way to set this up...

GLenum texture_internal_formats[] = 
{
  GL_R8,
  GL_R8_SNORM,
  GL_RG8,
  GL_RG8_SNORM,
  GL_R16,
  GL_R16_SIGNED,
  GL_R3_G3_B2,
  GL_RGB4,
  GL_RGB5,
  GL_RGB8,
  GL_RGB8_SNORM,
  GL_RGB10,
  GL_RGB12,
  GL_RGB16_SNORM,
  GL_RGBA2,
  GL_RGBA4,
  GL_RGB5_A1,
  GL_RGBA8,
  GL_RGBA8_SNORM,
  GL_RGB10_A2,
  GL_RGB10_A2UI,
  GL_RGBA12,
  GL_RGBA16,
  GL_SRGB8,
  GL_SRGB8_A8,
  GL_R16F,
  GL_RG16F,
  GL_RGB16F,
  GL_RGBA16F,
  GL_R32F,
  GL_RG32F,
  GL_RGB32F,
  GL_RGBA32F,
  GL_R11F_G11F_B10F,
  GL_RGB9_E5,
  GL_R8I,
  GL_R8UI,
  GL_R16I,
  GL_R16UI,
  GL_R32I,
  GL_R32UI,
  GL_RG8I,
  GL_RG8UI,
  GL_RG16I,
  GL_RG16UI,
  GL_RG32I,
  GL_RG32UI,
  GL_RGB8I,
  GL_RGB8UI,
  GL_RGB16I,
  GL_RGB16UI,
  GL_RGB32I,
  GL_RGB32UI,
  GL_RGBA8I,
  GL_RGBA8UI,
  GL_RGBA16I,
  GL_RGBA16UI,
  GL_RGBA32I,
  GL_RGBA32UI,
  GL_DEPTH_COMPONENT16,
  GL_DEPTH_COMPONENT24,
  GL_DEPTH_COMPONENT32,
  GL_DEPTH_COMPONENT32F,
  GL_DEPTH24_STENCIL8,
  GL_DEPTH32F_STENCIL8,
  GL_STENCIL_INDEX1,
  GL_STENCIL_INDEX4,
  GL_STENCIL_INDEX8,
  GL_STENCIL_INDEX16
};

class Texture : public ITexture
{
    enum eDimensions
    {
      ONE = 0, TWO, THREE
    };
    
  public:
    GLuint id;
    GLenum target;
    eDimensions dim;
    rTextureData d;
  
    void destroy();
    void setSamplerState(const rTextureSamplerData* data);
    void update(const rTextureUpdateData* data);
    void getSubData(const rTextureUpdateData* data); //the pointer should be updated
    rTextureData getFormat();
    void genMipChain();
};
