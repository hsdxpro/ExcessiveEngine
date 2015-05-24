#pragma once

#include "mymath/mymath.h"

#include "Common.h"
#include "ITextureGapi.h"
#include "IBuffer.h"
#include "IShaderProgram.h"
#include "IInputLayout.h"


class IGapi
{
public:
	///////////////////////////////////////
	// shader creation

	/// Create shader program from source.
	/// \return nullptr is returned on failure.
	virtual IShaderProgram* createShaderSource(
		const char* vertex_shader_source,
		const char* pixel_shader_source,
		const char* geometry_shader_source = nullptr,
		const char* tess_control_shader_source = nullptr,
		const char* tess_eval_shader_source = nullptr) = 0;

	/// Create shader program from pre-compiled binary code.
	/// \return nullptr is returned on failure.
	virtual IShaderProgram* createShaderBinary(void* data, size_t size) = 0;


	///////////////////////////////////////
	// buffer creation

	/// Create uniform buffer.
	/// \return nullptr is returned of failure.
	virtual IUniformBuffer* createUniformBuffer(const rBuffer& data) = 0;

	/// Create vertex buffer.
	/// \return nullptr is returned of failure.
	virtual IVertexBuffer*	createVertexBuffer(const rBuffer& data) = 0;

	/// Create index buffer.
	/// \return nullptr is returned of failure.
	virtual IIndexBuffer*	createIndexBuffer(const rBuffer& data) = 0;

	/// Create texture.
	/// \return nullptr is returned of failure.
	virtual ITextureGapi*	createTexture(const rTextureGapi& data) = 0;


	///////////////////////////////////////
	// buffer data manipulation
	virtual void writeTexture(ITextureGapi* t, const rTextureUpdate& d) = 0;
	virtual void readTexture(ITextureGapi* t, const rTextureUpdate& d) = 0;

	virtual void writeBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void readBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void writeBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void readBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void writeBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void readBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) = 0;


	///////////////////////////////////////
	// render states
	virtual void setDepthState(const rDepthState& state) = 0;
	virtual void setStencilState(const rStencilState& state) = 0;
	virtual void setBlendState(const rBlendState& state) = 0;
	virtual void setRasterizationState(const rRasterizerState& state) = 0;
	virtual void setSamplerState(const char* slotName, const rSamplerState& smpdata, ITextureGapi* t) = 0;

	virtual void setSRGBWrites(bool val) = 0;
	virtual void setSeamlessCubeMaps(bool val) = 0;

	virtual void setViewport(int x, int y, unsigned w, unsigned h) = 0;


	///////////////////////////////////////
	// debug
	virtual bool getError() = 0;
	virtual void setDebugOutput(bool val) = 0;
	virtual void setSyncDebugOutput(bool val) = 0;


	///////////////////////////////////////
    // set render input
    virtual void setShaderProgram(IShaderProgram* sp) = 0;
    virtual void setTexture(ITextureGapi* t, u32 idx) = 0;
	virtual void setRenderTargets(const rRenderTargetInfo* render_targets, u32 size) = 0;
    virtual void setUniformBuffer(IUniformBuffer* buf, u32 idx) = 0;
	virtual void setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers) = 0;
    virtual void setIndexBuffer(IIndexBuffer* ibo) = 0;
    

	///////////////////////////////////////
    // draw calls
	virtual void drawIndexed(u32 num_indices, u32 index_byte_offset = 0) = 0;

	virtual void draw(u32 nVertices, u32 offset = 0) = 0;

	///////////////////////////////////////
	// global manipulators

	/// Clear actual frame buffer which is blitted to window client region.
	virtual void clearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth = 0, i32 stencil = 0) = 0;


	///////////////////////////////////////
	// input layout & vertex streams

	/// Get number of vertex buffer slots.
	virtual u32 getNumVertexBufferSlots() const = 0;

	/// Set vertex buffers.
	virtual void setVertexBuffers(
		const IVertexBuffer* const * buffers, ///< buffers to bind
		u32* strides, ///< size of one vertex in bytes; for each buffer
		u32* offsets, ///< how many bytes the 0th vertex is offseted from start of buffer
		u32 start_slot, ///< bind 1st buffer here
		u32 num_buffers) = 0;

	/// Create an input layout.
	/// \return nullptr is returned on failure.
	virtual IInputLayout* createInputLayout(rInputElement* elements, size_t num_elements) = 0;

	/// Set input layout.
	virtual void setInputLayout(IInputLayout* layout) = 0;


};