#pragma once

#include "GL/glew.h"
#include "ITextureView.h"
#include "IGapi.h"

class TextureViewGL : public ITextureView
{
  public:
    GLuint id;
    GLenum target;
    unsigned dim;
    rDesc d;
    
    void destroy();
	void setSamplerState(const rTextureSampler* data) override;
	void update(const rTextureUpdate* data) override;
	void getSubData(const rTextureUpdate* data) override;
	rDesc getDesc() override;
    void genMipChain() override;
};