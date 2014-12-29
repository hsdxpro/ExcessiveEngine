#pragma once

#include "../../GraphicsApi_Interface/interface/ITexture.h"

#include "GL/glew.h"

#define MAX_TEXTURE_SLOTS 16

//static GLuint currently_bound_textures[MAX_TEXTURE_SLOTS];
static u32 max_anisotropy = 4; //TODO we'd need a way to set this up...
extern GLenum texture_formats[69];
extern GLenum texture_types[69];
extern u32 texture_sizes[69];
extern GLenum texture_internal_formats[69];

class TextureGL : public ITexture
{
public:
	GLuint ID;
	GLuint viewID;
	GLenum target;
	u32 dim;
	rTexture desc;

	void destroy() override;
	const rTexture& getDesc() override;
	void genMipChain() override;
};
