#pragma once

#include "IGapi.h"

#include "GL/glew.h"


class Gapi : public IGapi
{
  public:
    unsigned global_vao;
  
    IShaderProgram* createShaderProgram() override;
    ITexture* createTexture(const ITexture::rDesc& data) override;
	ITextureView* createTextureView(const ITextureView::rDesc& data) override;
	IVertexBuffer* createVertexBuffer(const IVertexBuffer::rDesc& data) override;
	IIndexBuffer* createIndexBuffer(const IIndexBuffer::rDesc& data) override;
	IUniformBuffer* createUniformBuffer(const IUniformBuffer::rDesc& data) override;
  
    void setDepthState(const rDepthState& state) override;
    void setStencilState(const rStencilState& state) override;
    void setBlendState(const rBlendState& state) override;

    void setSRGBWrites(bool val) override;
    void setSeamlessCubeMaps(bool val) override;
    
    void setViewport(int x, int y, unsigned w, unsigned h) override;

    void setRasterizationState(const rRasterizerState& state) override;

    bool getError() override;
    void setDebugOutput(bool val) override;
    void setSyncDebugOutput(bool val) override;
    
    //pass input/output to shader
    void setShaderProgram(IShaderProgram* sp) override;
    void setTextureView(ITextureView* tex, unsigned index) override;
	void setRenderTargets(const rRenderTargetInfo* render_targets, unsigned size) override;
	void setUniformBuffer(IUniformBuffer* buf, unsigned index) override;
	void setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, unsigned num_buffers) override;
	void setIndexBuffer(IIndexBuffer* ibo) override;
    
    // draw stuff
    void draw(unsigned num_indices);

	// input layout & vertex streams
	IInputLayout* createInputLayout(InputElement* elements, size_t num_elements) override;
	void setInputLayout(IInputLayout* layout) override;

	void setVertexStreams(
		IVertexBuffer** buffers,
		unsigned* strides,
		unsigned* offsets,
		unsigned start_slot,
		unsigned num_buffers) override;
};