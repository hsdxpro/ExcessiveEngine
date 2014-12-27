#pragma once

#include "GL/glew.h"
#include "../../GraphicsApi_Interface/interface/IGapi.h"
#include "UniformBufferGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "ShaderProgramGL.h"
#include "TextureGL.h"
#include "InputLayoutGL.h"


class GapiGL : public IGapi
{
  public:
    GLuint global_vao;
  
	GapiGL();

	//IShaderProgram* createShaderProgram() override;
	//IShaderProgram* createShaderProgram(const rShaderPaths& data) override;
	//IShaderProgram* createShaderProgram(const rShaderSources& data) override;

	ShaderProgramGL* createShaderSource(
		const char* vertex_shader_source,
		const char* pixel_shader_source,
		const char* geometry_shader_source = nullptr,
		const char* tess_control_shader_source = nullptr,
		const char* tess_eval_shader_source = nullptr) override;
	ShaderProgramGL* createShaderFile(
		const wchar_t* vertex_shader_path,
		const wchar_t* pixel_shader_path,
		const wchar_t* geometry_shader_path = nullptr,
		const wchar_t* tess_control_shader_path = nullptr,
		const wchar_t* tess_eval_shader_path = nullptr) override;
	ShaderProgramGL* createShaderBinary(void* data, size_t size) override;

	UniformBufferGL* createUniformBuffer(const rBuffer& data) override;
	VertexBufferGL*	createVertexBuffer(const rBuffer& data) override;
	TextureGL*		createTexture(const rTexture& data) override;
	IndexBufferGL*	createIndexBuffer(const rBuffer& data) override;
    
	void WriteTexture(ITexture* t, const rTextureUpdate& d);
	
    void setDepthState(const rDepthState& state) override;
    void setStencilState(const rStencilState& state) override;
    void setBlendState(const rBlendState& state) override;
	void setSamplerState(const char* slotName, const rSamplerState& smpdata, ITexture* t) override;

    void setSRGBWrites(bool val) override;
    void setSeamlessCubeMaps(bool val) override;
    
    void setViewport(int x, int y, u32 w, u32 h) override;

    void setRasterizationState(const rRasterizerState& state) override;

    bool getError() override;
    void setDebugOutput(bool val) override;
    void setSyncDebugOutput(bool val) override;
    
    //pass input/output to shader
    void setShaderProgram(IShaderProgram* sp) override;
	void setRenderTargets(const rRenderTargetInfo* render_targets, u32 size) override;
	void setUniformBuffer(IUniformBuffer* buf, u32 index) override;
	void setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers) override;
	void setIndexBuffer(IIndexBuffer* ibo) override;
	void setTexture(ITexture* t, u32 idx) override;

    // draw stuff
    void draw(u32 num_indices);

	void clearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth = 0, i32 stencil = 0);

	// input layout & vertex streams
	InputLayoutGL* createInputLayout(rInputElement* elements, size_t num_elements) override;
	void setInputLayout(IInputLayout* layout) override;

	void setVertexStreams(
		IVertexBuffer** buffers,
		u32* strides,
		u32* offsets,
		u32 start_slot,
		u32 num_buffers) override;
};