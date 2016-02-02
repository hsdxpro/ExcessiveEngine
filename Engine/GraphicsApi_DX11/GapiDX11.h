#pragma once
#include "..\IHighLevelGapi.h"
#include "..\..\GraphicsEngine\ITexture.h"
#include "..\IBuffer.h"


class GapiDX11 : public IHighLevelGapi
{
public:
	GapiDX11();

	// shader creation
	IShaderProgram* CreateShaderSource( const char* vertex_shader_source,
										const char* pixel_shader_source,
										const char* geometry_shader_source = nullptr,
										const char* tess_control_shader_source = nullptr,
										const char* tess_eval_shader_source = nullptr) override { return 0; }

	IShaderProgram* CreateShaderBinary(void* data, size_t size) override { return 0; }

	// buffer creationg
	IUniformBuffer* CreateUniformBuffer(const rBuffer& data) override { return 0; }
	IVertexBuffer*	CreateVertexBuffer(const rBuffer& data) override { return 0; }
	ITextureGapi*	CreateTexture(const rTextureGapi& data) override { return 0; }
	IIndexBuffer*	CreateIndexBuffer(const rBuffer& data) override { return 0; }

	// buffer data manipulation
	void WriteTexture(ITextureGapi* t, const rTextureUpdate& d) override {}
	void writeTexture(ITextureGapi* t, const rTextureUpdate& d) override {}

	void WriteBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) override {}
	void ReadBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) override {}
	void WriteBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) override {}
	void ReadBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) override {}
	void WriteBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) override {}
	void ReadBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) override {}

	// rendereing states
	void ResetStatesToDefault() override {}
	void SetDepthState(const rDepthState& state) override {}
	void SetStencilState(const rStencilState& state) override {}
	void SetBlendState(const rBlendState& state) override {}
	void SetRasterizationState(const rRasterizerState& state) override {}
	void SetSamplerState(const char* slotName, const rSamplerState& smpdata, ITextureGapi* t) override {} // move to SetTexture

	void SetSRGBWrites(bool val) override {} // move to texture parameters
	void SetSeamlessCubeMaps(bool val) override {} // should be true by default, shouldn't it?  

	void SetViewport(int x, int y, u32 w, u32 h) override {}

	// debug
	bool GetError() override { return false; }
	void SetDebugOutput(bool val) override {}
	void SetSyncDebugOutput(bool val) override {}

	// Set render input
	void SetShaderProgram(IShaderProgram* sp) override {}
	void SetRenderTargets(const rRenderTargetInfo* render_targets, u32 size) override {}
	void SetUniformBuffer(IUniformBuffer* buf, u32 index) override {}
	void SetVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers) override {}
	void SetIndexBuffer(IIndexBuffer* ibo) override {}
	void SetTexture(ITextureGapi* t, u32 idx) override {}

	// Draw calls
	void DrawIndexed(u32 num_indices, u32 index_byte_offset = 0) override {}
	void Draw(u32 nVertices, u32 offset = 0) override {}

	void ClearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth = 0, i32 stencil = 0) override {}

	// input layout & vertex streams
	u32 GetNumVertexBufferSlots() const override { return 0; }

	void SetVertexBuffers(	const IVertexBuffer* const * buffers,
							u32* strides,
							u32* offsets,
							u32 start_slot,
							u32 num_buffers) override {}

	IInputLayout* CreateInputLayout(rInputElement* elements, size_t num_elements) override { return 0; }
	void SetInputLayout(IInputLayout* layout) override {}
};