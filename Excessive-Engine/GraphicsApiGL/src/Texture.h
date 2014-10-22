#pragma once

#include "IGapi.h"
#include "ITexture.h"

#include "GL/glew.h"

#define MAX_TEXTURE_SLOTS 16

//static GLuint currently_bound_textures[MAX_TEXTURE_SLOTS];
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
    unsigned dim;
    rDesc d;
  
    void destroy() override;
    void setSamplerState(const rTextureSamplerData* data) override;
	void update(const rTextureUpdateData* data) override;
	void getSubData(const rTextureUpdateData* data) override; //the pointer should be updated
	rDesc getDesc() override;
	void genMipChain() override;
};
