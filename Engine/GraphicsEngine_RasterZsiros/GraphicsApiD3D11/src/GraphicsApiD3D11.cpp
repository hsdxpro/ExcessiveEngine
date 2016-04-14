#include "../../Core/src/PipelineState.h"

#include "GraphicsApiD3D11.h"

#include "Dx11_SDK/d3dx11.h"

#include "VertexBufferD3D11.h"
#include "IndexBufferD3D11.h"
#include "ShaderProgramD3D11.h"
#include "Texture2DD3D11.h"

#include "../../Core/src/common.h"
#include "../../Core/src/StrUtil.h"
#include "../../Core/src/FileUtil.h"

#include "../../Core/src/CgShaderHelper.h"
#include "../../../PlatformLibrary/Window.h"
#include <unordered_map>
#include <fstream>
#include <memory>
#include <memory>




uint16_t eFormatBitSize(eFormat f) {
	static const std::unordered_map<eFormat, uint16_t> lookupTable = {
		{ eFormat::UNKNOWN, 0 },
		{ eFormat::R32G32B32A32_TYPELESS, 128 },
		{ eFormat::R32G32B32A32_FLOAT, 128 },
		{ eFormat::R32G32B32A32_UINT, 128 },
		{ eFormat::R32G32B32A32_SINT, 128 },
		{ eFormat::R32G32B32_TYPELESS, 96 },
		{ eFormat::R32G32B32_FLOAT, 96 },
		{ eFormat::R32G32B32_UINT, 96 },
		{ eFormat::R32G32B32_SINT, 96 },
		{ eFormat::R16G16B16A16_TYPELESS, 64 },
		{ eFormat::R16G16B16A16_FLOAT, 64 },
		{ eFormat::R16G16B16A16_UNORM, 64 },
		{ eFormat::R16G16B16A16_UINT, 64 },
		{ eFormat::R16G16B16A16_SNORM, 64 },
		{ eFormat::R16G16B16A16_SINT, 64 },
		{ eFormat::R32G32_TYPELESS, 64 },
		{ eFormat::R32G32_FLOAT, 64 },
		{ eFormat::R32G32_UINT, 64 },
		{ eFormat::R32G32_SINT, 64 },
		{ eFormat::R32G8X24_TYPELESS, 64 },
		{ eFormat::D32_FLOAT_S8X24_UINT, 32 },
		{ eFormat::R32_FLOAT_X8X24_TYPELESS, 32 },
		{ eFormat::X32_TYPELESS_G8X24_UINT, 32 },
		{ eFormat::R10G10B10A2_TYPELESS, 32 },
		{ eFormat::R10G10B10A2_UNORM, 32 },
		{ eFormat::R10G10B10A2_UINT, 32 },
		{ eFormat::R11G11B10_FLOAT, 32 },
		{ eFormat::R8G8B8A8_TYPELESS, 32 },
		{ eFormat::R8G8B8A8_UNORM, 32 },
		{ eFormat::R8G8B8A8_UNORM_SRGB, 32 },
		{ eFormat::R8G8B8A8_UINT, 32 },
		{ eFormat::R8G8B8A8_SNORM, 32 },
		{ eFormat::R8G8B8A8_SINT, 32 },
		{ eFormat::R16G16_TYPELESS, 32 },
		{ eFormat::R16G16_FLOAT, 32 },
		{ eFormat::R16G16_UNORM, 32 },
		{ eFormat::R16G16_UINT, 32 },
		{ eFormat::R16G16_SNORM, 32 },
		{ eFormat::R16G16_SINT, 32 },
		{ eFormat::R32_TYPELESS, 32 },
		{ eFormat::D32_FLOAT, 32 },
		{ eFormat::R32_FLOAT, 32 },
		{ eFormat::R32_UINT, 32 },
		{ eFormat::R32_SINT, 32 },
		{ eFormat::R24G8_TYPELESS, 32 },
		{ eFormat::D24_UNORM_S8_UINT, 32 },
		{ eFormat::R24_UNORM_X8_TYPELESS, 32 },
		{ eFormat::X24_TYPELESS_G8_UINT, 32 },
		{ eFormat::R8G8_TYPELESS, 16 },
		{ eFormat::R8G8_UNORM, 16 },
		{ eFormat::R8G8_UINT, 16 },
		{ eFormat::R8G8_SNORM, 16 },
		{ eFormat::R8G8_SINT, 16 },
		{ eFormat::R16_TYPELESS, 16 },
		{ eFormat::R16_FLOAT, 16 },
		{ eFormat::D16_UNORM, 16 },
		{ eFormat::R16_UNORM, 16 },
		{ eFormat::R16_UINT, 16 },
		{ eFormat::R16_SNORM, 16 },
		{ eFormat::R16_SINT, 16 },
		{ eFormat::R8_TYPELESS, 8 },
		{ eFormat::R8_UNORM, 8 },
		{ eFormat::R8_UINT, 8 },
		{ eFormat::R8_SNORM, 8 },
		{ eFormat::R8_SINT, 8 },
		{ eFormat::A8_UNORM, 8 },
		{ eFormat::R1_UNORM, 8 },
		{ eFormat::R9G9B9E5_SHAREDEXP, 32 },
		{ eFormat::R8G8_B8G8_UNORM, 32 },
		{ eFormat::G8R8_G8B8_UNORM, 32 },
		{ eFormat::BC1_TYPELESS, 4 },
		{ eFormat::BC1_UNORM, 4 },
		{ eFormat::BC1_UNORM_SRGB, 4 },
		{ eFormat::BC2_TYPELESS, 8 },
		{ eFormat::BC2_UNORM, 8 },
		{ eFormat::BC2_UNORM_SRGB, 8 },
		{ eFormat::BC3_TYPELESS, 8 },
		{ eFormat::BC3_UNORM, 8 },
		{ eFormat::BC3_UNORM_SRGB, 8 },
		{ eFormat::BC4_TYPELESS, 4 },
		{ eFormat::BC4_UNORM, 4 },
		{ eFormat::BC4_SNORM, 4 },
		{ eFormat::BC5_TYPELESS, 8 },
		{ eFormat::BC5_UNORM, 8 },
		{ eFormat::BC5_SNORM, 8 },
		{ eFormat::B5G6R5_UNORM, 16 },
		{ eFormat::B5G5R5A1_UNORM, 16 },
		{ eFormat::B8G8R8A8_UNORM, 32 },
		{ eFormat::B8G8R8X8_UNORM, 32 },
		{ eFormat::R10G10B10_XR_BIAS_A2_UNORM, 32 },
		{ eFormat::B8G8R8A8_TYPELESS, 32 },
		{ eFormat::B8G8R8A8_UNORM_SRGB, 32 },
		{ eFormat::B8G8R8X8_TYPELESS, 32 },
		{ eFormat::B8G8R8X8_UNORM_SRGB, 32 },
		{ eFormat::BC6H_TYPELESS, 8 },
		{ eFormat::BC6H_UF16, 8 },
		{ eFormat::BC6H_SF16, 8 },
		{ eFormat::BC7_TYPELESS, 8 },
		{ eFormat::BC7_UNORM, 8 },
		{ eFormat::BC7_UNORM_SRGB, 8 },
		{ eFormat::AYUV, 8 },
		{ eFormat::Y410, 8 },
		{ eFormat::Y416, 8 },
		{ eFormat::NV12, 8 },
		{ eFormat::P010, 8 },
		{ eFormat::P016, 8 },
		{ eFormat::_420_OPAQUE, 8 },
		{ eFormat::YUY2, 8 },
		{ eFormat::Y210, 8 },
		{ eFormat::Y216, 8 },
		{ eFormat::NV11, 8 },
		{ eFormat::AI44, 8 },
		{ eFormat::IA44, 8 },
		{ eFormat::P8, 8 },
		{ eFormat::A8P8, 16 },
		{ eFormat::B4G4R4A4_UNORM, 16 },
	};
	auto it = lookupTable.find(f);
	assert(it != lookupTable.end());
	return it->second;
}

cGraphicsApiD3D11::cGraphicsApiD3D11()
: 
d3ddev(nullptr),
d3dcon(nullptr),
d3dsc(nullptr),
defaultRenderTarget(nullptr),
activeShaderProg(nullptr),
activeVertexBuffer(nullptr)
{
	// Create d3ddevice, d3dcontext
	eGapiResult r = CreateDevice();
	if (r != eGapiResult::OK) {
		throw std::runtime_error("failed to create direct3d device");
	}

	// Const buffer handling init
	vsConstBuffer = psConstBuffer = nullptr;
	vsConstBufferData = psConstBufferData = nullptr;
	vsConstBufferSize = psConstBufferSize = 0;
	vsConstBufferStateChanged = psConstBufferStateChanged = false;
	
	// Create default states
	r = CreateDefaultStates();
	if (r != eGapiResult::OK) {
		throw std::runtime_error("failed to create default states");
	}

	memset(activeViewports, 0, sizeof(D3D11_VIEWPORT) * 16);
	memset(activeRTVs	  , 0, sizeof(ID3D11RenderTargetView*) * 16);
}

void cGraphicsApiD3D11::Release() {
	delete this;
}
cGraphicsApiD3D11::~cGraphicsApiD3D11() {
	for (size_t i = 0; i < depthStencilStates.size(); i++)
		SAFE_RELEASE(depthStencilStates[i].state);

	for (size_t i = 0; i < blendStates.size(); i++)
		SAFE_RELEASE(blendStates[i].state);

	for (size_t i = 0; i < samplerStates.size(); i++)
		SAFE_RELEASE(samplerStates[i].state);

	ID3D11ShaderResourceView* nullSrvs[16] = { 0 };
	d3dcon->PSSetShaderResources(0, 16, nullSrvs);
	d3dcon->VSSetShaderResources(0, 16, nullSrvs);

	ID3D11SamplerState* nullSamplers[16] = { 0 };
	d3dcon->VSSetSamplers(0, 16, nullSamplers);
	d3dcon->PSSetSamplers(0, 16, nullSamplers);

	ID3D11RenderTargetView *nulltargets[4] = { 0 };
	d3dcon->OMSetRenderTargets(4, nulltargets, 0);

	for (size_t i = 0; i < 16; i++)
		SAFE_RELEASE(activeRTVs[i]);

	if (d3dcon)d3dcon->ClearState();
	if (d3dcon)d3dcon->Flush();

	SAFE_DELETE_ARRAY(vsConstBufferData);
	SAFE_DELETE_ARRAY(psConstBufferData);
	SAFE_RELEASE(vsConstBuffer);
	SAFE_RELEASE(psConstBuffer);
	SAFE_RELEASE(defaultRenderTarget);
	SAFE_RELEASE(d3ddev);
	SAFE_RELEASE(d3dcon);
	SAFE_RELEASE(d3dsc);
}


////////////////////////////////////////////////////////////////////////////////
// Direct3D helper functions

eGapiResult cGraphicsApiD3D11::CreateDevice() {
	// create Graphic Infrastructure factory
	IDXGIFactory* fact = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&fact);

	// enumerate adapters
	IDXGIAdapter* mainAdapter = nullptr;
	UINT adapterIndex = 0;
	fact->EnumAdapters(adapterIndex, &mainAdapter);

	// feature level array
	const D3D_FEATURE_LEVEL featurelevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// go from high feature level to lower, try to create device with each
	D3D_FEATURE_LEVEL featurelevel;
	int i = 0;
	const int nFeatureLevels = ARRAYSIZE(featurelevels);
	for (; i < nFeatureLevels; i++, featurelevel = featurelevels[i]) {
		HRESULT hr = D3D11CreateDevice(mainAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, 0, featurelevels, nFeatureLevels, D3D11_SDK_VERSION, &d3ddev, &featurelevel, &d3dcon);

		// Wrap mode... for gpu's not supporting hardware accelerated D3D11
		//HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP , 0, flags, featurelevels, nFeatureLevels, D3D11_SDK_VERSION, &d3ddev, &featurelevel, &d3dcon); // For dx9 machines
		if (!FAILED(hr))
			break;
		else {
			if (hr == E_OUTOFMEMORY)
				return eGapiResult::ERROR_OUT_OF_MEMORY;
		}
	}

	if (i == ARRAYSIZE(featurelevels)) {
		ASSERT_MSG(false, L"Can't create DirectX Device");
		return eGapiResult::ERROR_UNKNOWN;
	}

	SAFE_RELEASE(fact);
	SAFE_RELEASE(mainAdapter);

	return eGapiResult::OK;
}

eGapiResult cGraphicsApiD3D11::CreateMostAcceptableSwapChain(size_t width, size_t height, HWND windowHandle) {
	if (d3dsc != nullptr)
		SAFE_RELEASE(d3dsc);

	DXGI_FORMAT bbFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// create Graphic Infrastructure factory
	IDXGIFactory* fact = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&fact);

	// enumerate adapters
	IDXGIAdapter* mainAdapter = nullptr;
	UINT adapterIndex = 0;
	fact->EnumAdapters(adapterIndex, &mainAdapter);

	// enumerate outputs
	IDXGIOutput* mainOutput = nullptr;
	DXGI_MODE_DESC* modeDesc = nullptr;
	UINT outputIndex = 0;
	mainAdapter->EnumOutputs(outputIndex, &mainOutput);

	// how many displayModes ?
	UINT numModes = 0;
	mainOutput->GetDisplayModeList(bbFormat, 0, &numModes, 0);

	// get those displayModes
	modeDesc = new DXGI_MODE_DESC[numModes];
	mainOutput->GetDisplayModeList(bbFormat, 0, &numModes, modeDesc);

	// select displayModes that matches our renderWindow params and DirectX Config
	// worst case sized array
	DXGI_MODE_DESC** filteredVideoModes = new DXGI_MODE_DESC*[numModes];

	UINT displayModeIndex = 0;
	for (size_t i = 0; i < numModes; i++) {
		DXGI_MODE_DESC* currMode = &modeDesc[i];
		/*
		// Collect The best resolution that the video card handle
		if (config.createDeviceAtMaxResolution) {
			// add to matched videoModes if that VideoMode have full screen resolution
			if (GetSystemMetrics(SM_CXSCREEN) == currMode->Width && GetSystemMetrics(SM_CYSCREEN) == currMode->Height) {
				filteredVideoModes[displayModeIndex] = currMode;
				displayModeIndex++;
			}
			// Collect videoModes which resoltuion is equal with the window passed to ManagerDx

		}*/
		if (currMode->Width == width && currMode->Height == height) {
			filteredVideoModes[displayModeIndex] = currMode;
			displayModeIndex++;
		}
	}

	// select highest HZ videoMode
	DXGI_MODE_DESC* selectedVideoMode = 0;
	UINT maxHz = 0;
	for (size_t i = 0; i < displayModeIndex; i++) {
		DXGI_MODE_DESC* curr = filteredVideoModes[i];
		if (maxHz < curr->RefreshRate.Numerator) {
			maxHz = curr->RefreshRate.Numerator;
			selectedVideoMode = curr;
		}
	}

	// Use the selected display mode to fill swap chain description
	DXGI_SWAP_CHAIN_DESC sdesc;
	sdesc.BufferCount = 1;
	if (selectedVideoMode != nullptr) {
		sdesc.BufferDesc = *selectedVideoMode; // Copy DisplayMode Data
	}
	else {
		ASSERT_MSG(false, L"Using non standard resolution, this may slow down the DirectX application");
		sdesc.BufferDesc.Format = bbFormat;
		sdesc.BufferDesc.Width = width;
		sdesc.BufferDesc.Height = height;
		sdesc.BufferDesc.RefreshRate.Numerator = 1;
		sdesc.BufferDesc.RefreshRate.Denominator = 0;
		sdesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sdesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	}
	sdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	sdesc.OutputWindow = windowHandle;
	sdesc.SampleDesc.Count = 1;
	sdesc.SampleDesc.Quality = 0;
	sdesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sdesc.Windowed = true;// Sys::GetScreenSize().x != width || Sys::GetScreenSize().y != height;

	HRESULT hr = fact->CreateSwapChain(d3ddev, &sdesc, &d3dsc);

	// Go full screen if backbuffer size is equals to the screen size
	if (Sys::GetScreenSize().x == width && Sys::GetScreenSize().y == height)
	{
		d3dsc->SetFullscreenState(true, NULL);
	}

	//hr = fact->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

	// free up everything
	SAFE_RELEASE(mainOutput);
	SAFE_RELEASE(mainAdapter);
	SAFE_RELEASE(fact);

	SAFE_DELETE_ARRAY(filteredVideoModes);
	SAFE_DELETE_ARRAY(modeDesc);

	switch (hr) {
	case S_OK:
		return eGapiResult::OK;
	case E_OUTOFMEMORY:
		return eGapiResult::ERROR_OUT_OF_MEMORY;
	default:
		return eGapiResult::ERROR_UNKNOWN;
	}
}

eGapiResult cGraphicsApiD3D11::CreateViewsForBB() {
	SAFE_RELEASE(defaultRenderTarget);

	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
	ID3D11DepthStencilView* dsv;

	ID3D11Texture2D *backBuffer = nullptr;
	HRESULT hr = d3dsc->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&backBuffer);
	if (FAILED(hr)) {
		ASSERT_MSG(false, L"Failed to Get SwapChain buffer");
		return eGapiResult::ERROR_UNKNOWN;
	}
	hr = d3ddev->CreateRenderTargetView(backBuffer, 0, &rtv);
	if (FAILED(hr)) {
		ASSERT_MSG(false, L"Failed to create render target view for SwapChain's BackBuffer");
		if (hr == E_OUTOFMEMORY)
			return eGapiResult::ERROR_OUT_OF_MEMORY;
		else
			return eGapiResult::ERROR_UNKNOWN;
	}

	hr = d3ddev->CreateShaderResourceView(backBuffer, nullptr, &srv);
	if (FAILED(hr)) {
		ASSERT_MSG(false, L"Failed to create shader resource view for SwapChain's BackBuffer");
		if (hr == E_OUTOFMEMORY)
			return eGapiResult::ERROR_OUT_OF_MEMORY;
		else
			return eGapiResult::ERROR_UNKNOWN;
	}

	D3D11_TEXTURE2D_DESC bbDesc;
	backBuffer->GetDesc(&bbDesc);

	// create Depth texture
	ID3D11Texture2D *depthTexture = nullptr;
	D3D11_TEXTURE2D_DESC tD;
	tD.ArraySize = 1;
	tD.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tD.CPUAccessFlags = 0;
	tD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tD.Width = bbDesc.Width;
	tD.Height = bbDesc.Height;
	tD.MipLevels = 1;
	tD.MiscFlags = 0;
	tD.SampleDesc.Count = 1;// config.multiSampleCount;
	tD.SampleDesc.Quality = 0;// config.multiSampleQuality;
	tD.Usage = D3D11_USAGE_DEFAULT;
	hr = d3ddev->CreateTexture2D(&tD, 0, &depthTexture);
	if (FAILED(hr)) {
		SAFE_RELEASE(backBuffer);

		ASSERT_MSG(false, L"Failed to create depth buffer for swapChain");

		if (hr == E_OUTOFMEMORY)
			return eGapiResult::ERROR_OUT_OF_MEMORY;
		else
			return eGapiResult::ERROR_UNKNOWN;
	}

	// create DepthStencilView
	hr = d3ddev->CreateDepthStencilView(depthTexture, nullptr, &dsv);

	if (FAILED(hr)) {
		ASSERT_MSG(false, L"Failed to create depth buffer VIEW for swapChain");
		SAFE_RELEASE(depthTexture);
		SAFE_RELEASE(backBuffer);
		if (hr == E_OUTOFMEMORY)
			return eGapiResult::ERROR_OUT_OF_MEMORY;
		else
			return eGapiResult::ERROR_UNKNOWN;
	}

	defaultRenderTarget = new cTexture2DD3D11(this, bbDesc.Width, bbDesc.Height, backBuffer, srv, rtv, dsv);

	SAFE_RELEASE(depthTexture);

	return eGapiResult::OK;
}


eGapiResult cGraphicsApiD3D11::CreateDefaultStates() {

	// Default geometry topology
	d3dcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Default samplers DESC
	D3D11_SAMPLER_DESC d;
	d.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	d.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d.MaxAnisotropy = 1;
	d.MaxLOD = 1;
	d.MinLOD = 0;
	d.MipLODBias = 0;

	static ID3D11SamplerState* defaultSamplers[16];
	// Create 16 default sampler lol..
	for (size_t i = 0; i < 16; i++)
	{
		HRESULT hr = d3ddev->CreateSamplerState(&d, &defaultSamplers[i]);

		if (FAILED(hr)) {
			ASSERT_MSG(false, L"Failed to create default sampler state");
			if (hr == E_OUTOFMEMORY)
				return eGapiResult::ERROR_OUT_OF_MEMORY;
			else
				return eGapiResult::ERROR_UNKNOWN;
		}

	}

	// Set default samplers
	d3dcon->PSSetSamplers(0, 16, defaultSamplers);


	ID3D11RasterizerState* pRasterState;
	D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = true;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;
	HRESULT hr = d3ddev->CreateRasterizerState(&rasterDesc, &pRasterState);
	assert(hr == S_OK);
	d3dcon->RSSetState(pRasterState);
	return eGapiResult::OK;
}

void cGraphicsApiD3D11::ApplyConstantBuffers() {
	D3D11_MAPPED_SUBRESOURCE mapped;
	// Update vertex shader constants
	if (vsConstBufferStateChanged && vsConstBuffer) {
		d3dcon->Map(vsConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			memcpy(mapped.pData, vsConstBufferData, vsConstBufferSize);
		d3dcon->Unmap(vsConstBuffer, 0);
	}
	
	if (psConstBufferStateChanged && psConstBuffer) {
		// Update pixel shader constants
		d3dcon->Map(psConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			memcpy(mapped.pData, psConstBufferData, psConstBufferSize);
		d3dcon->Unmap(psConstBuffer, 0);
	}

	vsConstBufferStateChanged = psConstBufferStateChanged = false;
}

void cGraphicsApiD3D11::ApplySamplerStates() {
	// Set VertexShader samplers
	auto vsSamplerStates = activeShaderProg->GetSamplerStatesVS();
	for (size_t i = 0; i < vsSamplerStates.size(); i++) {
		d3dcon->VSSetSamplers(vsSamplerStates[i].slot, 1, &vsSamplerStates[i].state);
	}

	// Set PixelShader samplers
	auto psSamplerStates = activeShaderProg->GetSamplerStatesPS();
	for (size_t i = 0; i < psSamplerStates.size(); i++) {
		d3dcon->PSSetSamplers(psSamplerStates[i].slot, 1, &psSamplerStates[i].state);
	}
}

////////////////////////////////////////////////////////////////////////////////
//	Manage graphics resources

// Create index and vertex buffes for indexed poly-meshes
eGapiResult	cGraphicsApiD3D11::CreateVertexBuffer(IVertexBuffer** resource, eUsage usage, cVertexFormat format, size_t size, void* data/*= nullptr*/) {
	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = ConvertToNativeUsage(usage);
	desc.CPUAccessFlags = 0;
	if (desc.Usage == D3D11_USAGE_DYNAMIC)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (desc.Usage == D3D11_USAGE_STAGING)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA resData;
	resData.pSysMem = data;
	resData.SysMemPitch = 0;
	resData.SysMemSlicePitch = 0;

	HRESULT hr = d3ddev->CreateBuffer(&desc, data ? &resData : NULL, &buffer);
	switch (hr) {
	case S_OK:
		*resource = new cVertexBufferD3D11(buffer, usage, format, desc.ByteWidth);
		return eGapiResult::OK;
	case E_OUTOFMEMORY:
		return eGapiResult::ERROR_OUT_OF_MEMORY;
	default:
		return eGapiResult::ERROR_UNKNOWN;
	}
}

IVertexBuffer* cGraphicsApiD3D11::CreateVertexBuffer(const rBuffer& data)
{
	cVertexFormat::Attribute attrs[4];
	attrs[0].bitsPerComponent = cVertexFormat::eBits::_32_BIT;
	attrs[0].nComponents = 3;
	attrs[0].semantic = cVertexFormat::POSITION;
	attrs[0].type = cVertexFormat::eType::FLOAT;

	attrs[1].bitsPerComponent = cVertexFormat::eBits::_32_BIT;
	attrs[1].nComponents = 3;
	attrs[1].semantic = cVertexFormat::NORMAL;
	attrs[1].type = cVertexFormat::eType::FLOAT;

	attrs[2].bitsPerComponent = cVertexFormat::eBits::_32_BIT;
	attrs[2].nComponents = 3;
	attrs[2].semantic = cVertexFormat::COLOR;
	attrs[2].type = cVertexFormat::eType::FLOAT;

	attrs[3].bitsPerComponent = cVertexFormat::eBits::_32_BIT;
	attrs[3].nComponents = 2;
	attrs[3].semantic = cVertexFormat::TEXCOORD;
	attrs[3].type = cVertexFormat::eType::FLOAT;

	cVertexFormat dummy;
	dummy.Create(attrs, 4);

	IVertexBuffer* pBuffer;
	CreateVertexBuffer(&pBuffer, eUsage::DYNAMIC, dummy, data.size, data.initial_data);
	//CreateVertexBuffer(&pBuffer, data.is_writable ? eUsage::DYNAMIC : eUsage::DEFAULT, dummy, data.size, data.initial_data);
	return pBuffer;
}

eGapiResult	cGraphicsApiD3D11::CreateIndexBuffer(IIndexBuffer** resource, eUsage usage, size_t size, void* data/*= nullptr*/) {
	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = ConvertToNativeUsage(usage);
	desc.CPUAccessFlags = 0;
	if (desc.Usage == D3D11_USAGE_DYNAMIC)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (desc.Usage == D3D11_USAGE_STAGING)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA resData;
	resData.pSysMem = data;
	resData.SysMemPitch = 0;
	resData.SysMemSlicePitch = 0;

	HRESULT hr = d3ddev->CreateBuffer(&desc, data ? &resData : nullptr, &buffer);
	switch (hr) {
	case S_OK:
		*resource = new cIndexBufferD3D11(buffer, desc.ByteWidth, usage);
		return eGapiResult::OK;
	case E_OUTOFMEMORY:
		return eGapiResult::ERROR_OUT_OF_MEMORY;
	default:
		return eGapiResult::ERROR_UNKNOWN;
	}
}

IIndexBuffer* cGraphicsApiD3D11::CreateIndexBuffer(const rBuffer& data)
{
	IIndexBuffer* pBuffer;
	CreateIndexBuffer( &pBuffer, eUsage::DYNAMIC, data.size, data.initial_data );
	return pBuffer;
}

// Create texture from file
eGapiResult cGraphicsApiD3D11::CreateTexture(ITexture2D** resource, const wchar_t* filePath) {

	// Shader Resource View of texture
	ID3D11ShaderResourceView* srv;
	D3DX11_IMAGE_LOAD_INFO info;
	info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//info.Format = DXGI_FORMAT_BC2_UNORM; // 16 bytes per 16 pixel

	HRESULT hr = D3DX11CreateShaderResourceViewFromFileW(d3ddev, filePath, &info, 0, &srv, 0);

	switch (hr) {
		case S_OK: {
			// Get Width, Height
			size_t width;
			size_t height;

			ID3D11Texture2D* tex2D;
			srv->GetResource((ID3D11Resource**)&tex2D);

			D3D11_TEXTURE2D_DESC texDesc; 
			tex2D->GetDesc(&texDesc);
			tex2D->Release();

			width = texDesc.Width;
			height = texDesc.Height;

			// return
			*resource = new cTexture2DD3D11(this, width, height, tex2D, srv);
			return eGapiResult::OK;
		}
		case D3D11_ERROR_FILE_NOT_FOUND:
			return eGapiResult::ERROR_FILE_NOT_FOUND;
		case E_OUTOFMEMORY:
			return eGapiResult::ERROR_OUT_OF_MEMORY;
		default:
			return eGapiResult::ERROR_UNKNOWN;
	}
}

// Create texture in memory
eGapiResult cGraphicsApiD3D11::CreateTexture(ITexture2D** resource, const ITexture2D::tDesc& desc, void* data /*= nullptr*/) {
	ID3D11Texture2D* tex = nullptr;
	
	// Outputs
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;

	bool isRenderTarget	  = 0 != ((int)desc.bind & (int)eBind::RENDER_TARGET);
	bool isShaderBindable = 0 != ((int)desc.bind & (int)eBind::SHADER_RESOURCE);
	bool hasDepthStencil  = 0 != ((int)desc.bind & (int)eBind::DEPTH_STENCIL);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.ArraySize = desc.arraySize;
	texDesc.BindFlags = ConvertToNativeBind(desc.bind);
	texDesc.Format = ConvertToNativeFormat(desc.format);
	texDesc.Height = desc.height;
	texDesc.Width = desc.width;
	texDesc.MipLevels = desc.mipLevels;
	texDesc.MiscFlags = desc.generateMips && (isShaderBindable || isRenderTarget) ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = ConvertToNativeUsage(desc.usage); // TODO ( PARAMETERIZE )
	texDesc.CPUAccessFlags = [&]()->UINT{
		if (texDesc.Usage == D3D11_USAGE_DYNAMIC)
			return D3D11_CPU_ACCESS_WRITE;
		else if (texDesc.Usage == D3D11_USAGE_STAGING)
			return D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		else
			return 0;
	}();

	HRESULT hr = S_OK;
	// make the format typeless if it has depth:
	if (hasDepthStencil) {
		switch (desc.depthFormat) {
		case eFormat::D16_UNORM:			texDesc.Format = DXGI_FORMAT_R16_TYPELESS;	  break;
		case eFormat::D24_UNORM_S8_UINT:	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;  break;
		case eFormat::D32_FLOAT:			texDesc.Format = DXGI_FORMAT_R32_TYPELESS;	  break;
		case eFormat::D32_FLOAT_S8X24_UINT: texDesc.Format = DXGI_FORMAT_R32G32_TYPELESS; break;
		}
	}

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = data;
	subData.SysMemPitch = desc.width * eFormatBitSize(desc.format);
	subData.SysMemSlicePitch = 0;	

	// create texture resource
	hr = d3ddev->CreateTexture2D(&texDesc, data ? &subData : nullptr, &tex);
	if (FAILED(hr)) {
		lastErrorMsg = L"failed to create resource";
		if (hr == E_OUTOFMEMORY)
			return eGapiResult::ERROR_OUT_OF_MEMORY;
		else
			return eGapiResult::ERROR_INVALID_ARG;
	}

	// view descriptors
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC* pSrvDesc = nullptr;
	// create views as needed
	if (hasDepthStencil) {
		// fill ds view desc
		memset(&dsvDesc, 0, sizeof(dsvDesc));
		dsvDesc.Format = ConvertToNativeFormat(desc.depthFormat);
		dsvDesc.ViewDimension = (desc.arraySize > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		dsvDesc.Texture2DArray.ArraySize = desc.arraySize;

		// fill sr view desc
		memset(&srvDesc, 0, sizeof(srvDesc));
		srvDesc.Format = ConvertToNativeFormat(desc.format);
		srvDesc.ViewDimension = (desc.arraySize > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2DArray.ArraySize = desc.arraySize;
		pSrvDesc = &srvDesc;

		hr = d3ddev->CreateDepthStencilView(tex, &dsvDesc, &dsv);
		if (FAILED(hr)) {
			SAFE_RELEASE(tex);
			lastErrorMsg = L"failed to create depth-stencil view";
			if (hr == E_OUTOFMEMORY)
				return eGapiResult::ERROR_OUT_OF_MEMORY;
			else
				return eGapiResult::ERROR_INVALID_ARG;
		}
	}
	if (isRenderTarget) {
		hr = d3ddev->CreateRenderTargetView(tex, nullptr, &rtv);
		if (FAILED(hr)) {
			SAFE_RELEASE(dsv);
			SAFE_RELEASE(tex);
			lastErrorMsg = L"failed to create render target view";
			if (hr == E_OUTOFMEMORY)
				return eGapiResult::ERROR_OUT_OF_MEMORY;
			else
				return eGapiResult::ERROR_INVALID_ARG;
		}
	}
	if (isShaderBindable) {
		hr = d3ddev->CreateShaderResourceView(tex, pSrvDesc, &srv);
		if (FAILED(hr)) {
			SAFE_RELEASE(rtv);
			SAFE_RELEASE(dsv);
			SAFE_RELEASE(tex);
			lastErrorMsg = L"failed to create shader resource view";
			if (hr == E_OUTOFMEMORY)
				return eGapiResult::ERROR_OUT_OF_MEMORY;
			else
				return eGapiResult::ERROR_INVALID_ARG;
		}
	}

	// create sub-resource views
	std::vector<ID3D11DepthStencilView*> subdsv;
	std::vector<ID3D11ShaderResourceView*> subsrv;
	std::vector<ID3D11RenderTargetView*> subrtv;
	if (desc.arraySize > 1) {

		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsvDesc.Texture2DArray.MipSlice = 0;
		dsvDesc.Texture2DArray.ArraySize = 1;
		
		eGapiResult errorCode = eGapiResult::OK;

		for (size_t i = 0; i < desc.arraySize; i++) {
			if (hasDepthStencil) {
				dsvDesc.Texture2DArray.FirstArraySlice = i;
				hr = d3ddev->CreateDepthStencilView(tex, &dsvDesc, &dsv);
				if (FAILED(hr)) {
					if (hr == E_OUTOFMEMORY)
						errorCode = eGapiResult::ERROR_OUT_OF_MEMORY;
					else
						errorCode = eGapiResult::ERROR_UNKNOWN;
					break;
				}
				subdsv.push_back(dsv);
			}
			if (isRenderTarget) {

			}
			if (isShaderBindable) {

			}
		}
		if (errorCode != eGapiResult::OK) {
			// clear all and return failure
			MessageBoxA(nullptr, "NEM MEGY EZ A FOS", "BAZMEG", MB_OK);
		}
	}
	
	*resource = new cTexture2DD3D11(this, desc.width, desc.height, tex, srv, rtv, dsv, nullptr, nullptr, subdsv.data(), subdsv.size());
	return eGapiResult::OK;
}


ITextureGapi* cGraphicsApiD3D11::CreateTexture(const rTextureGapi& data)
{
	ITexture2D::tDesc desc;
	desc.arraySize = 1;
	desc.bind = (unsigned)eBind::SHADER_RESOURCE;
	desc.format = eFormat::R8G8B8A8_UNORM;
	desc.generateMips = false;
	desc.height = data.height;
	desc.width = data.width;
	desc.mipLevels = data.num_levels;
	desc.usage = eUsage::DYNAMIC;

	ITexture2D* tex;
	CreateTexture( &tex, desc );

	return tex;
}

// Create shader program
HRESULT cGraphicsApiD3D11::CompileShaderFromFile(const zsString& fileName, const zsString& entry, const zsString& profile, zsString* compilerMessage, ID3DBlob** ppBlobOut) {
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3D10_SHADER_OPTIMIZATION_LEVEL3;// | D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;

	ID3DBlob* pErrorBlob;
	char ansiEntry[256];
	char ansiProfile[256];
	wcstombs(ansiEntry, entry.c_str(), 256);
	wcstombs(ansiProfile, profile.c_str(), 256);

	hr = D3DX11CompileFromFileW(fileName.c_str(), nullptr, nullptr, ansiEntry, ansiProfile,
		dwShaderFlags, 0, nullptr, ppBlobOut, &pErrorBlob, nullptr);
	if (FAILED(hr) && pErrorBlob) {
		char* errorStr = (char*)pErrorBlob->GetBufferPointer();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, errorStr, strlen(errorStr), nullptr, 0);
		zsString errorStrW(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, errorStr, strlen(errorStr), &errorStrW[0], size_needed);

		*compilerMessage = errorStrW;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return hr;
}

eGapiResult cGraphicsApiD3D11::CreateShaderProgram(IShaderProgram** resource, const wchar_t* shaderPath_) {

	// Cg file path
	const zsString shaderPath(shaderPath_);

	// path without extension
	const zsString shaderPathNoExt = cStrUtil::CutBack(shaderPath, '.');

	// preferred Binary path
	const zsString binShaderPath = shaderPathNoExt + L".bin";


	// For further reading writing files..
	std::ifstream is;
	std::ofstream os;

	// Cg file exists ?
	is.open(shaderPath);	
		bool cgFileExists  = is.is_open();
	is.close();

	// Binary file exists ?
	is.open(binShaderPath);	
		bool binFileExists = is.is_open();
	is.close();


	bool compileCg	= false;
	bool readBin	= false;

	cCgShaderHelper* cgHelper = nullptr;
	uint64_t sumLastWriteTimes = 0;

	// Cg exist
	if (cgFileExists) {

		// cg helper come and help out us :), you know lot of things about the current CG file
		cgHelper = new cCgShaderHelper(shaderPath);
		if (cgHelper->GetLastErrorMsg() != nullptr)
		{
			lastErrorMsg = cgHelper->GetLastErrorMsg();
			
			OutputDebugStringW(lastErrorMsg.c_str());
			return eGapiResult::ERROR_UNKNOWN;
		}

		// CURRENT sum last write
		sumLastWriteTimes = cFileUtil::GetLastWriteTime(shaderPath);
		for (auto includedFilePath : cgHelper->GetIncludedFilesPaths()) {
			sumLastWriteTimes += cFileUtil::GetLastWriteTime(includedFilePath);
		}

		// Cg and bin exists, we need recompiling?
		if (binFileExists) {

			// Read OLD sum last write from binary
			is.open(binShaderPath, std::ios::binary);
			uint64_t oldSumLastWriteTimes;
			cFileUtil::Read(is, oldSumLastWriteTimes);

			// If they not equal, so sorry, but need recompiling shader !
			if (oldSumLastWriteTimes != sumLastWriteTimes)
				compileCg = true;
			else
				readBin = true;

			is.close();

		} else // Just cg exist need recompile
			compileCg = true;

	// Cg not exist
	} else {

		// Just binary exists, read it
		if (binFileExists)
			readBin = true;

		else { // we have no cg and binary fuck it
			lastErrorMsg = L"Missing .cg file: " + shaderPath;
			return eGapiResult::ERROR_UNKNOWN;
		}
	}

	// Data that can be saved to binary and read up next time
	struct tShaderByteCodeInfo {
		uint32_t byteCodeSize;
		std::shared_ptr<char> byteCode;
		tShaderByteCodeInfo():byteCodeSize(0){}
	};
	tShaderByteCodeInfo byteCodes[cCgShaderHelper::NDOMAINS]; // Shader byte codes

	std::unordered_map<zsString, uint16_t> textureSlots[cCgShaderHelper::NDOMAINS];
	std::unordered_map<zsString, uint16_t> samplerStateSlots[cCgShaderHelper::NDOMAINS];

	std::unordered_map<zsString, tSamplerStateDesc> cgSamplerPairs;	// Cg parsed samplers
	cVertexFormat inputLayoutFormat;	// Vertex shader input layout

	// Need cg recompile
	if (compileCg) {

		auto domainsInfo  = cgHelper->GetDomainInfo();
		cgSamplerPairs	  = cgHelper->GetSamplerStates();
		inputLayoutFormat = cgHelper->GetVSInputFormat();

		// Clear binary file
		os.open(binShaderPath, std::ios::binary, std::ios::trunc);

		// Write cg included summed last write times :)  "last write checksum"
		cFileUtil::Write(os, sumLastWriteTimes);

		// Compile each domain if it exists, write binary...
		for (uint8_t i = 0; i < cCgShaderHelper::NDOMAINS; i++) {

			// Write binary "is that fuckin domain exists?"
			cFileUtil::Write(os, domainsInfo.domainsExist[i]);

			// If not exists leave it
			if (!domainsInfo.domainsExist[i])
				continue;

			zsString dxProfile;
			cCgShaderHelper::eProfileCG cgProfile;
			switch (i) {
			case cCgShaderHelper::VS: cgProfile = cCgShaderHelper::eProfileCG::VS_5_0; dxProfile = "vs_5_0"; break;
			case cCgShaderHelper::HS: cgProfile = cCgShaderHelper::eProfileCG::HS_5_0; dxProfile = "hs_5_0"; break;
			case cCgShaderHelper::DS: cgProfile = cCgShaderHelper::eProfileCG::DS_5_0; dxProfile = "ds_5_0"; break;
			case cCgShaderHelper::GS: cgProfile = cCgShaderHelper::eProfileCG::GS_5_0; dxProfile = "gs_5_0"; break;
			case cCgShaderHelper::PS: cgProfile = cCgShaderHelper::eProfileCG::PS_5_0; dxProfile = "ps_5_0"; break;
			default: assert(0);
			}

			const zsString tmpBinPath = shaderPathNoExt + L"_TMP.bin";

			// Compile CG to HLSL
			cgHelper->CompileCg(shaderPath, tmpBinPath, cgProfile);

			if (cgHelper->GetLastErrorMsg()) {
				lastErrorMsg = L"\n\nshader: " + shaderPath + cgHelper->GetLastErrorMsg();
				os.close();
				cFileUtil::Delete(binShaderPath);
				SAFE_DELETE(cgHelper);
				return eGapiResult::ERROR_UNKNOWN;
			}

			// Compile HLSL to bytecode
			zsString compMessage;
			ID3DBlob* blob;
			HRESULT hr = CompileShaderFromFile(tmpBinPath, L"main", dxProfile, &compMessage, &blob);
			if (FAILED(hr)) {
				lastErrorMsg = L"\n\n HLSL COMPILE FAIL : " + shaderPath + L"\n\n ErrorMsg: " + compMessage;
				os.close();
				cFileUtil::Delete(binShaderPath);
				SAFE_DELETE(cgHelper);
				return eGapiResult::ERROR_UNKNOWN;
			}

			// Save bytecode info
			byteCodes[i].byteCodeSize = blob->GetBufferSize();
			byteCodes[i].byteCode = std::shared_ptr<char>(new char[byteCodes[i].byteCodeSize]);
			memcpy(byteCodes[i].byteCode.get(), blob->GetBufferPointer(), byteCodes[i].byteCodeSize);
			SAFE_RELEASE(blob);

			// Save texture slots
			auto hlslDesc = cgHelper->GetHLSLDesc(tmpBinPath, byteCodes[i].byteCode.get(), byteCodes[i].byteCodeSize);
			for (auto s : hlslDesc.samplerInfo) {
				textureSlots[i][s.first] = s.second.textureSlot;
				samplerStateSlots[i][s.first] = s.second.samplerStateSlot;
			}
			

			// Delete TMP bin
			cFileUtil::Delete(tmpBinPath);

		// Write all the datas need for reconstructing shaders from binary...
			// Byte code size (uint32_t)
 			cFileUtil::Write(os, byteCodes[i].byteCodeSize);

			// Byte code (ptr, uint32_t)
			cFileUtil::Write(os, byteCodes[i].byteCode.get(), byteCodes[i].byteCodeSize);

			// Vertex shader input layout format
			cFileUtil::Write(os, inputLayoutFormat);

		// Texture slots
			// nTexture slots
			uint8_t nTextures = textureSlots[i].size();
			cFileUtil::Write(os, nTextures);

			// Slot name slot index
			for (auto p : (textureSlots[i])) {
				cFileUtil::Write(os, p.first);
				cFileUtil::Write(os, (uint16_t)p.second);
			}

		// Sampler state slots
			// nSamplerStates
			uint8_t nSamplerStates = samplerStateSlots[i].size();
			cFileUtil::Write(os, nSamplerStates);

			for (auto p : (samplerStateSlots[i])) {
				cFileUtil::Write(os, p.first);
				cFileUtil::Write(os, (uint16_t)p.second);
			}
		}

		uint8_t nSamplers = cgSamplerPairs.size();
		cFileUtil::Write(os, nSamplers);
		for (auto p : cgSamplerPairs) {
			// Sampler name
			cFileUtil::Write(os, p.first);

			// Sampler description
			cFileUtil::Write(os, p.second);
		}
		os.close();

	// Read binary
	} else if(readBin) {

		is.open(binShaderPath, std::ios::binary);

		// Read up last write shit for nothing...
		uint64_t sumLastWriteTimesDUMMY;
		cFileUtil::Read(is, sumLastWriteTimesDUMMY);

		// Read up domain datas
		for (uint8_t i = 0; i < cCgShaderHelper::NDOMAINS; i++) {
			
			// Domain exist ?
			bool domainExists;
			cFileUtil::Read(is, domainExists);

			if ( ! domainExists)
				continue;
			
			// Byte code size (uint32_t)
			cFileUtil::Read(is, byteCodes[i].byteCodeSize);

			// Alloc for ByteCode
			byteCodes[i].byteCode = std::shared_ptr<char>(new char[byteCodes[i].byteCodeSize]);

			// Byte code
			cFileUtil::Read(is, byteCodes[i].byteCode.get(), byteCodes[i].byteCodeSize);

			// Vertex shader input layout format (uint64_t)
			cFileUtil::Read(is, inputLayoutFormat);


			zsString textureName;
			zsString samplerName;
			uint16_t slot;


			// Textures
			uint8_t nTextures;
			cFileUtil::Read(is, nTextures);
			for (uint8_t j = 0; j < nTextures; j++) {
				cFileUtil::Read(is, textureName);
				cFileUtil::Read(is, slot);
				textureSlots[i][textureName] = slot;
			}

			// SamplerStates
			uint8_t nSamplerStateSlots;
			cFileUtil::Read(is, nSamplerStateSlots);
			for (uint8_t j = 0; j < nSamplerStateSlots; j++) {
				cFileUtil::Read(is, samplerName);
				cFileUtil::Read(is, slot);
				samplerStateSlots[i][samplerName] = slot;
			}
		}

		uint8_t nSamplers;
		cFileUtil::Read(is, nSamplers);

		zsString samplerName;
		tSamplerStateDesc desc;
		for (int i = 0; i < nSamplers; i++) {
			// Sampler name
			cFileUtil::Read(is, samplerName);

			// Sampler description
			cFileUtil::Read(is, desc);

			cgSamplerPairs[samplerName] = desc;
		}

		is.close();

	}
	SAFE_DELETE(cgHelper);

	ID3D11VertexShader*	  vs = nullptr;
	ID3D11HullShader*	  hs = nullptr;
	ID3D11DomainShader*   ds = nullptr;
	ID3D11GeometryShader* gs = nullptr;
	ID3D11PixelShader*	  ps = nullptr;

	HRESULT hr = S_OK;
	// Create vertex shader
	if (byteCodes[cCgShaderHelper::VS].byteCodeSize != 0) {
		auto byteCodeInfo = byteCodes[cCgShaderHelper::VS];
		hr = d3ddev->CreateVertexShader(byteCodeInfo.byteCode.get(), byteCodeInfo.byteCodeSize, nullptr, &vs);
		if (FAILED(hr)) {
			lastErrorMsg = L"Failed to create vertex shader from bytecode: " + binShaderPath;
			return eGapiResult::ERROR_UNKNOWN;
		}
	}

	// Create hull shader
	if (byteCodes[cCgShaderHelper::HS].byteCodeSize != 0) {
		auto byteCodeInfo = byteCodes[cCgShaderHelper::HS];
		hr = d3ddev->CreateHullShader(byteCodeInfo.byteCode.get(), byteCodeInfo.byteCodeSize, nullptr, &hs);
		if (FAILED(hr)) {
			lastErrorMsg = L"Failed to create hull shader from bytecode: " + binShaderPath;
			return eGapiResult::ERROR_UNKNOWN;
		}
	}

	// Create domain shader
	if (byteCodes[cCgShaderHelper::DS].byteCodeSize != 0) {
		auto byteCodeInfo = byteCodes[cCgShaderHelper::DS];
		hr = d3ddev->CreateDomainShader(byteCodeInfo.byteCode.get(), byteCodeInfo.byteCodeSize, nullptr, &ds);
		if (FAILED(hr)) {
			lastErrorMsg = L"Failed to create domain shader from bytecode: " + binShaderPath;
			return eGapiResult::ERROR_UNKNOWN;
		}
	}

	// Create geometry shader
	if (byteCodes[cCgShaderHelper::GS].byteCodeSize != 0) {
		auto byteCodeInfo = byteCodes[cCgShaderHelper::GS];
		hr = d3ddev->CreateGeometryShader(byteCodeInfo.byteCode.get(), byteCodeInfo.byteCodeSize, nullptr, &gs);
		if (FAILED(hr)) {
			lastErrorMsg = L"Failed to create geometry shader from bytecode: " + binShaderPath;
			return eGapiResult::ERROR_UNKNOWN;
		}
	}

	// Create pixel shader
	if (byteCodes[cCgShaderHelper::PS].byteCodeSize != 0) {
		auto byteCodeInfo = byteCodes[cCgShaderHelper::PS];
		hr = d3ddev->CreatePixelShader(byteCodeInfo.byteCode.get(), byteCodeInfo.byteCodeSize, nullptr, &ps);
		if (FAILED(hr)) {
			lastErrorMsg = L"Failed to create pixel shader from bytecode: " + binShaderPath;
			return eGapiResult::ERROR_UNKNOWN;
		}
	}


	std::vector<cShaderProgramD3D11::tSamplerState> shaderSamplerStates[cCgShaderHelper::NDOMAINS];

	// Create non existing samplers in gApi
	for (auto pair : cgSamplerPairs) {
		// Interpret cg sampler description as DirectX sampler Desc
		D3D11_SAMPLER_DESC sDesc = ConvertToNativeSampler(pair.second);

		// Check if that sampler exists in gapi
		ID3D11SamplerState* state = nullptr;
		for (size_t i = 0; i < samplerStates.size(); i++) {
			if (memcmp(&sDesc, &samplerStates[i].desc, sizeof(D3D11_SAMPLER_DESC)) == 0) {
				state = samplerStates[i].state;
				break;
			}
		}

		// Not found, so create, add
		if (state == nullptr) 
		{
			HRESULT hr = d3ddev->CreateSamplerState(&sDesc, &state);
			if (FAILED(hr)) {
				lastErrorMsg = L"Can't create SamplerState";
				return eGapiResult::ERROR_UNKNOWN;
			}

			tSamplerLink info;
			info.desc = sDesc;
			info.state = state;
			samplerStates.push_back(info);
		}

		// Match cg texture slots with dx domain texture slots
		for (size_t j = 0; j < cCgShaderHelper::NDOMAINS; j++) {
			// sampler found in domain
			int chPos = cStrUtil::Find(pair.first, '['); // TODO Need FindLast :)

			// sampler2D[n] pattern.. TODO NEED DEPRECATION I THINK
			if (chPos >= 0) {
				zsString texName = cStrUtil::SubStrLeft(pair.first, chPos - 1);

				// Found texture name in that domain
				auto texIt = textureSlots[j].find(texName); 
				if (texIt != textureSlots[j].end()) {
					wchar_t chars[3];
					uint16_t nElements;
					cStrUtil::LastNumber(pair.first, nElements, chars, 3);

					for (uint16_t k = 0; k < nElements; k++)
						shaderSamplerStates[j].push_back(cShaderProgramD3D11::tSamplerState(state, texIt->second + k));
				}

			} else {
				// Found texture name in that domain
				auto texIt = samplerStateSlots[j].find(pair.first);
				if (texIt != samplerStateSlots[j].end()) {
					shaderSamplerStates[j].push_back(cShaderProgramD3D11::tSamplerState(state, texIt->second));
				}
			}
		}
	}

	// Create shader program
	cShaderProgramD3D11* shProg = new cShaderProgramD3D11(	byteCodes[cCgShaderHelper::VS].byteCode.get(), 
															byteCodes[cCgShaderHelper::VS].byteCodeSize, 
															inputLayoutFormat,
															vs, hs, ds, gs, ps);

	// Set look up maps
	shProg->SetTextureSlotsVS(textureSlots[cCgShaderHelper::VS]);
	shProg->SetTextureSlotsPS(textureSlots[cCgShaderHelper::PS]);

	shProg->SetSamplerStatesVS(shaderSamplerStates[cCgShaderHelper::VS]);
	shProg->SetSamplerStatesPS(shaderSamplerStates[cCgShaderHelper::PS]);

	// return and tell everyone dat success
	*resource = shProg;
	return eGapiResult::OK;
}


//	Write to various resources
eGapiResult cGraphicsApiD3D11::WriteResource(IIndexBuffer* buffer, void* source, size_t size /*= ZS_NUMLIMITMAX(size_t)*/, size_t offset /*= 0*/) {
	ASSERT(buffer != nullptr);

	if (((cIndexBufferD3D11*)buffer)->GetByteSize() < size + offset)
		return eGapiResult::ERROR_OUT_OF_RANGE;

	HRESULT hr;
	ID3D11Buffer* d3dBuffer = ((cIndexBufferD3D11*)buffer)->buffer;
	D3D11_MAPPED_SUBRESOURCE mappedRes;

	hr = d3dcon->Map(d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
	if (hr != S_OK)
		return eGapiResult::ERROR_INVALID_ARG;

	memcpy((void*)(size_t(mappedRes.pData) + offset), source, size);

	d3dcon->Unmap(d3dBuffer, 0);

	return eGapiResult::OK;
}

eGapiResult cGraphicsApiD3D11::WriteResource(IVertexBuffer* buffer, void* source, size_t size /*= ZS_NUMLIMITMAX(size_t)*/, size_t offset /*= 0*/) {
	ASSERT(buffer != nullptr);

	if (((cVertexBufferD3D11*)buffer)->GetSize() < size + offset)
		return eGapiResult::ERROR_OUT_OF_RANGE;

	HRESULT hr = S_OK;
	ID3D11Buffer* d3dBuffer = ((cVertexBufferD3D11*)buffer)->buffer;
	D3D11_MAPPED_SUBRESOURCE mappedRes;

	hr = d3dcon->Map(d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
	if (hr != S_OK)
		return eGapiResult::ERROR_INVALID_ARG;

	memcpy((void*)(size_t(mappedRes.pData) + offset), source, size);

	d3dcon->Unmap(d3dBuffer, 0);

	return eGapiResult::OK;
}

// Read from various resources
eGapiResult cGraphicsApiD3D11::ReadResource(IIndexBuffer* buffer, void* dest, size_t size, size_t offset /*= 0*/) {
	ASSERT(buffer != nullptr);

	if (((cIndexBufferD3D11*)buffer)->GetByteSize() < size + offset)
		return eGapiResult::ERROR_OUT_OF_RANGE;

	HRESULT hr = S_OK;
	ID3D11Buffer* d3dBuffer = (ID3D11Buffer*)buffer;
	D3D11_MAPPED_SUBRESOURCE mappedRes;

	hr = d3dcon->Map(d3dBuffer, 0, D3D11_MAP_READ, 0, &mappedRes);
	if (hr != S_OK)
		return eGapiResult::ERROR_INVALID_ARG;


	memcpy(dest, (void*)(size_t(mappedRes.pData) + offset), size);

	d3dcon->Unmap(d3dBuffer, 0);

	return eGapiResult::OK;
}

eGapiResult cGraphicsApiD3D11::ReadResource(IVertexBuffer* buffer, void* dest, size_t size, size_t offset /*= 0*/) {
	ASSERT(buffer != nullptr);

	if (((cVertexBufferD3D11*)buffer)->GetSize() < size + offset)
		return eGapiResult::ERROR_OUT_OF_RANGE;

	HRESULT hr = S_OK;
	ID3D11Buffer* d3dBuffer = (ID3D11Buffer*)buffer;
	D3D11_MAPPED_SUBRESOURCE mappedRes;

	hr = d3dcon->Map(d3dBuffer, 0, D3D11_MAP_READ, 0, &mappedRes);
	if (hr != S_OK) {
		return eGapiResult::ERROR_INVALID_ARG;
	}

	memcpy(dest, (void*)(size_t(mappedRes.pData) + offset), size);

	d3dcon->Unmap(d3dBuffer, 0);

	return eGapiResult::OK;
}

eGapiResult cGraphicsApiD3D11::ReadResource(ITexture2D* texture, void* dest, size_t size, size_t offset /*= 0*/) {
	ASSERT(texture != nullptr);

	// TODO OUT OF RANGE CHECK
	//if (buffer->GetSize() < size + offset)
	//return eGapiResult::ERROR_OUT_OF_RANGE;

	HRESULT hr = S_OK;
	ID3D11Texture2D* d3dBuffer = ((cTexture2DD3D11*)texture)->Get();
	D3D11_MAPPED_SUBRESOURCE mappedRes;

	hr = d3dcon->Map(d3dBuffer, 0, D3D11_MAP_READ, 0, &mappedRes);
	if (hr != S_OK) {
		return eGapiResult::ERROR_INVALID_ARG;
	}

	memcpy(dest, (void*)(size_t(mappedRes.pData) + offset), size);

	d3dcon->Unmap(d3dBuffer, 0);

	return eGapiResult::OK;
}

// Copy among resources
eGapiResult cGraphicsApiD3D11::CopyResource(ITexture2D* src, ITexture2D* dst) {
	ASSERT(src != nullptr && dst != nullptr);

	d3dcon->CopyResource((ID3D11Resource*)((cTexture2DD3D11*)dst)->Get(), (ID3D11Resource*)((cTexture2DD3D11*)src)->Get());
	d3dcon->Flush();
	return eGapiResult::OK;
}




////////////////////////////////////////////////////////////////////////////////
//	Draw and rendering

// Clear render-target
void cGraphicsApiD3D11::Clear(bool target /*= true*/, bool depth /*= false*/, bool stencil /*= false*/, const Vec4& clearColor /*= Vec4(0.3f, 0.3f, 0.3f, 1.0f)*/) {

	// Setup clear flags
	UINT clearFlags = 0;
	if (depth)
		clearFlags |= D3D11_CLEAR_DEPTH;
	if (stencil)
		clearFlags |= D3D11_CLEAR_STENCIL;

	// Clear depth, stencil if needed
	if (depth || stencil)
		d3dcon->ClearDepthStencilView((ID3D11DepthStencilView*)defaultRenderTarget->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Clear BackBuffer
	if (target)
		d3dcon->ClearRenderTargetView((ID3D11RenderTargetView*)defaultRenderTarget->GetRTV(), (FLOAT*)&clearColor);
}

// Clear texture
void cGraphicsApiD3D11::ClearTexture(ITexture2D* t, eClearFlag clearFlag /*= eClearFlag::COLOR_DEPTH_STENCIL*/, const Vec4& clearColor /*= Vec4()*/, float depthVal /*= 1.0f*/, size_t stencilVal /*= 0*/) {
	ASSERT(t != nullptr);

	ID3D11DepthStencilView* dsv = ((cTexture2DD3D11*)t)->GetDSV();
	if (dsv)
	{
		bool clearDepth		= ((unsigned)clearFlag & (unsigned)eClearFlag::DEPTH) > 0;
		bool clearStencil	= ((unsigned)clearFlag & (unsigned)eClearFlag::STENCIL) > 0;

		if (clearDepth && clearStencil)
			d3dcon->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depthVal, (UINT8)stencilVal);
		else if (clearDepth)
			d3dcon->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH , depthVal, (UINT8)stencilVal);
		else if (clearStencil)
			d3dcon->ClearDepthStencilView(dsv, D3D11_CLEAR_STENCIL, depthVal, (UINT8)stencilVal);
	}

	ID3D11RenderTargetView* rtv = ((cTexture2DD3D11*)t)->GetRTV();
	if (rtv && ((unsigned)clearFlag & (unsigned)eClearFlag::COLOR) )
		d3dcon->ClearRenderTargetView(rtv, (FLOAT*)&clearColor);
}

// Present
void cGraphicsApiD3D11::Present() {
	d3dsc->Present(1, 0);
}

void cGraphicsApiD3D11::Draw(u32 nVertices, u32 offset /*= 0*/)
{
	ApplySamplerStates();
	ApplyConstantBuffers();
	d3dcon->Draw(nVertices, offset);
}

void cGraphicsApiD3D11::DrawIndexed(u32 nIndices, u32 idxStart)
{
	ApplySamplerStates();
	ApplyConstantBuffers();
	d3dcon->DrawIndexed(nIndices, idxStart, 0);
}

void cGraphicsApiD3D11::DrawInstanced(size_t nVerticesPerInstance, size_t nInstances, size_t idxStartVertex /*= 0*/, size_t idxStartInstance /*= 0*/) {
	ApplySamplerStates();
	ApplyConstantBuffers();
	d3dcon->DrawInstanced(nVerticesPerInstance, nInstances, idxStartVertex, idxStartInstance);
}

void cGraphicsApiD3D11::DrawInstancedIndexed(size_t nIndicesPerInstance, size_t nInstances, size_t idxStartIndex /*= 0*/, size_t idxStartInstance /*= 0*/) {
	ApplySamplerStates();
	ApplyConstantBuffers();
	d3dcon->DrawIndexedInstanced(nIndicesPerInstance, nInstances, idxStartIndex, 0, idxStartInstance);
}


////////////////////////////////////////////////////////////////////////////////
//	Pipeline state

// Set indexed poly-mesh buffers
void cGraphicsApiD3D11::SetVertexBuffer(const IVertexBuffer* vb) {
	ASSERT(vb != nullptr);
	activeVertexBuffer = (cVertexBufferD3D11*)vb;

	const UINT strides = activeVertexBuffer->GetStride();
	const UINT offset = 0;
	ID3D11Buffer* vertices = activeVertexBuffer->GetBufferPointer();
	d3dcon->IASetVertexBuffers(0, 1, &vertices, &strides, &offset);
	AutoSetInputLayout(activeShaderProg, activeVertexBuffer);
}

void cGraphicsApiD3D11::SetVertexBuffers(
	const IVertexBuffer* const * buffers,
	u32* strides,
	u32* offsets,
	u32 start_slot,
	u32 num_buffers)
{
	assert(num_buffers == 1);

	ID3D11Buffer* pBuffers[16];
	for (u32 i = 0; i < num_buffers; ++i)
		pBuffers[i] = ((cVertexBufferD3D11*)buffers[i])->GetBufferPointer();

	d3dcon->IASetVertexBuffers(start_slot, num_buffers, pBuffers, strides, offsets);
	AutoSetInputLayout(activeShaderProg, (cVertexBufferD3D11*)&buffers[0]);
}

void cGraphicsApiD3D11::SetVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetIndexBuffer(IIndexBuffer* ibo)
{
	ASSERT(ibo != nullptr);

	d3dcon->IASetIndexBuffer(((cIndexBufferD3D11*)ibo)->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);
}

// Set instance data for instanced rendering
void cGraphicsApiD3D11::SetInstanceData() {
}

// Set shader texture resource
eGapiResult cGraphicsApiD3D11::SetTexture(int slotIdx, const ITexture2D* t) {
	ASSERT(t != nullptr);

	const ID3D11ShaderResourceView* srv = ((cTexture2DD3D11*)t)->GetSRV();
	if (srv != nullptr) {
		d3dcon->PSSetShaderResources(slotIdx, 1, (ID3D11ShaderResourceView**)&srv);
		return eGapiResult::OK;
	} else {
		return eGapiResult::ERROR_INVALID_ARG;
	}
}

// Set shader texture resource
eGapiResult cGraphicsApiD3D11::SetTexture(const wchar_t* varName, const ITextureGapi* t) {
	return SetTextureArray(varName, &t);
}

void cGraphicsApiD3D11::SetTexture(ITextureGapi* t, u32 idx)
{
	throw std::logic_error("The method or operation is not implemented.");
}

eGapiResult cGraphicsApiD3D11::SetTextureArray(const wchar_t* varName, const ITextureGapi* const * t, uint8_t nTextures/* = 1*/) {
	ASSERT(t != nullptr);

	eGapiResult r;
	for (uint8_t i = 0; i < nTextures; i++) {
		const ID3D11ShaderResourceView* srv = ((cTexture2DD3D11*)t[i])->GetSRV();
		assert(srv);
		if (srv != nullptr) {
			int startSlot = ((cShaderProgramD3D11*)activeShaderProg)->GetTextureSlotVS(varName);
			if (startSlot < 0) {
				startSlot = ((cShaderProgramD3D11*)activeShaderProg)->GetTextureSlotPS(varName);
				if (startSlot < 0) {
					return eGapiResult::ERROR_INVALID_ARG;
					assert(0);
				}

				// PS texture found
				d3dcon->PSSetShaderResources(startSlot + i, 1, (ID3D11ShaderResourceView**)&srv);
				r = eGapiResult::OK;
			} else {
				// VS texture found
				d3dcon->VSSetShaderResources(startSlot + i, 1, (ID3D11ShaderResourceView**)&srv);
				r = eGapiResult::OK;
			}
		}
		else {
			r = eGapiResult::ERROR_INVALID_ARG;
			break;
		}
	}
	return r;
}

// Set compiled-linked shader program
void cGraphicsApiD3D11::SetShaderProgram(IShaderProgram* shProg) {
	ASSERT(shProg != nullptr);

	activeShaderProg = (cShaderProgramD3D11*)shProg;

	AutoSetInputLayout(activeShaderProg, activeVertexBuffer);

	d3dcon->VSSetShader(const_cast<ID3D11VertexShader*>(activeShaderProg->GetVS()), 0, 0);
	d3dcon->PSSetShader(const_cast<ID3D11PixelShader*>(activeShaderProg->GetPS()), 0, 0);
}

// Set primitive topology
void cGraphicsApiD3D11::SetPrimitiveTopology(ePrimitiveTopology t) {
	switch (t) {
	case ePrimitiveTopology::LINE_LIST:
		d3dcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case ePrimitiveTopology::TRIANGLE_LIST:
		d3dcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	}
}

eGapiResult cGraphicsApiD3D11::SetVSConstantBuffer(const void* data, size_t size, size_t slotIdx) {
	vsConstBufferStateChanged = true;

	// 16 means one register byte size... (vec4)
	size_t dstByteOffset = slotIdx * 16;  // slotIdx * sizeof(float) * 4
	// Need resize for constant buffer
	if (vsConstBufferSize < dstByteOffset + size)
	{
		// Save old size
		size_t oldSize = vsConstBufferSize;

		// Determine new size
		// Not multiple of 16, for ex. 24, then size + 16 - (size % 16) = 32
		vsConstBufferSize = dstByteOffset + size;
		if (((dstByteOffset + size) % 16) != 0)
			vsConstBufferSize = vsConstBufferSize + 16 - (vsConstBufferSize % 16);

		// Resize our void*
		Realloc(vsConstBufferData, oldSize, vsConstBufferSize);

		// Recreate constant buffer
		SAFE_RELEASE(vsConstBuffer);
		D3D11_BUFFER_DESC desc;
			desc.ByteWidth = vsConstBufferSize;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HRESULT hr = d3ddev->CreateBuffer(&desc, nullptr, &vsConstBuffer);
		if (FAILED(hr)) {
			return eGapiResult::ERROR_OUT_OF_MEMORY;
		}
		d3dcon->VSSetConstantBuffers(0, 1, &vsConstBuffer); // Set Buffer
	}
	memcpy((unsigned char*)vsConstBufferData + dstByteOffset, data, size);
	return eGapiResult::OK;
}

eGapiResult cGraphicsApiD3D11::SetPSConstantBuffer(const void* data, size_t size, size_t slotIdx) {
	psConstBufferStateChanged = true;

	// 16 means one register byte size... (vec4)
	size_t dstByteOffset = slotIdx * 16;  // slotIdx * sizeof(float) * 4
	// Need resize for constant buffer
	if (psConstBufferSize < dstByteOffset + size)
	{
		// Save old size
		size_t oldSize = psConstBufferSize;

		// Determine new size
		psConstBufferSize = dstByteOffset + size;
		if (((dstByteOffset + size) % 16) != 0)
			psConstBufferSize = psConstBufferSize + 16 - (psConstBufferSize % 16);
			
		// Resize our void*
		Realloc(psConstBufferData, oldSize, psConstBufferSize);

		// Recreate constant buffer
		SAFE_RELEASE( psConstBuffer );
		D3D11_BUFFER_DESC desc;
			desc.ByteWidth = psConstBufferSize;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HRESULT hr = d3ddev->CreateBuffer(&desc, nullptr, &psConstBuffer);
		if (FAILED(hr)) {
			return eGapiResult::ERROR_OUT_OF_MEMORY;
		}
		d3dcon->PSSetConstantBuffers(0, 1, &psConstBuffer); // Set Buffer
	}
	memcpy((unsigned char*)psConstBufferData + dstByteOffset, data, size);
	return eGapiResult::OK;
}

// Set (multiple) render targets
eGapiResult cGraphicsApiD3D11::SetRenderTargets(unsigned nTargets, const ITexture2D* const* renderTargets, ITexture2D* depthStencilTarget /* = nullptr */) {

	ID3D11DepthStencilView* dsv = (depthStencilTarget) ? ((cTexture2DD3D11*)depthStencilTarget)->GetDSV() : nullptr;

	// If there are rtv's create viewports based on it
	memset(activeRTVs, 0, sizeof(ID3D11RenderTargetView*) * 16);
	unsigned i = 0;
	for (; i < nTargets; i++) {
		activeRTVs[i] = ((const cTexture2DD3D11*)(renderTargets[i]))->GetRTV();
		if (activeRTVs[i]) {
			activeViewports[i].Width = ((const cTexture2DD3D11*)(renderTargets[i]))->GetWidth();
			activeViewports[i].Height = ((const cTexture2DD3D11*)(renderTargets[i]))->GetHeight();
			activeViewports[i].TopLeftX = 0;
			activeViewports[i].TopLeftY = 0;
			activeViewports[i].MinDepth = 0.0f;
			activeViewports[i].MaxDepth = 1.0f;
		}
	}

	// There is no rtv, but dsv, create viewports based on dsv
	if (!renderTargets && depthStencilTarget) {
		activeViewports[0].Width = ((const cTexture2DD3D11*)depthStencilTarget)->GetWidth();
		activeViewports[0].Height = ((const cTexture2DD3D11*)depthStencilTarget)->GetHeight();
		activeViewports[0].TopLeftX = 0;
		activeViewports[0].TopLeftY = 0;
		activeViewports[0].MinDepth = 0.0f;
		activeViewports[0].MaxDepth = 1.0f;
		nTargets = 1; // use that shit
	}

	// Set Viewports
	d3dcon->RSSetViewports(nTargets, activeViewports);

	// Set RTVS, dsv
	d3dcon->OMSetRenderTargets(nTargets, activeRTVs, dsv);

	return eGapiResult::OK;
}

void cGraphicsApiD3D11::SetRenderTargets(const rRenderTargetInfo* render_targets, u32 size)
{
	throw std::logic_error("The method or operation is not implemented.");
}

// Resize swap chain
eGapiResult cGraphicsApiD3D11::SetBackBufferSize(unsigned width, unsigned height) {

	if (defaultRenderTarget->GetWidth() == width && defaultRenderTarget->GetHeight() == height)
		return eGapiResult::OK;

	// Release default render target (BackBuffer)
	SAFE_DELETE(defaultRenderTarget);

	HRESULT hr = d3dsc->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_EFFECT_DISCARD | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	// Set full screen state according to width height
	IDXGIOutput* op; 
	hr = d3dsc->GetContainingOutput(&op);

	bool isFullScreen = (GetSystemMetrics(SM_CXSCREEN) == width) && (GetSystemMetrics(SM_CYSCREEN) == height);

	hr = d3dsc->SetFullscreenState(isFullScreen, isFullScreen ? op : nullptr);

	if (!FAILED(hr)) {
		defaultVP.Width = width;
		defaultVP.Height = height;
		return CreateViewsForBB();
	}
	switch (hr) {
	case S_OK: return eGapiResult::OK;   break;
	case E_OUTOFMEMORY: return eGapiResult::ERROR_OUT_OF_MEMORY; break;
	case E_INVALIDARG:	return eGapiResult::ERROR_INVALID_ARG; break;
	default:
		return eGapiResult::ERROR_UNKNOWN;
	}
}

// Get default render target == backbuffer
ITexture2D* cGraphicsApiD3D11::GetDefaultRenderTarget() const {
	return defaultRenderTarget;
}

// Set backBuffer as render-target
eGapiResult cGraphicsApiD3D11::SetRenderTargetDefault() {
	d3dcon->RSSetViewports(1, &defaultVP);
	ID3D11RenderTargetView* rtv = defaultRenderTarget->GetRTV();
	d3dcon->OMSetRenderTargets(1, &rtv, (ID3D11DepthStencilView*)defaultRenderTarget->GetDSV());
	return eGapiResult::OK;
}

// Set blend state
eGapiResult cGraphicsApiD3D11::SetBlendState(const tBlendDesc& desc) {
	D3D11_BLEND_DESC bsDesc = ConvertToNativeBlend(desc);

	ID3D11BlendState* state = nullptr;

	size_t i = 0;
	for (; i < blendStates.size(); i++)
		if (memcmp(&bsDesc, &blendStates[i].desc, sizeof(D3D11_BLEND_DESC)) == 0)
			break;

	if (i == blendStates.size()) {
		HRESULT hr = d3ddev->CreateBlendState(&bsDesc, &state);
		if (FAILED(hr)) {
			switch (hr) {
			case E_OUTOFMEMORY: return eGapiResult::ERROR_OUT_OF_MEMORY;
			default: return eGapiResult::ERROR_UNKNOWN;
			}
		}

		tBlendLink info;
			info.desc = bsDesc;
			info.state = state;
		blendStates.push_back(info);

		//SAFE_RELEASE(state);

	} else {
		state = blendStates[i].state;
	}

	d3dcon->OMSetBlendState(state, nullptr, 0xFFFFFFFF);
	return eGapiResult::OK;
}

void cGraphicsApiD3D11::SetBlendState(const rBlendState& state)
{
	throw std::logic_error("The method or operation is not implemented.");
}

// Set depth-stencil state
eGapiResult cGraphicsApiD3D11::SetDepthStencilState(const tDepthStencilDesc& desc, uint8_t stencilRef) {
	D3D11_DEPTH_STENCIL_DESC dsDesc = ConvertToNativeDepthStencil(desc);

	size_t i = 0;
	for (; i < depthStencilStates.size(); i++)
		if (memcmp(&dsDesc, &depthStencilStates[i].desc, sizeof(D3D11_DEPTH_STENCIL_DESC)) == 0)
			break;

	ID3D11DepthStencilState* state = nullptr;

	// Not existing
	if ( i == depthStencilStates.size()) {
		HRESULT hr = d3ddev->CreateDepthStencilState(&dsDesc, &state);
		if (FAILED(hr)) {
			switch (hr) {
			case E_OUTOFMEMORY: return eGapiResult::ERROR_OUT_OF_MEMORY;
			default: return eGapiResult::ERROR_UNKNOWN;
			}
		}
		tDepthStencilLink info;
			info.desc = dsDesc;
			info.state = state;
		depthStencilStates.push_back(info);
	} else {
		state = depthStencilStates[i].state;
	}
	
	d3dcon->OMSetDepthStencilState(state, stencilRef);
	return eGapiResult::OK;
}

// Set Target Window
eGapiResult cGraphicsApiD3D11::SetWindow(Window *renderWindow) {
	ASSERT(renderWindow != nullptr);

	size_t clientWidth = renderWindow->GetClientWidth();
	size_t clientHeight = renderWindow->GetClientHeight();
	// Same window size : don't need new swap chain
	if (defaultRenderTarget != nullptr)
	if (clientWidth == defaultRenderTarget->GetWidth() && clientHeight == defaultRenderTarget->GetHeight())
		return eGapiResult::OK;

	// Create swap chain for device
	eGapiResult r = CreateMostAcceptableSwapChain(clientWidth, clientHeight, (HWND)(renderWindow->GetHandle()));
	if (r != eGapiResult::OK) return r;

	// Create main render target (BackBuffer)
	r = CreateViewsForBB();
	if (r != eGapiResult::OK) return r;

	// Create default viewport for swapChain rendering
	defaultVP.TopLeftX = 0,
	defaultVP.TopLeftY = 0;
	defaultVP.Width = clientWidth;
	defaultVP.Height = clientHeight;
	defaultVP.MaxDepth = 1.0f;
	defaultVP.MinDepth = 0.0f;

	// BackBuffer will be the render target in default
	r = SetRenderTargetDefault();
	return r;
}


////////////////////////////////////////////////////////////////////////////////
// Misc
const wchar_t* cGraphicsApiD3D11::GetLastErrorMsg() const {
	return lastErrorMsg.c_str();
}

///////////////////////////////////////////////////////////////////////////////
// Utility
eGapiResult cGraphicsApiD3D11::SaveTextureToFile(ITexture2D* t, ITexture2D::eImageFormat f, const char* filePath)  {
	assert(t);

	// Extract resource from texture, that we want to save
	ID3D11Resource* res;
	ID3D11ShaderResourceView* srv = ((cTexture2DD3D11*)t)->GetSRV();
	assert(srv);
	if (!srv)
		return eGapiResult::ERROR_INVALID_ARG;

	srv->GetResource(&res);

	// Convert format
	D3DX11_IMAGE_FILE_FORMAT format;
	switch (f) {
	case ITexture2D::eImageFormat::BMP:  format = D3DX11_IFF_BMP;  break;
	case ITexture2D::eImageFormat::JPG:  format = D3DX11_IFF_JPG;  break;
	case ITexture2D::eImageFormat::PNG:  format = D3DX11_IFF_PNG;  break;
	case ITexture2D::eImageFormat::DDS:  format = D3DX11_IFF_DDS;  break;
	case ITexture2D::eImageFormat::TIFF: format = D3DX11_IFF_TIFF; break;
	case ITexture2D::eImageFormat::GIF:  format = D3DX11_IFF_GIF;  break;
	case ITexture2D::eImageFormat::WMP:  format = D3DX11_IFF_WMP;  break;
	}
	HRESULT hr = D3DX11SaveTextureToFileA(d3dcon, res, format, filePath);

	// OMG MOTHERFUCKER NAB FUNCTION "D3DX11SaveTextureToFileA" solve that...
	if (FAILED(hr))
	{
		// New texture, (we write our resource into that) with hardcoded shader
		ITexture2D* tmpTex = NULL;
		ITexture2D::tDesc d;
			d.arraySize = 1;
			d.bind = (int)eBind::RENDER_TARGET | (int)eBind::SHADER_RESOURCE;
			d.format = eFormat::R8G8B8A8_UNORM;
			d.width = t->GetWidth();
			d.height = t->GetHeight();
			d.mipLevels = 1;
			d.usage = eUsage::DEFAULT;
		CreateTexture(&tmpTex, d);

		// We use screen copy cg shader for the task
		const char* tmpShaderFilePath = "zsiroskenyergraphicsapid3d11_tmpscreencopyshader.cg";
		std::ofstream os(tmpShaderFilePath, std::ios::trunc);

		os << "sampler2D inputTexture = {\n";
		os << "	MipFilter = POINT,\n";
		os << "	MinFilter = POINT,\n";
		os << "	MagFilter = POINT,\n";
		os << "	AddressU = CLAMP,\n";
		os << "	AddressV = CLAMP,\n";
		os << "};\n";
		os << "\n";
		os << "struct VS_IN {\n";
		os << "	float3 posL : POSITION;\n";
		os << "	float2 tex0 : TEXCOORD0;\n";
		os << "};\n";
		os << "\n";
		os << "struct VS_OUT {\n";
		os << "	float4 posH : SV_POSITION;\n";
		os << "	float2 tex0 : TEXCOORD2;\n";
		os << "};\n";
		os << "\n";
		os << "\n";
		os << "VS_OUT VS_MAIN(VS_IN In, uint VertexID : VERTEXID) {\n";
		os << "	VS_OUT Out;\n";
		os << "	Out.tex0 = float2((VertexID << 1) & 2, VertexID & 2);\n";
		os << "	Out.posH = float4(Out.tex0 * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);\n";
		os << "	return Out;\n";
		os << "}\n";
		os << "\n";
		os << "float4 PS_MAIN(VS_OUT In) : COLOR0{\n";
		os << "	float4 color = tex2D(inputTexture, In.tex0);\n";
		os << "	return color;\n";
		os << "};\n";
		os << "\n";
		os << "technique t\n";
		os << "{\n";
		os << "	pass p\n";
		os << "	{\n";
		os << "	VertexProgram = compile vs_4_0 VS_MAIN();\n";
		os << "	FragmentProgram = compile ps_4_0 PS_MAIN();\n";
		os << "}\n";
		os << "}";
		os.close();



		// Create the shader
		// TODO HLSL SHADERREL KELL MEGOLDANI, ami helyben van kifejtve ( de most lusta vagyok ) !!
		IShaderProgram* shaderScreenCopy;
		zsString shaderFilePath = zsString(tmpShaderFilePath).c_str();
		eGapiResult errCode = CreateShaderProgram(&shaderScreenCopy, shaderFilePath.c_str());
		assert(errCode == eGapiResult::OK);
		cFileUtil::Delete(shaderFilePath);
		cFileUtil::Delete("zsiroskenyergraphicsapid3d11_tmpscreencopyshader.bin");

		// Clear the texture
		ClearTexture(tmpTex);

		// " Copy " resource into texture
		SetRenderTargets(1, &tmpTex, nullptr);
		SetShaderProgram(shaderScreenCopy);
		SetTexture(L"inputTexture", t);
		Draw(3);

		// Now DirectX function can handle that texture format
		errCode = SaveTextureToFile(tmpTex, f, filePath);
		if (errCode != eGapiResult::OK)
		{
			assert(0);
			return eGapiResult::ERROR_FILE_NOT_FOUND;
		}

		// Release temporary resources
		shaderScreenCopy->Release();
		tmpTex->Release();

	}
	return eGapiResult::OK;
}

eGapiResult cGraphicsApiD3D11::GenerateMips(ITexture2D* t)
{
	cTexture2DD3D11* tex = (cTexture2DD3D11*)t;

	ASSERT(tex->GetSRV());

	// We can only generate mips, for texture that have srv, and D3D11_RESOURCE_MISC_GENERATE_MIPS misc flag defined
	d3dcon->GenerateMips(tex->GetSRV());
	return eGapiResult::OK;
}
////////////////////////////////////////////////////////////////////////////////
// Internal helper functions


////////////////////////////////////////
// Create input layouts
ID3D11InputLayout* cGraphicsApiD3D11::GetInputLayout(cShaderProgramD3D11* shader, cVertexFormat bufferFormat) {
	cVertexFormat shaderFormat = shader->GetVSInputFormat();

	//if (!shaderFormat.IsSubsetOf(bufferFormat)) {
	//	return nullptr;
	//}

	std::pair<cVertexFormat, cVertexFormat> key(shaderFormat, bufferFormat);

	auto it = inputLayoutStore.find(key);
	if (it == inputLayoutStore.end()) {
		// create new input layout
		ID3D11InputLayout* layout = nullptr;

		auto vertexDesc = ConvertToNativeVertexFormat(shaderFormat);

		d3ddev->CreateInputLayout(vertexDesc.data(), vertexDesc.size(), shader->GetVSByteCode(), shader->GetVSByteCodeSize(), &layout);

		// add input layout to stuff
		if (!layout) {
			return nullptr;
		}
		else {
			inputLayoutStore.insert(InputLayoutMapT::value_type(key, layout));
			return layout;
		}
	}
	else {
		return it->second;
	}
}

// Set said input layout
void cGraphicsApiD3D11::AutoSetInputLayout(cShaderProgramD3D11* shader, cVertexBufferD3D11* buffer) {
	if (buffer == nullptr || shader == nullptr) {
		return;
	}

	auto bufferFormat = buffer->GetFormat();

	auto inputLayout = GetInputLayout(shader, bufferFormat);
	if (inputLayout) {
		d3dcon->IASetInputLayout(inputLayout);
	}
	else {
		d3dcon->IASetInputLayout(nullptr);
	}
}

////////////////////////////////////////
// Convert stuff to native format

// vertex decl
std::vector<D3D11_INPUT_ELEMENT_DESC> cGraphicsApiD3D11::ConvertToNativeVertexFormat(cVertexFormat format) {
	auto elems = format.Decode();
	std::vector<D3D11_INPUT_ELEMENT_DESC> ret;

	unsigned pos = 0, color = 0, normal = 0, tex = 0;

	unsigned alignedByteoffset = 0;
	for (auto& v : elems) {
		D3D11_INPUT_ELEMENT_DESC desc;

		// semantic and semantic index
		switch (v.semantic) {
			case cVertexFormat::POSITION:
				desc.SemanticName =  "POSITION";
				desc.SemanticIndex = pos;
				pos++;
				break;
			case cVertexFormat::NORMAL:
				desc.SemanticName = "NORMAL";
				desc.SemanticIndex = normal;
				normal++;
				break;
			case cVertexFormat::TEXCOORD:
				desc.SemanticName = "TEXCOORD";
				desc.SemanticIndex = tex;
				tex++;
				break;
			// default is color
			default:
				desc.SemanticName = "COLOR";
				desc.SemanticIndex = color;
				color++;
				break;
		}

		desc.InputSlot = 0;
		desc.AlignedByteOffset = alignedByteoffset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		alignedByteoffset += v.GetByteSize();

		// format
		desc.Format = [&]() ->DXGI_FORMAT {
			if (v.nComponents == 1) {
				if (v.bitsPerComponent == cVertexFormat::_8_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R8_SINT;
					else if (v.type == cVertexFormat::NORM)
						return DXGI_FORMAT_R8_SNORM;
				}
				if (v.bitsPerComponent == cVertexFormat::_16_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R16_SINT;
					else if (v.type == cVertexFormat::NORM)
						return DXGI_FORMAT_R16_SNORM;
					else if (v.type == cVertexFormat::FLOAT)
						return DXGI_FORMAT_R16_FLOAT;
				}
				if (v.bitsPerComponent == cVertexFormat::_32_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R32_SINT;
					else if (v.type == cVertexFormat::FLOAT)
						return DXGI_FORMAT_R32_FLOAT;
				}
			}
			else if (v.nComponents == 2) {
				if (v.bitsPerComponent == cVertexFormat::_8_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R8G8_SINT;
					else if (v.type == cVertexFormat::NORM)
						return DXGI_FORMAT_R8G8_SNORM;
				}
				if (v.bitsPerComponent == cVertexFormat::_16_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R16G16_SINT;
					else if (v.type == cVertexFormat::NORM)
						return DXGI_FORMAT_R16G16_SNORM;
					else if (v.type == cVertexFormat::FLOAT)
						return DXGI_FORMAT_R16G16_FLOAT;
				}
				if (v.bitsPerComponent == cVertexFormat::_32_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R32G32_SINT;
					else if (v.type == cVertexFormat::FLOAT)
						return DXGI_FORMAT_R32G32_FLOAT;
				}
			}
			else if (v.nComponents == 3) {
				if (v.bitsPerComponent == cVertexFormat::_32_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R32G32B32_SINT;
					else if (v.type == cVertexFormat::FLOAT)
						return DXGI_FORMAT_R32G32B32_FLOAT;
				}
			}
			else if (v.nComponents == 4) {
				if (v.bitsPerComponent == cVertexFormat::_8_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R8G8B8A8_SINT;
					else if (v.type == cVertexFormat::NORM)
						return DXGI_FORMAT_R8G8B8A8_SNORM;
				}
				if (v.bitsPerComponent == cVertexFormat::_16_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R16G16B16A16_SINT;
					else if (v.type == cVertexFormat::NORM)
						return DXGI_FORMAT_R16G16B16A16_SNORM;
					else if (v.type == cVertexFormat::FLOAT)
						return DXGI_FORMAT_R16G16B16A16_FLOAT;
				}
				if (v.bitsPerComponent == cVertexFormat::_32_BIT) {
					if (v.type == cVertexFormat::INT)
						return DXGI_FORMAT_R32G32B32A32_SINT;
					else if (v.type == cVertexFormat::FLOAT)
						return DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
			}
			return DXGI_FORMAT_UNKNOWN;
		}();
		
		ret.push_back(desc);
	}
	return ret;
}

IShaderProgram* cGraphicsApiD3D11::CreateShaderSource(const char* vertex_shader_source, const char* pixel_shader_source, const char* geometry_shader_source /*= nullptr*/, const char* tess_control_shader_source /*= nullptr*/, const char* tess_eval_shader_source /*= nullptr*/)
{
	throw std::logic_error("The method or operation is not implemented.");
}

IShaderProgram* cGraphicsApiD3D11::CreateShaderBinary(void* data, size_t size)
{
	throw std::logic_error("The method or operation is not implemented.");
}

IUniformBuffer* cGraphicsApiD3D11::CreateUniformBuffer(const rBuffer& data)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::WriteBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset)
{
	WriteResource(buffer, data, size, offset);
}

void cGraphicsApiD3D11::WriteBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset)
{
	WriteResource(buffer, data, size, offset);
}

void cGraphicsApiD3D11::WriteBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::ReadBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::ReadBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::ReadBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::ResetStatesToDefault()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetDepthState(const rDepthState& state)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetStencilState(const rStencilState& state)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetRasterizationState(const rRasterizerState& state)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetSamplerState(const char* slotName, const rSamplerState& smpdata, ITextureGapi* t)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetSRGBWrites(bool val)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetSeamlessCubeMaps(bool val)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetViewport(int x, int y, unsigned w, unsigned h)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool cGraphicsApiD3D11::GetError()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetDebugOutput(bool val)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetSyncDebugOutput(bool val)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetUniformBuffer(IUniformBuffer* buf, u32 idx)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::ClearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth /*= 0*/, i32 stencil /*= 0*/)
{
	throw std::logic_error("The method or operation is not implemented.");
}

u32 cGraphicsApiD3D11::GetNumVertexBufferSlots() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

IInputLayout* cGraphicsApiD3D11::CreateInputLayout(rInputElement* elements, size_t num_elements)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::SetInputLayout(IInputLayout* layout)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void cGraphicsApiD3D11::WriteTexture(ITextureGapi* t, const rTextureUpdate& d)
{
	cTexture2DD3D11* pTex = (cTexture2DD3D11*)t;

	ID3D11Texture2D* pNativeTex = pTex->Get();

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = d3dcon->Map( pNativeTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData );
	memcpy(mappedData.pData, d.data, d.height * d.width * sizeof(BYTE) * 4);
	d3dcon->Unmap( pNativeTex, 0 );
}

// bind
unsigned cGraphicsApiD3D11::ConvertToNativeBind(unsigned flags) {
	unsigned ret = 0;
	ret |= ((flags&(unsigned)eBind::CONSTANT_BUFFER) != 0 ? D3D11_BIND_CONSTANT_BUFFER : 0);
	ret |= ((flags&(unsigned)eBind::INDEX_BUFFER) != 0 ? D3D11_BIND_INDEX_BUFFER : 0);
	ret |= ((flags&(unsigned)eBind::DEPTH_STENCIL) != 0 ? D3D11_BIND_DEPTH_STENCIL : 0);
	ret |= ((flags&(unsigned)eBind::RENDER_TARGET) != 0 ? D3D11_BIND_RENDER_TARGET : 0);
	ret |= ((flags&(unsigned)eBind::SHADER_RESOURCE) != 0 ? D3D11_BIND_SHADER_RESOURCE : 0);
	ret |= ((flags&(unsigned)eBind::VERTEX_BUFFER) != 0 ? D3D11_BIND_VERTEX_BUFFER : 0);

	return ret;
}

// usage
D3D11_USAGE cGraphicsApiD3D11::ConvertToNativeUsage(eUsage usage) {
	static const std::unordered_map<eUsage, D3D11_USAGE> lookupTable = {
		{ eUsage::DEFAULT, D3D11_USAGE_DEFAULT },
		{ eUsage::DYNAMIC, D3D11_USAGE_DYNAMIC },
		{ eUsage::IMMUTABLE, D3D11_USAGE_IMMUTABLE },
		{ eUsage::STAGING, D3D11_USAGE_STAGING },
	};
	auto it = lookupTable.find(usage);
	assert(it != lookupTable.end());
	return it->second;
}

// format
DXGI_FORMAT cGraphicsApiD3D11::ConvertToNativeFormat(eFormat fmt) {
	static const std::unordered_map<eFormat, DXGI_FORMAT> lookupTable = {
		{ eFormat::UNKNOWN, DXGI_FORMAT_UNKNOWN },
		{ eFormat::R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_TYPELESS },
		{ eFormat::R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT },
		{ eFormat::R32G32B32A32_UINT, DXGI_FORMAT_R32G32B32A32_UINT },
		{ eFormat::R32G32B32A32_SINT, DXGI_FORMAT_R32G32B32A32_SINT },
		{ eFormat::R32G32B32_TYPELESS, DXGI_FORMAT_R32G32B32_TYPELESS },
		{ eFormat::R32G32B32_FLOAT, DXGI_FORMAT_R32G32B32_FLOAT },
		{ eFormat::R32G32B32_UINT, DXGI_FORMAT_R32G32B32_UINT },
		{ eFormat::R32G32B32_SINT, DXGI_FORMAT_R32G32B32_SINT },
		{ eFormat::R16G16B16A16_TYPELESS, DXGI_FORMAT_R16G16B16A16_TYPELESS },
		{ eFormat::R16G16B16A16_FLOAT, DXGI_FORMAT_R16G16B16A16_FLOAT },
		{ eFormat::R16G16B16A16_UNORM, DXGI_FORMAT_R16G16B16A16_UNORM },
		{ eFormat::R16G16B16A16_UINT, DXGI_FORMAT_R16G16B16A16_UINT },
		{ eFormat::R16G16B16A16_SNORM, DXGI_FORMAT_R16G16B16A16_SNORM },
		{ eFormat::R16G16B16A16_SINT, DXGI_FORMAT_R16G16B16A16_SINT },
		{ eFormat::R32G32_TYPELESS, DXGI_FORMAT_R32G32_TYPELESS },
		{ eFormat::R32G32_FLOAT, DXGI_FORMAT_R32G32_FLOAT },
		{ eFormat::R32G32_UINT, DXGI_FORMAT_R32G32_UINT },
		{ eFormat::R32G32_SINT, DXGI_FORMAT_R32G32_SINT },
		{ eFormat::R32G8X24_TYPELESS, DXGI_FORMAT_R32G8X24_TYPELESS },
		{ eFormat::D32_FLOAT_S8X24_UINT, DXGI_FORMAT_D32_FLOAT_S8X24_UINT },
		{ eFormat::R32_FLOAT_X8X24_TYPELESS, DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS },
		{ eFormat::X32_TYPELESS_G8X24_UINT, DXGI_FORMAT_X32_TYPELESS_G8X24_UINT },
		{ eFormat::R10G10B10A2_TYPELESS, DXGI_FORMAT_R10G10B10A2_TYPELESS },
		{ eFormat::R10G10B10A2_UNORM, DXGI_FORMAT_R10G10B10A2_UNORM },
		{ eFormat::R10G10B10A2_UINT, DXGI_FORMAT_R10G10B10A2_UINT },
		{ eFormat::R11G11B10_FLOAT, DXGI_FORMAT_R11G11B10_FLOAT },
		{ eFormat::R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_TYPELESS },
		{ eFormat::R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM },
		{ eFormat::R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB },
		{ eFormat::R8G8B8A8_UINT, DXGI_FORMAT_R8G8B8A8_UINT },
		{ eFormat::R8G8B8A8_SNORM, DXGI_FORMAT_R8G8B8A8_SNORM },
		{ eFormat::R8G8B8A8_SINT, DXGI_FORMAT_R8G8B8A8_SINT },
		{ eFormat::R16G16_TYPELESS, DXGI_FORMAT_R16G16_TYPELESS },
		{ eFormat::R16G16_FLOAT, DXGI_FORMAT_R16G16_FLOAT },
		{ eFormat::R16G16_UNORM, DXGI_FORMAT_R16G16_UNORM },
		{ eFormat::R16G16_UINT, DXGI_FORMAT_R16G16_UINT },
		{ eFormat::R16G16_SNORM, DXGI_FORMAT_R16G16_SNORM },
		{ eFormat::R16G16_SINT, DXGI_FORMAT_R16G16_SINT },
		{ eFormat::R32_TYPELESS, DXGI_FORMAT_R32_TYPELESS },
		{ eFormat::D32_FLOAT, DXGI_FORMAT_D32_FLOAT },
		{ eFormat::R32_FLOAT, DXGI_FORMAT_R32_FLOAT },
		{ eFormat::R32_UINT, DXGI_FORMAT_R32_UINT },
		{ eFormat::R32_SINT, DXGI_FORMAT_R32_SINT },
		{ eFormat::R24G8_TYPELESS, DXGI_FORMAT_R24G8_TYPELESS },
		{ eFormat::D24_UNORM_S8_UINT, DXGI_FORMAT_D24_UNORM_S8_UINT },
		{ eFormat::R24_UNORM_X8_TYPELESS, DXGI_FORMAT_R24_UNORM_X8_TYPELESS },
		{ eFormat::X24_TYPELESS_G8_UINT, DXGI_FORMAT_X24_TYPELESS_G8_UINT },
		{ eFormat::R8G8_TYPELESS, DXGI_FORMAT_R8G8_TYPELESS },
		{ eFormat::R8G8_UNORM, DXGI_FORMAT_R8G8_UNORM },
		{ eFormat::R8G8_UINT, DXGI_FORMAT_R8G8_UINT },
		{ eFormat::R8G8_SNORM, DXGI_FORMAT_R8G8_SNORM },
		{ eFormat::R8G8_SINT, DXGI_FORMAT_R8G8_SINT },
		{ eFormat::R16_TYPELESS, DXGI_FORMAT_R16_TYPELESS },
		{ eFormat::R16_FLOAT, DXGI_FORMAT_R16_FLOAT },
		{ eFormat::D16_UNORM, DXGI_FORMAT_D16_UNORM },
		{ eFormat::R16_UNORM, DXGI_FORMAT_R16_UNORM },
		{ eFormat::R16_UINT, DXGI_FORMAT_R16_UINT },
		{ eFormat::R16_SNORM, DXGI_FORMAT_R16_SNORM },
		{ eFormat::R16_SINT, DXGI_FORMAT_R16_SINT },
		{ eFormat::R8_TYPELESS, DXGI_FORMAT_R8_TYPELESS },
		{ eFormat::R8_UNORM, DXGI_FORMAT_R8_UNORM },
		{ eFormat::R8_UINT, DXGI_FORMAT_R8_UINT },
		{ eFormat::R8_SNORM, DXGI_FORMAT_R8_SNORM },
		{ eFormat::R8_SINT, DXGI_FORMAT_R8_SINT },
		{ eFormat::A8_UNORM, DXGI_FORMAT_A8_UNORM },
		{ eFormat::R1_UNORM, DXGI_FORMAT_R1_UNORM },
		{ eFormat::R9G9B9E5_SHAREDEXP, DXGI_FORMAT_R9G9B9E5_SHAREDEXP },
		{ eFormat::R8G8_B8G8_UNORM, DXGI_FORMAT_R8G8_B8G8_UNORM },
		{ eFormat::G8R8_G8B8_UNORM, DXGI_FORMAT_G8R8_G8B8_UNORM },
		{ eFormat::BC1_TYPELESS, DXGI_FORMAT_BC1_TYPELESS },
		{ eFormat::BC1_UNORM, DXGI_FORMAT_BC1_UNORM },
		{ eFormat::BC1_UNORM_SRGB, DXGI_FORMAT_BC1_UNORM_SRGB },
		{ eFormat::BC2_TYPELESS, DXGI_FORMAT_BC2_TYPELESS },
		{ eFormat::BC2_UNORM, DXGI_FORMAT_BC2_UNORM },
		{ eFormat::BC2_UNORM_SRGB, DXGI_FORMAT_BC2_UNORM_SRGB },
		{ eFormat::BC3_TYPELESS, DXGI_FORMAT_BC3_TYPELESS },
		{ eFormat::BC3_UNORM, DXGI_FORMAT_BC3_UNORM },
		{ eFormat::BC3_UNORM_SRGB, DXGI_FORMAT_BC3_UNORM_SRGB },
		{ eFormat::BC4_TYPELESS, DXGI_FORMAT_BC4_TYPELESS },
		{ eFormat::BC4_UNORM, DXGI_FORMAT_BC4_UNORM },
		{ eFormat::BC4_SNORM, DXGI_FORMAT_BC4_SNORM },
		{ eFormat::BC5_TYPELESS, DXGI_FORMAT_BC5_TYPELESS },
		{ eFormat::BC5_UNORM, DXGI_FORMAT_BC5_UNORM },
		{ eFormat::BC5_SNORM, DXGI_FORMAT_BC5_SNORM },
		{ eFormat::B5G6R5_UNORM, DXGI_FORMAT_B5G6R5_UNORM },
		{ eFormat::B5G5R5A1_UNORM, DXGI_FORMAT_B5G5R5A1_UNORM },
		{ eFormat::B8G8R8A8_UNORM, DXGI_FORMAT_B8G8R8A8_UNORM },
		{ eFormat::B8G8R8X8_UNORM, DXGI_FORMAT_B8G8R8X8_UNORM },
		{ eFormat::R10G10B10_XR_BIAS_A2_UNORM, DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM },
		{ eFormat::B8G8R8A8_TYPELESS, DXGI_FORMAT_B8G8R8A8_TYPELESS },
		{ eFormat::B8G8R8A8_UNORM_SRGB, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB },
		{ eFormat::B8G8R8X8_TYPELESS, DXGI_FORMAT_B8G8R8X8_TYPELESS },
		{ eFormat::B8G8R8X8_UNORM_SRGB, DXGI_FORMAT_B8G8R8X8_UNORM_SRGB },
		{ eFormat::BC6H_TYPELESS, DXGI_FORMAT_BC6H_TYPELESS },
		{ eFormat::BC6H_UF16, DXGI_FORMAT_BC6H_UF16 },
		{ eFormat::BC6H_SF16, DXGI_FORMAT_BC6H_SF16 },
		{ eFormat::BC7_TYPELESS, DXGI_FORMAT_BC7_TYPELESS },
		{ eFormat::BC7_UNORM, DXGI_FORMAT_BC7_UNORM },
		{ eFormat::BC7_UNORM_SRGB, DXGI_FORMAT_BC7_UNORM_SRGB },
		{ eFormat::AYUV, DXGI_FORMAT_AYUV },
		{ eFormat::Y410, DXGI_FORMAT_Y410 },
		{ eFormat::Y416, DXGI_FORMAT_Y416 },
		{ eFormat::NV12, DXGI_FORMAT_NV12 },
		{ eFormat::P010, DXGI_FORMAT_P010 },
		{ eFormat::P016, DXGI_FORMAT_P016 },
		{ eFormat::_420_OPAQUE, DXGI_FORMAT_420_OPAQUE },
		{ eFormat::YUY2, DXGI_FORMAT_YUY2 },
		{ eFormat::Y210, DXGI_FORMAT_Y210 },
		{ eFormat::Y216, DXGI_FORMAT_Y216 },
		{ eFormat::NV11, DXGI_FORMAT_NV11 },
		{ eFormat::AI44, DXGI_FORMAT_AI44 },
		{ eFormat::IA44, DXGI_FORMAT_IA44 },
		{ eFormat::P8, DXGI_FORMAT_P8 },
		{ eFormat::A8P8, DXGI_FORMAT_A8P8 },
		{ eFormat::B4G4R4A4_UNORM, DXGI_FORMAT_B4G4R4A4_UNORM },
	};
	auto it = lookupTable.find(fmt);
	assert(it != lookupTable.end());
	return it->second;
}


// blend to native
D3D11_BLEND_OP cGraphicsApiD3D11::ConvertToNativeBlendOp(eBlendOp blendOp) {
	switch (blendOp) {
	case eBlendOp::ADD: return D3D11_BLEND_OP_ADD;
	case eBlendOp::SUBTRACT: return D3D11_BLEND_OP_SUBTRACT;
	case eBlendOp::REV_SUBTRACT: return D3D11_BLEND_OP_REV_SUBTRACT;
	case eBlendOp::MAX: return D3D11_BLEND_OP_MAX;
	case eBlendOp::MIN: return D3D11_BLEND_OP_MIN;
	default: return D3D11_BLEND_OP_ADD;
	}
}

D3D11_BLEND cGraphicsApiD3D11::ConvertToNativeBlendFactor(eBlendFactor blendFactor) {
	switch (blendFactor) {
	case eBlendFactor::ZERO:				return D3D11_BLEND_ZERO;
	case eBlendFactor::ONE:					return D3D11_BLEND_ONE;
	case eBlendFactor::SRC_COLOR:			return D3D11_BLEND_SRC_COLOR;
	case eBlendFactor::INV_SRC_COLOR:		return D3D11_BLEND_INV_SRC_COLOR;
	case eBlendFactor::SRC_ALPHA:			return D3D11_BLEND_SRC_ALPHA;
	case eBlendFactor::INV_SRC_ALPHA:		return D3D11_BLEND_INV_SRC_ALPHA;
	case eBlendFactor::DEST_ALPHA:			return D3D11_BLEND_DEST_ALPHA;
	case eBlendFactor::INV_DEST_ALPHA:		return D3D11_BLEND_INV_DEST_ALPHA;
	case eBlendFactor::DEST_COLOR:			return D3D11_BLEND_DEST_COLOR;
	case eBlendFactor::INV_DEST_COLOR:		return D3D11_BLEND_INV_DEST_COLOR;
	case eBlendFactor::SRC_ALPHA_SAT:		return D3D11_BLEND_SRC_ALPHA_SAT;
	case eBlendFactor::BLEND_FACTOR:		return D3D11_BLEND_BLEND_FACTOR;
	case eBlendFactor::INV_BLEND_FACTOR:	return D3D11_BLEND_INV_BLEND_FACTOR;
	case eBlendFactor::SRC1_COLOR:			return D3D11_BLEND_SRC1_COLOR;
	case eBlendFactor::INV_SRC1_COLOR:		return D3D11_BLEND_INV_SRC1_COLOR;
	case eBlendFactor::SRC1_ALPHA:			return D3D11_BLEND_SRC1_ALPHA;
	case eBlendFactor::INV_SRC1_ALPHA:		return D3D11_BLEND_INV_SRC1_ALPHA;
	default:								return D3D11_BLEND_ONE;
	}
}

uint8_t cGraphicsApiD3D11::ConvertToNativeBlendMask(eBlendWriteMask blendMask) {
	uint8_t ret = 0u;
	if ((uint8_t)blendMask & (uint8_t)eBlendWriteMask::ALPHA)
		ret |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
	if ((uint8_t)blendMask & (uint8_t)eBlendWriteMask::RED)
		ret |= D3D11_COLOR_WRITE_ENABLE_RED;
	if ((uint8_t)blendMask & (uint8_t)eBlendWriteMask::GREEN)
		ret |= D3D11_COLOR_WRITE_ENABLE_GREEN;
	if ((uint8_t)blendMask & (uint8_t)eBlendWriteMask::BLUE)
		ret |= D3D11_COLOR_WRITE_ENABLE_BLUE;
	if ((uint8_t)blendMask & (uint8_t)eBlendWriteMask::ALL)
		ret |= D3D11_COLOR_WRITE_ENABLE_ALL;

	return ret;
}

D3D11_BLEND_DESC cGraphicsApiD3D11::ConvertToNativeBlend(tBlendDesc blend) {
	D3D11_BLEND_DESC ret;
	ret.AlphaToCoverageEnable = (blend.alphaToCoverageEnable ? TRUE : FALSE);
	ret.IndependentBlendEnable = (blend.independentBlendEnable ? TRUE : FALSE);
	for (int i = 0; i < 8; i++) {
		ret.RenderTarget[i].BlendEnable = blend[i].enable;
		ret.RenderTarget[i].BlendOp = ConvertToNativeBlendOp(blend[i].blendOp);
		ret.RenderTarget[i].BlendOpAlpha = ConvertToNativeBlendOp(blend[i].blendOpAlpha);
		ret.RenderTarget[i].DestBlend = ConvertToNativeBlendFactor(blend[i].destBlend);
		ret.RenderTarget[i].DestBlendAlpha = ConvertToNativeBlendFactor(blend[i].destBlendAlpha);
		ret.RenderTarget[i].RenderTargetWriteMask = ConvertToNativeBlendMask((eBlendWriteMask)blend[i].writeMask);
		ret.RenderTarget[i].SrcBlend = ConvertToNativeBlendFactor(blend[i].srcBlend);
		ret.RenderTarget[i].SrcBlendAlpha = ConvertToNativeBlendFactor(blend[i].srcBlendAlpha);
	}

	return ret;
}

// depth-stencil to native
D3D11_COMPARISON_FUNC cGraphicsApiD3D11::ConvertToNativeCompFunc(eComparisonFunc compFunc) {
	switch (compFunc) {
	case eComparisonFunc::ALWAYS:		return D3D11_COMPARISON_ALWAYS;
	case eComparisonFunc::EQUAL:		return D3D11_COMPARISON_EQUAL;
	case eComparisonFunc::GREATER:		return D3D11_COMPARISON_GREATER;
	case eComparisonFunc::GREATER_EQUAL:return D3D11_COMPARISON_GREATER_EQUAL;
	case eComparisonFunc::LESS:			return D3D11_COMPARISON_LESS;
	case eComparisonFunc::LESS_EQUAL:	return D3D11_COMPARISON_LESS_EQUAL;
	case eComparisonFunc::NEVER:		return D3D11_COMPARISON_NEVER;
	case eComparisonFunc::NOT_EQUAL:	return D3D11_COMPARISON_NOT_EQUAL;
	default: return D3D11_COMPARISON_LESS_EQUAL;
	}
}

D3D11_STENCIL_OP cGraphicsApiD3D11::ConvertToNativeStencilOp(eStencilOp stencilOp) {
	switch (stencilOp) {
	case eStencilOp::DECR:		return D3D11_STENCIL_OP_DECR;
	case eStencilOp::DECR_SAT:	return D3D11_STENCIL_OP_DECR_SAT;
	case eStencilOp::INCR:		return D3D11_STENCIL_OP_INCR;
	case eStencilOp::INCR_SAT:	return D3D11_STENCIL_OP_INCR_SAT;
	case eStencilOp::INVERT:	return D3D11_STENCIL_OP_INVERT;
	case eStencilOp::KEEP:		return D3D11_STENCIL_OP_KEEP;
	case eStencilOp::REPLACE:	return D3D11_STENCIL_OP_REPLACE;
	case eStencilOp::ZERO:		return D3D11_STENCIL_OP_ZERO;
	default: return D3D11_STENCIL_OP_REPLACE;
	}
}

D3D11_DEPTH_STENCIL_DESC cGraphicsApiD3D11::ConvertToNativeDepthStencil(const tDepthStencilDesc& depthStencil) {
	D3D11_DEPTH_STENCIL_DESC ret;
	ret.DepthEnable = (depthStencil.depthEnable ? TRUE : FALSE);
	ret.DepthFunc = ConvertToNativeCompFunc(depthStencil.depthCompare);
	ret.DepthWriteMask = (depthStencil.depthWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO);
	ret.StencilEnable = (depthStencil.stencilEnable ? TRUE : FALSE);
	ret.StencilReadMask = depthStencil.stencilReadMask;
	ret.StencilWriteMask = depthStencil.stencilWriteMask;

	ret.BackFace.StencilDepthFailOp = ConvertToNativeStencilOp(depthStencil.stencilOpBackFace.stencilPassDepthFail);
	ret.BackFace.StencilFailOp = ConvertToNativeStencilOp(depthStencil.stencilOpBackFace.stencilFail);
	ret.BackFace.StencilPassOp = ConvertToNativeStencilOp(depthStencil.stencilOpBackFace.stencilPass);
	ret.BackFace.StencilFunc = ConvertToNativeCompFunc(depthStencil.stencilOpBackFace.stencilCompare);

	ret.FrontFace.StencilDepthFailOp = ConvertToNativeStencilOp(depthStencil.stencilOpFrontFace.stencilPassDepthFail);
	ret.FrontFace.StencilFailOp = ConvertToNativeStencilOp(depthStencil.stencilOpFrontFace.stencilFail);
	ret.FrontFace.StencilPassOp = ConvertToNativeStencilOp(depthStencil.stencilOpFrontFace.stencilPass);
	ret.FrontFace.StencilFunc = ConvertToNativeCompFunc(depthStencil.stencilOpFrontFace.stencilCompare);

	return ret;
}

D3D11_SAMPLER_DESC cGraphicsApiD3D11::ConvertToNativeSampler(const tSamplerStateDesc& sDesc) {
	D3D11_SAMPLER_DESC r; memset(&r, 0, sizeof(r));

	switch (sDesc.addressU) {
		case eAddress::CLAMP:	r.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;	break;
		case eAddress::WRAP:	r.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	break;
		case eAddress::MIRROR:	r.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;	break;
	}

	switch (sDesc.addressV) {
		case eAddress::CLAMP:	r.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;	break;
		case eAddress::WRAP:	r.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;	break;
		case eAddress::MIRROR:	r.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;	break;
	}

	// TODO these 5 shits
	r.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	r.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	r.MinLOD = 0.0f;
	r.MaxLOD = 1.0f;
	r.MipLODBias = 1;

	// Simplest case, MIN MAG MIP type EQUAL
	if (sDesc.filterMag == sDesc.filterMin && sDesc.filterMag == sDesc.filterMip) {
		switch (sDesc.filterMag) {
			case eFilter::POINT:		r.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	break;
			case eFilter::LINEAR:		r.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; break;
			case eFilter::ANISOTROPIC:
			{
				r.Filter = D3D11_FILTER_ANISOTROPIC;
				r.MaxAnisotropy = 16; // TODO
				break;
			}
				
		}
	} else {
		switch (sDesc.filterMag) {
		case eFilter::POINT:
			switch (sDesc.filterMin) {
			case eFilter::POINT:
				switch (sDesc.filterMip) { // MIN POINT | MAG POINT
				case eFilter::POINT:	r.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;			break;
				case eFilter::LINEAR:	r.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;	break;
				}
				break;
			case eFilter::LINEAR:
				switch (sDesc.filterMip) { // MIN LINEAR | MAG POINT
				case eFilter::POINT:	r.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;			break;
				case eFilter::LINEAR:	r.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;	break;
				}
				break;
			}
			break;
		case eFilter::LINEAR: // Filter MAG
			switch (sDesc.filterMin) {
			case eFilter::POINT:
				switch (sDesc.filterMip) {// MAG LINAER | MIN POINT
				case eFilter::POINT:	r.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;	break;
				case eFilter::LINEAR:	r.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;		break;
				}
				break;
			case eFilter::LINEAR:
				switch (sDesc.filterMip) { // MAG LINAER | MIN LINEAR
				case eFilter::POINT:	r.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;	break;
				case eFilter::LINEAR:	r.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;			break;
				}
				break;
			}
			break;
		}
	}
	
	return r;
}