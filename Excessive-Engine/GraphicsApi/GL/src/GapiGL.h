#pragma once

#include "GL/glew.h"

#include "UniformBufferGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "ShaderProgramGL.h"
#include "TextureGL.h"
#include "InputLayoutGL.h"

#include "../../Interfaces/IGapi.h"


class GapiGL : public IGapi
{
public:
	GLuint global_vao;

	GapiGL();

	// shader creation
	ShaderProgramGL* createShaderSource(
		const char* vertex_shader_source,
		const char* pixel_shader_source,
		const char* geometry_shader_source = nullptr,
		const char* tess_control_shader_source = nullptr,
		const char* tess_eval_shader_source = nullptr) override;
	ShaderProgramGL* createShaderBinary(void* data, size_t size) override;

	// buffer creationg
	UniformBufferGL* createUniformBuffer(const rBuffer& data) override;
	VertexBufferGL*	createVertexBuffer(const rBuffer& data) override;
	TextureGL*		createTexture(const rTexture& data) override;
	IndexBufferGL*	createIndexBuffer(const rBuffer& data) override;

	// buffer data manipulation
	void writeTexture(ITexture* t, const rTextureUpdate& d) override;
	void readTexture(ITexture* t, const rTextureUpdate& d) override;

	void writeBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) override;
	void readBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) override;
	void writeBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) override;
	void readBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) override;
	void writeBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) override;
	void readBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) override;

	// rendereing states
	void setDepthState(const rDepthState& state) override;
	void setStencilState(const rStencilState& state) override;
	void setBlendState(const rBlendState& state) override;
	void setRasterizationState(const rRasterizerState& state) override;
	void setSamplerState(const char* slotName, const rSamplerState& smpdata, ITexture* t) override; // move to setTexture

	void setSRGBWrites(bool val) override; // move to texture parameters
	void setSeamlessCubeMaps(bool val) override; // should be true by default, shouldn't it?  

	void setViewport(int x, int y, u32 w, u32 h) override;

	// debug
	bool getError() override;
	void setDebugOutput(bool val) override;
	void setSyncDebugOutput(bool val) override;

	// set render input
	void setShaderProgram(IShaderProgram* sp) override;
	void setRenderTargets(const rRenderTargetInfo* render_targets, u32 size) override;
	void setUniformBuffer(IUniformBuffer* buf, u32 index) override;
	void setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers) override;
	void setIndexBuffer(IIndexBuffer* ibo) override;
	void setTexture(ITexture* t, u32 idx) override;

	// draw calls
	void draw(u32 num_indices, u32 index_byte_offset = 0);

	void clearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth = 0, i32 stencil = 0);

	// input layout & vertex streams
	u32 getNumVertexBufferSlots() const override;
	void setVertexBuffers(
		const IVertexBuffer* const * buffers,
		u32* strides,
		u32* offsets,
		u32 start_slot,
		u32 num_buffers) override;

	InputLayoutGL* createInputLayout(rInputElement* elements, size_t num_elements) override;
	void setInputLayout(IInputLayout* layout) override;

protected:
	/// Pairs current shader program and input layout.
	/// Called anytime a layout or a shader is set. 
	void bindInputLayout();

private:
	bool is_layout_bound;
	ShaderProgramGL* active_shader;
	InputLayoutGL* active_layout;
};