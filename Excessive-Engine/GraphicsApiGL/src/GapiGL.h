#pragma once

#include "GL/glew.h"
#include "../../GraphicsApi_Interface/interface/IGapi.h"


class GapiGL : public IGapi
{
  public:
    unsigned global_vao;
  
	GapiGL();

	//IShaderProgram* createShaderProgram() override;
	IShaderProgram* createShaderProgram(const rShaderPaths& data) override;
	IShaderProgram* createShaderProgram(const rShaderSources& data) override;

	IUniformBuffer* createUniformBuffer(const rBuffer& data) override;
	IVertexBuffer*	createVertexBuffer(const rBuffer& data) override;
	ITexture*		createTexture(const rTexture& data) override;
	ITexture*		createTexture(const char* path) override;
	IIndexBuffer*	createIndexBuffer(const rBuffer& data) override;
    
	void WriteTexture(ITexture* t, const rTextureUpdate& d);
	
    void setDepthState(const rDepthState& state) override;
    void setStencilState(const rStencilState& state) override;
    void setBlendState(const rBlendState& state) override;
	void setSamplerState(const char* slotName, const rSamplerState& smpdata, ITexture* t) override;

    void setSRGBWrites(bool val) override;
    void setSeamlessCubeMaps(bool val) override;
    
    void setViewport(int x, int y, unsigned w, unsigned h) override;

    void setRasterizationState(const rRasterizerState& state) override;

    bool getError() override;
    void setDebugOutput(bool val) override;
    void setSyncDebugOutput(bool val) override;
    
    //pass input/output to shader
    void setShaderProgram(IShaderProgram* sp) override;
	void setRenderTargets(const rRenderTargetInfo* render_targets, unsigned size) override;
	void setUniformBuffer(IUniformBuffer* buf, unsigned index) override;
	void setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, unsigned num_buffers) override;
	void setIndexBuffer(IIndexBuffer* ibo) override;
	void setTexture(ITexture* t, unsigned idx) override;

    // draw stuff
    void draw(unsigned num_indices);

	// input layout & vertex streams
	IInputLayout* createInputLayout(InputElement* elements, size_t num_elements) override { return nullptr; }
	void setInputLayout(IInputLayout* layout) override {}

	void setVertexStreams(
		IVertexBuffer** buffers,
		unsigned* strides,
		unsigned* offsets,
		unsigned start_slot,
		unsigned num_buffers) override {}
};