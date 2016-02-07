#pragma once

#include <mymath/mymath.h>

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
	virtual IShaderProgram* CreateShaderSource(
		const char* vertex_shader_source,
		const char* pixel_shader_source,
		const char* geometry_shader_source = nullptr,
		const char* tess_control_shader_source = nullptr,
		const char* tess_eval_shader_source = nullptr) = 0;

	/// Create shader program from pre-compiled binary code.
	/// \return nullptr is returned on failure.
	virtual IShaderProgram* CreateShaderBinary(void* data, size_t size) = 0;


	///////////////////////////////////////
	// buffer creation

	/// Create uniform buffer.
	/// \return nullptr is returned of failure.
	virtual IUniformBuffer* CreateUniformBuffer(const rBuffer& data) = 0;

	/// Create vertex buffer.
	/// \return nullptr is returned of failure.
	virtual IVertexBuffer*	CreateVertexBuffer(const rBuffer& data) = 0;

	/// Create index buffer.
	/// \return nullptr is returned of failure.
	virtual IIndexBuffer*	CreateIndexBuffer(const rBuffer& data) = 0;

	/// Create texture.
	/// \return nullptr is returned of failure.
	virtual ITextureGapi*	CreateTexture(const rTextureGapi& data) = 0;


	///////////////////////////////////////
	// buffer data manipulation
	virtual void WriteTexture(ITextureGapi* t, const rTextureUpdate& d) = 0;

	virtual void WriteBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void ReadBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void WriteBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void ReadBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void WriteBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) = 0;
	virtual void ReadBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) = 0;


	///////////////////////////////////////
	// render states
	virtual void ResetStatesToDefault() = 0;
	virtual void SetDepthState(const rDepthState& state) = 0;
	virtual void SetStencilState(const rStencilState& state) = 0;
	virtual void SetBlendState(const rBlendState& state) = 0;
	virtual void SetRasterizationState(const rRasterizerState& state) = 0;
	virtual void SetSamplerState(const char* slotName, const rSamplerState& smpdata, ITextureGapi* t) = 0;

	virtual void SetSRGBWrites(bool val) = 0;
	virtual void SetSeamlessCubeMaps(bool val) = 0;

	virtual void SetViewport(int x, int y, unsigned w, unsigned h) = 0;


	///////////////////////////////////////
	// debug
	virtual bool GetError() = 0;
	virtual void SetDebugOutput(bool val) = 0;
	virtual void SetSyncDebugOutput(bool val) = 0;


	///////////////////////////////////////
    // Set render input
    virtual void SetShaderProgram(IShaderProgram* sp) = 0;
    virtual void SetTexture(ITextureGapi* t, u32 idx) = 0;
	virtual void SetRenderTargets(const rRenderTargetInfo* render_targets, u32 size) = 0;
    virtual void SetUniformBuffer(IUniformBuffer* buf, u32 idx) = 0;
	virtual void SetVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers) = 0;
    virtual void SetIndexBuffer(IIndexBuffer* ibo) = 0;
    

	///////////////////////////////////////
    // Draw calls
	virtual void DrawIndexed(u32 nIndices, u32 idxStart = 0) = 0;

	virtual void Draw(u32 nVertices, u32 offset = 0) = 0;

	///////////////////////////////////////
	// global manipulators

	/// Clear actual frame buffer which is blitted to window client region.
	virtual void ClearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth = 0, i32 stencil = 0) = 0;


	///////////////////////////////////////
	// input layout & vertex streams

	/// Get number of vertex buffer slots.
	virtual u32 GetNumVertexBufferSlots() const = 0;

	/// Set vertex buffers.
	virtual void SetVertexBuffers(
		const IVertexBuffer* const * buffers, ///< buffers to bind
		u32* strides, ///< size of one vertex in bytes; for each buffer
		u32* offsets, ///< how many bytes the 0th vertex is offseted from start of buffer
		u32 start_slot, ///< bind 1st buffer here
		u32 num_buffers) = 0;

	/// Create an input layout.
	/// \return nullptr is returned on failure.
	virtual IInputLayout* CreateInputLayout(rInputElement* elements, size_t num_elements) = 0;

	/// Set input layout.
	virtual void SetInputLayout(IInputLayout* layout) = 0;


};