#pragma once

#include "mymath/mymath.h"

#include "Common.h"
#include "ITexture.h"
#include "IBuffer.h"
#include "IShaderProgram.h"
#include "IInputLayout.h"


class IGapi
{
public:
	//virtual IShaderProgram* createShaderProgram(const rShaderPaths& data) = 0;
	//virtual IShaderProgram* createShaderProgram(const rShaderSources& data) = 0;

	/// Create shader program from source.
	/// \return nullptr is returned on failure.
	virtual IShaderProgram* createShaderSource(
		const char* vertex_shader_source,
		const char* pixel_shader_source,
		const char* geometry_shader_source = nullptr,
		const char* tess_control_shader_source = nullptr,
		const char* tess_eval_shader_source = nullptr) = 0;
	/// Create shader program from files on disk.
	/// \return nullptr is returned on failure.
	virtual IShaderProgram* createShaderFile(
		const wchar_t* vertex_shader_path,
		const wchar_t* pixel_shader_path,
		const wchar_t* geometry_shader_path = nullptr,
		const wchar_t* tess_control_shader_path = nullptr,
		const wchar_t* tess_eval_shader_path = nullptr) = 0;
	/// Create shader program from pre-compiled binary code.
	/// \return nullptr is returned on failure.
	virtual IShaderProgram* createShaderBinary(void* data, size_t size) = 0;


	virtual IUniformBuffer* createUniformBuffer(const rBuffer& data) = 0;
	virtual IVertexBuffer*	createVertexBuffer(const rBuffer& data) = 0;
	virtual IIndexBuffer*	createIndexBuffer(const rBuffer& data) = 0;
	virtual ITexture*		createTexture(const rTexture& data) = 0;

	virtual void WriteTexture(ITexture* t, const rTextureUpdate& d) = 0;

	virtual void setDepthState(const rDepthState& state) = 0;
	virtual void setStencilState(const rStencilState& state) = 0;
	virtual void setBlendState(const rBlendState& state) = 0;
	virtual void setRasterizationState(const rRasterizerState& state) = 0;
	virtual void setSamplerState(const char* slotName, const rSamplerState& smpdata, ITexture* t) = 0;

	virtual void setSRGBWrites(bool val) = 0;
	virtual void setSeamlessCubeMaps(bool val) = 0;
	virtual void setViewport(int x, int y, unsigned w, unsigned h) = 0;

	virtual bool getError() = 0;
	virtual void setDebugOutput(bool val) = 0;
	virtual void setSyncDebugOutput(bool val) = 0;

    //pass input/output to shader
    virtual void setShaderProgram(IShaderProgram* sp) = 0;
    virtual void setTexture(ITexture* t, u32 idx) = 0;
	virtual void setRenderTargets(const rRenderTargetInfo* render_targets, u32 size) = 0;
    virtual void setUniformBuffer(IUniformBuffer* buf, u32 idx) = 0;
	virtual void setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers) = 0;
    virtual void setIndexBuffer(IIndexBuffer* ibo) = 0;
    
    //draw stuff
    virtual void draw(size_t num_indices) = 0;

	// Clear actual frame buffer which is blitted to window client region
	virtual void clearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth = 0, i32 stencil = 0) = 0;


	// input layout & vertex streams

	/// Create an input layout.
	/// \return nullptr is returned on failure.
	virtual IInputLayout* createInputLayout(rInputElement* elements, size_t num_elements) = 0;
	/// Set input layout.
	virtual void setInputLayout(IInputLayout* layout) = 0;

	/*
	Egyszerûen bindeli a vertex buffereket a megfelelõ 'slot'-ba.
	Ha több buffert adunk meg, akkor azokat a start_slot, start_slot+1, start_slot+2 helyekre bindeli.
	A nullptr buffer unbindeli az adott slotból a buffert.
	*/
	/// Set vertex buffers.
	virtual void setVertexStreams(
		IVertexBuffer** buffers, ///< buffers to bind
		u32* strides, ///< size of one vertex in bytes; for each buffer
		u32* offsets, ///< how many bytes the 0th vertex is offseted from start of buffer
		u32 start_slot, ///< bind 1st buffer here
		u32 num_buffers) = 0;
};