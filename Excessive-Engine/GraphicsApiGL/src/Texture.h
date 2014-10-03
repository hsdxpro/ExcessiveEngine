#pragma once

#include "IGapi.h"
#include "ITexture.h"

#include "GL/glew.h"

#define MAX_TEXTURE_SLOTS 16

static GLuint currently_bound_textures[MAX_TEXTURE_SLOTS];
static unsigned max_anisotropy = 4; //TODO we'd need a way to set this up...
extern GLenum texture_formats[69];
extern GLenum texture_types[69];
extern unsigned texture_sizes[69];
extern GLenum texture_internal_formats[69];

class Texture : public ITexture
{
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
