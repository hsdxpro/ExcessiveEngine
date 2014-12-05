#pragma once

#include "mymath/mymath.h"

#include "Common.h"
#include "ITextureView.h"
#include "ITexture.h"
#include "IBuffer.h"
#include "IShaderProgram.h"

#include <cstdint>

class IGapi
{
  public:
    virtual IShaderProgram* createShaderProgram() = 0;
	virtual ITexture* createTexture(const ITexture::rDesc& data) = 0;
	virtual ITextureView* createTextureView(const ITextureView::rDesc& data) = 0;
	virtual IVertexBuffer* createVertexBuffer(const IVertexBuffer::rDesc& data) = 0;
	virtual IIndexBuffer* createIndexBuffer(const IIndexBuffer::rDesc& data) = 0;
	virtual IUniformBuffer* createUniformBuffer(const IUniformBuffer::rDesc& data) = 0;
  
    virtual void setDepthState(const rDepthState& state) = 0;
    virtual void setStencilState(const rStencilState& state) = 0;
    virtual void setBlendState(const rBlendState& state) = 0;
	virtual void setRasterizationState(const rRasterizerState& state) = 0;

    virtual void setSRGBWrites(bool val) = 0;
    virtual void setSeamlessCubeMaps(bool val) = 0;
    
    virtual void setViewport(int x, int y, unsigned w, unsigned h) = 0;    

    virtual bool getError() = 0;
    virtual void setDebugOutput(bool val) = 0;
    virtual void setSyncDebugOutput(bool val) = 0;

    //pass input/output to shader
    virtual void setShaderProgram(IShaderProgram* sp) = 0;
    virtual void setTextureView(ITextureView* tex, unsigned index) = 0;
	virtual void setRenderTargets(const rRenderTargetInfo* render_targets, unsigned size) = 0;
    virtual void setUniformBuffer(IUniformBuffer* buf, unsigned index) = 0;
	virtual void setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, unsigned num_buffers) = 0;
    virtual void setIndexBuffer(IIndexBuffer* ibo) = 0;
    
    //draw stuff
    virtual void draw(unsigned num_indices) = 0;

	// REVIEW THESE
	virtual void setVertexFormat();
	virtual void setVertexBuffers(
		IVertexBuffer** buffers,
		unsigned* strides,
		unsigned* offsets,
		unsigned slot,
		unsigned num_buffers
		);
};