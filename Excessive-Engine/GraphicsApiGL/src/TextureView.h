#pragma once

#include "GL/glew.h"
#include "ITextureView.h"
#include "IGapi.h"

class TextureView : public ITextureView
{
  public:
    GLuint id;
    GLenum target;
    unsigned dim;
    rDesc d;
    
    void destroy();
	void setSamplerState(const rTextureSamplerData* data) override;
	void update(const rTextureUpdateData* data) override;
	void getSubData(const rTextureUpdateData* data) override;
	rDesc getDesc() override;
    void genMipChain() override;
};