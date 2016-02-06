// GraphicsD3D11.h By Zsíroskenyér team 2013.10.10 1:32 last modified Németh Richárd
// IGraphicsApi interface implementation based on the 3D graphics SDK DirectX11
#pragma once
#ifdef _MSC_VER
#pragma warning(disable: 4244)
#pragma warning(disable: 4005)
#endif

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#undef ERROR_FILE_NOT_FOUND // fucking dx collides with eGapiResult::ERROR_FILE_NOT_FOUND
#undef max // fasz kivan már ezzel a buzi windowssal

#include "../../Core/src/GAPI.h"
#include "../../Core/src/common.h"

#include <unordered_map>
#include "GraphicsApi/IGapi.h"
#include "../../Core/src/math/Vec4.h"
#include "PlatformLibrary/Window.h"
#include <map>

class IVertexBuffer;
class IIndexBuffer;
class IConstantBuffer;
class IShaderProgram;
class IGraphicsApi;
class cTexture2DD3D11;
class cShaderProgramD3D11;
class cVertexBufferD3D11;

enum class ePrimitiveTopology {
	LINE_LIST,
	TRIANGLE_LIST,
};

// input layout lazy creation
struct ILHasher {
	 size_t operator() (const std::pair<cVertexFormat, cVertexFormat>& obj) const {
		return std::hash<int64_t>()(obj.first.Raw() ^ obj.second.Raw());
	}
};

class cGraphicsApiD3D11 : public IGapi {
public:
	cGraphicsApiD3D11();
	~cGraphicsApiD3D11();
	void Release();

	// --- resources --- //
	eGapiResult	CreateVertexBuffer(IVertexBuffer** resource, eUsage usage, cVertexFormat format, size_t size, void* data = nullptr);

	virtual IVertexBuffer* CreateVertexBuffer(const rBuffer& data) override;

	eGapiResult	CreateIndexBuffer(IIndexBuffer** resource, eUsage usage, size_t size, void* data = nullptr);

	virtual IIndexBuffer* CreateIndexBuffer(const rBuffer& data) override;

	eGapiResult CreateTexture(ITexture2D** resource, const wchar_t* filePath);
	eGapiResult CreateTexture(ITexture2D** resource, const ITexture2D::tDesc& desc, void* data = nullptr);

	virtual ITextureGapi* CreateTexture(const rTextureGapi& data) override;

	eGapiResult CreateShaderProgram(IShaderProgram** resource, const wchar_t* shaderPath);

	eGapiResult WriteResource(IIndexBuffer* buffer, void* source, size_t size = ZS_NUMERIC_MAX(size_t), size_t offset = 0);
	eGapiResult WriteResource(IVertexBuffer* buffer, void* source, size_t size = ZS_NUMERIC_MAX(size_t), size_t offset = 0);

	eGapiResult ReadResource(IIndexBuffer* buffer, void* dest, size_t size, size_t offset = 0);
	eGapiResult ReadResource(IVertexBuffer* buffer, void* dest, size_t size, size_t offset = 0);
	eGapiResult ReadResource(ITexture2D* texture, void* dest, size_t size, size_t offset = 0);

	eGapiResult CopyResource(ITexture2D* src, ITexture2D* dst);

	// --- render --- //
	void Clear(bool target = true, bool depth = false, bool stencil = false, const Vec4& clearColor = Vec4(0.3f, 0.3f, 0.3f, 1.0f));
	void ClearTexture(ITexture2D* t, eClearFlag clearFlag = eClearFlag::COLOR_DEPTH_STENCIL, const Vec4& clearColor = Vec4(), float depthVal = 1.0f, size_t stencilVal = 0);

	virtual void Draw(u32 nVertices, u32 offset = 0) override;

	void DrawIndexed(size_t nIndices, size_t idxStartIndex = 0);

	virtual void DrawIndexed(u32 num_indices, u32 index_byte_offset = 0) override;

	void DrawInstanced(size_t nVerticesPerInstance, size_t nInstances, size_t idxStartVertex = 0, size_t idxStartInstance = 0);
	void DrawInstancedIndexed(size_t nIndicesPerInstance, size_t nInstances, size_t idxStartIndex = 0, size_t idxStartInstance = 0);

	void Present();

	// --- state --- //
	eGapiResult SetRenderTargetDefault();
	eGapiResult SetBackBufferSize(unsigned width, unsigned height);
	eGapiResult SetRenderTargets(unsigned nTargets, const ITexture2D* const* renderTargets, ITexture2D* depthStencilTarget = nullptr);

	virtual void SetRenderTargets(const rRenderTargetInfo* render_targets, u32 size) override;

	eGapiResult SetVSConstantBuffer(const void* data, size_t size, size_t slotIdx);
	eGapiResult SetPSConstantBuffer(const void* data, size_t size, size_t slotIdx);
	eGapiResult SetTextureArray(const wchar_t* varName, const ITextureGapi* const * t, uint8_t nTextures = 1);
	eGapiResult SetTexture(int slotIdx, const ITexture2D* t);
	eGapiResult SetTexture(const wchar_t* varName, const ITextureGapi* t);

	virtual void SetTexture(ITextureGapi* t, u32 idx) override;

	void SetPrimitiveTopology(ePrimitiveTopology t);
	void SetShaderProgram(IShaderProgram* shProg);
	void SetInstanceData(/*whatever*/);
	void SetVertexBuffer(const IVertexBuffer* vb);
	void SetVertexBuffers(
		const IVertexBuffer* const * buffers,
		u32* strides,
		u32* offsets,
		u32 start_slot,
		u32 num_buffers);

	virtual void SetVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers) override;

	void SetIndexBuffer(const IIndexBuffer* ib);

	virtual void SetIndexBuffer(IIndexBuffer* ibo) override;

	eGapiResult SetDepthStencilState(const tDepthStencilDesc& desc, uint8_t stencilRef);
	eGapiResult SetBlendState(const tBlendDesc& desc);

	virtual void SetBlendState(const rBlendState& state) override;

	// --- misc --- //
	eGapiResult SetWindow(Window *renderWindow);


	// --- misc --- //
	ITexture2D* GetDefaultRenderTarget() const;
	const wchar_t* GetLastErrorMsg() const;

	// --- utility --- //
	eGapiResult SaveTextureToFile(ITexture2D* t, ITexture2D::eImageFormat f, const char* filePath);

	eGapiResult GenerateMips(ITexture2D* t);

private:
	eGapiResult CreateDevice();
	eGapiResult CreateMostAcceptableSwapChain(size_t width, size_t height, HWND windowHandle);
	eGapiResult CreateViewsForBB();
	eGapiResult CreateDefaultStates();
	void ApplyConstantBuffers();
	void ApplySamplerStates();

	HRESULT CompileShaderFromFile(const zsString& fileName, const zsString& entry, const zsString& profile, zsString* compilerMessage, ID3DBlob** ppBlobOut);
	void AutoSetInputLayout(cShaderProgramD3D11* shader, cVertexBufferD3D11* buffer);
	ID3D11InputLayout* GetInputLayout(cShaderProgramD3D11* shader, cVertexFormat bufferFormat);

	// Buffer convert
	DXGI_FORMAT ConvertToNativeFormat(eFormat fmt);
	unsigned ConvertToNativeBind(unsigned flags);
	D3D11_USAGE ConvertToNativeUsage(eUsage usage);

	// Blend convert
	D3D11_BLEND_OP ConvertToNativeBlendOp(eBlendOp blendOp);
	D3D11_BLEND ConvertToNativeBlendFactor(eBlendFactor blendFactor);
	uint8_t ConvertToNativeBlendMask(eBlendWriteMask blendMask);
	D3D11_BLEND_DESC ConvertToNativeBlend(tBlendDesc blend);

	// Depthstencil convert
	D3D11_COMPARISON_FUNC ConvertToNativeCompFunc(eComparisonFunc compFunc);
	D3D11_STENCIL_OP ConvertToNativeStencilOp(eStencilOp stencilOp);
	D3D11_DEPTH_STENCIL_DESC ConvertToNativeDepthStencil(const tDepthStencilDesc& depthStencil);

	// Sampler convert
	D3D11_SAMPLER_DESC ConvertToNativeSampler(const tSamplerStateDesc& sDesc);

	// Vertex format
	std::vector<D3D11_INPUT_ELEMENT_DESC> ConvertToNativeVertexFormat(cVertexFormat format);

	virtual IShaderProgram* CreateShaderSource(const char* vertex_shader_source, const char* pixel_shader_source, const char* geometry_shader_source = nullptr, const char* tess_control_shader_source = nullptr, const char* tess_eval_shader_source = nullptr) override;

	virtual IShaderProgram* CreateShaderBinary(void* data, size_t size) override;

	virtual IUniformBuffer* CreateUniformBuffer(const rBuffer& data) override;

	virtual void WriteBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) override;

	virtual void WriteBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) override;

	virtual void WriteBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) override;

	virtual void ReadBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) override;

	virtual void ReadBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) override;

	virtual void ReadBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) override;

	virtual void ResetStatesToDefault() override;

	virtual void SetDepthState(const rDepthState& state) override;

	virtual void SetStencilState(const rStencilState& state) override;

	virtual void SetRasterizationState(const rRasterizerState& state) override;

	virtual void SetSamplerState(const char* slotName, const rSamplerState& smpdata, ITextureGapi* t) override;

	virtual void SetSRGBWrites(bool val) override;

	virtual void SetSeamlessCubeMaps(bool val) override;

	virtual void SetViewport(int x, int y, unsigned w, unsigned h) override;

	virtual bool GetError() override;

	virtual void SetDebugOutput(bool val) override;

	virtual void SetSyncDebugOutput(bool val) override;

	virtual void SetUniformBuffer(IUniformBuffer* buf, u32 idx) override;

	virtual void ClearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth = 0, i32 stencil = 0) override;

	virtual u32 GetNumVertexBufferSlots() const override;

	virtual IInputLayout* CreateInputLayout(rInputElement* elements, size_t num_elements) override;

	virtual void SetInputLayout(IInputLayout* layout) override;

	virtual void WriteTexture(ITextureGapi* t, const rTextureUpdate& d) override;

protected:
	// Error handling
	zsString lastErrorMsg;
	
	// backBuffer will be the main render target
	cTexture2DD3D11* defaultRenderTarget;
	D3D11_VIEWPORT defaultVP;

	// Main Dx interfaces
	ID3D11DeviceContext *d3dcon;
	ID3D11Device *d3ddev;
	IDXGISwapChain *d3dsc;


	// Draw state stuff
	cShaderProgramD3D11* activeShaderProg;
	cVertexBufferD3D11* activeVertexBuffer;

	// Viewports and RTs
	D3D11_VIEWPORT	activeViewports[16];
	ID3D11RenderTargetView* activeRTVs[16];

	// Constant buffers handling
	void* vsConstBufferData; // These updated every ex. SetPSConstantBuffer(.....) call
	void* psConstBufferData;
	size_t vsConstBufferSize;
	size_t psConstBufferSize;
	ID3D11Buffer* vsConstBuffer; // Then these gets updates when Draw called
	ID3D11Buffer* psConstBuffer;
	bool vsConstBufferStateChanged; // Don't want to apply constant buffers when it's not changed
	bool psConstBufferStateChanged;

	// Inner state descriptor, for avoid hashing
	struct tDepthStencilLink {
		D3D11_DEPTH_STENCIL_DESC desc;
		ID3D11DepthStencilState* state;
	};
	struct tBlendLink {
		D3D11_BLEND_DESC desc;
		ID3D11BlendState* state;
	};
	struct tSamplerLink {
		D3D11_SAMPLER_DESC desc;
		ID3D11SamplerState* state;
	};

	std::vector<tDepthStencilLink> depthStencilStates;
	std::vector<tBlendLink> blendStates;
	std::vector<tSamplerLink> samplerStates;

	using InputLayoutMapT = std::unordered_map<std::pair<cVertexFormat, cVertexFormat>, ID3D11InputLayout*, ILHasher>;
	//typedef std::map<std::pair<cVertexFormat, cVertexFormat>, ID3D11InputLayout*> InputLayoutMapT;
	InputLayoutMapT inputLayoutStore;
};