////////////////////////////////////////////////////////////////////////////////
// Helper class for processing HDR


#include "GraphicsEngine.h"
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <algorithm>


////////////////////////////////////////////////////////////////////////////////
//	Constructor & destructor
cGraphicsEngine::cHDRProcessor::cHDRProcessor(cGraphicsEngine& parent) : parent(parent), gApi(parent.gApi) {
	// set vars
	avgLuminance = 0;
	adaptedLuminance = 0;
	sourceWidth = sourceHeight = 0;
	blurBuffer = nullptr;
	downSampled = nullptr;
	luminanceStaging = nullptr;
	shaderBlurHoriz = shaderBlurVert = shaderCompose = shaderLumAvg = shaderLumSample = shaderOverbright = nullptr;
	source = nullptr;
	dest = nullptr;
	for (auto& t : luminanceBuffer)
		t = nullptr;

	// create shaders
	try {
		LoadShaders();
	}
	catch (std::exception& e) {
		Cleanup();
		throw std::runtime_error(std::string("failed to create shaders:\n") + e.what());
	}

	// create luminance textures
	unsigned resolution=1;
	ITexture2D::tDesc desc;
	
	desc.format = eFormat::R32_FLOAT;
	desc.bind = (int)eBind::RENDER_TARGET | (int)eBind::SHADER_RESOURCE;
	desc.usage = eUsage::DEFAULT;
	for (int i = 0; i < 10; i++) {
		desc.width = desc.height = resolution;
		eGapiResult r = parent.gApi->CreateTexture(&luminanceBuffer[9-i], desc);
		if (r != eGapiResult::OK) {
			Cleanup();
			throw std::runtime_error("failed to create luminance buffers");
		}
		resolution *= 2;
	}

	desc.bind = 0;
	desc.usage = eUsage::STAGING;
	desc.width = desc.height = 1;
	eGapiResult r = parent.gApi->CreateTexture(&luminanceStaging, desc);
	if (r != eGapiResult::OK) {
		Cleanup();
		throw std::runtime_error("failed to create luminance cpu copy buffer");
	}
}

cGraphicsEngine::cHDRProcessor::~cHDRProcessor() {
	// release all textures
	Cleanup();
}

void cGraphicsEngine::cHDRProcessor::Cleanup() {
	// release shaders
	UnloadShaders();
	// release all textures
	for (auto& t : luminanceBuffer)
		SAFE_RELEASE(t);
	SAFE_RELEASE(blurBuffer);
	SAFE_RELEASE(downSampled);
	SAFE_RELEASE(luminanceStaging);
}

// load shaders
void cGraphicsEngine::cHDRProcessor::LoadShaders() {
	auto Create = [this](const wchar_t* shader)->IShaderProgram* {
		return SafeLoadShader(gApi, shader);
	};
	try {
		shaderLumSample = Create(L"shaders/hdr_luminance_sample.cg");
		shaderLumAvg = Create(L"shaders/hdr_luminance_avg.cg");
		shaderCompose = Create(L"shaders/hdr_compose.cg");
		shaderBlurHoriz = Create(L"shaders/hdr_blur_horiz.cg");
		shaderBlurVert = Create(L"shaders/hdr_blur_vert.cg");
		shaderOverbright = Create(L"shaders/hdr_overbright_downsample.cg");
	}
	catch (...) {
		UnloadShaders();
		throw;
	}
}

// unload shaders
void cGraphicsEngine::cHDRProcessor::UnloadShaders() {
	SAFE_RELEASE(shaderLumSample);
	SAFE_RELEASE(shaderLumAvg);
	SAFE_RELEASE(shaderCompose);
	SAFE_RELEASE(shaderBlurHoriz);
	SAFE_RELEASE(shaderBlurVert);
	SAFE_RELEASE(shaderOverbright);
}

// reload shaders
void cGraphicsEngine::cHDRProcessor::ReloadShaders() {
	auto Reload = [this](IShaderProgram** prog, const wchar_t* name)->void {
		IShaderProgram* tmp = SafeLoadShader(gApi, name); // it throws on error!
		(*prog)->Release();
		*prog = tmp;
	};
	Reload(&shaderLumSample, L"shaders/hdr_luminance_sample.cg");
	Reload(&shaderLumAvg, L"shaders/hdr_luminance_avg.cg");
	Reload(&shaderCompose, L"shaders/hdr_compose.cg");
	Reload(&shaderBlurHoriz, L"shaders/hdr_blur_horiz.cg");
	Reload(&shaderBlurVert, L"shaders/hdr_blur_vert.cg");
	Reload(&shaderOverbright, L"shaders/hdr_overbright_downsample.cg");
}


////////////////////////////////////////////////////////////////////////////////
//	Methods

//	Set HDR range float buffer
eGraphicsResult cGraphicsEngine::cHDRProcessor::SetSource(ITexture2D* t) {
	assert(t != nullptr);

	unsigned sourceWidth = t->GetWidth();
	unsigned sourceHeight = t->GetHeight();

	assert( sourceWidth  != 0 );
	assert( sourceHeight != 0 );

	// Good job no need of generating downsampled buffers and etc.
	if (t == source && this->sourceWidth == sourceWidth && this->sourceHeight == sourceHeight)
		return eGraphicsResult::OK;

	// backup old buffers
	auto blurBuffer_ = blurBuffer;
	auto downSampled_ = downSampled;
	// backup size
	auto sourceWidth_ = this->sourceWidth;
	auto sourceHeight_ = this->sourceHeight;

	// set internal vars
	this->sourceWidth = sourceWidth;
	this->sourceHeight = sourceHeight;
	this->source = t;

	// create downsampled buffers
	ITexture2D::tDesc desc;
	desc.bind = (int)eBind::RENDER_TARGET | (int)eBind::SHADER_RESOURCE;
	desc.format = eFormat::R16G16B16A16_FLOAT;
	desc.width = std::max(1u, sourceWidth / 2);
	desc.height = std::max(1u, sourceHeight / 2);
	auto r1 = parent.gApi->CreateTexture(&downSampled, desc);
	auto r2 = parent.gApi->CreateTexture(&blurBuffer, desc);
	// check results
	if (r1 != eGapiResult::OK || r2 != eGapiResult::OK) {
		// failure!
		// cleanup
		SAFE_RELEASE(downSampled);
		SAFE_RELEASE(blurBuffer);
		// rollback
		this->sourceWidth = sourceWidth_;
		this->sourceHeight = sourceHeight_;
		blurBuffer = blurBuffer_;
		downSampled = downSampled_;
		// return
		return eGraphicsResult::ERROR_UNKNOWN;
	}

	// release old buffers
	SAFE_RELEASE(blurBuffer_);
	SAFE_RELEASE(downSampled_);

	return eGraphicsResult::OK;
}

// Set LDR output buffer
void cGraphicsEngine::cHDRProcessor::SetDestination(ITexture2D* dest) {
	this->dest = dest;
}

////////////////////////////////////////////////////////////////////////////////
//	Update
#include <iostream>
float elapsedTotal = 0.0f; // debug print state changes

void cGraphicsEngine::cHDRProcessor::Update(float elapsedSec) {
	float elapsed = 1e-8f;
	if (elapsedSec >= 1e-8f) {
		elapsed = elapsedSec;
	}
	elapsedTotal += elapsed;

	// calculate average luminance
	gApi->SetShaderProgram(shaderLumSample);
	gApi->SetRenderTargets(1, &luminanceBuffer[0], nullptr);
	gApi->SetTexture(L"textureInput", source);
	gApi->Draw(3);

	parent.gApi->SetShaderProgram(shaderLumAvg);
	for (int i = 1; i < 10; i++) {
		gApi->SetRenderTargets(1, &luminanceBuffer[i], nullptr);
		gApi->SetTexture(L"textureInput", luminanceBuffer[i - 1]);
		gApi->Draw(3);
	}

	// extract average luminance from buffer
	auto r = gApi->CopyResource(luminanceBuffer[9], luminanceStaging);
	gApi->ReadResource(luminanceStaging, &avgLuminance, sizeof(float));

	// calculate adaptation
	avgLuminance = std::max(1e-15f, avgLuminance);
	float logAvgLum = log10(avgLuminance);
	float rodSensitivity = 0.04f / (0.04f + logAvgLum);
	float speed = rodSensitivity * 0.4f + (1.0f - rodSensitivity)*0.4f;
	float a = adaptedLuminance;
	adaptedLuminance = adaptedLuminance + (logAvgLum - adaptedLuminance)*(1 - exp(-elapsed / speed));

	if (isnan(adaptedLuminance)) {
		adaptedLuminance = 1.0;
		//__debugbreak();
	}

	// shader constants
	struct {
		float avgLum;
		float pad4, pad5, pad6;
		float blueShift;
		float pad1;
		float pad2;
		float pad3;
	} shaderConstants;

	
	// downsample that bullshit for blurring
	shaderConstants.avgLum = pow(10.0f, adaptedLuminance);

	gApi->SetRenderTargets(1, &downSampled);
	gApi->SetShaderProgram(shaderOverbright);
	gApi->SetPSConstantBuffer(&shaderConstants, sizeof(shaderConstants), 0);
	gApi->SetTexture(L"textureInput", source);
	gApi->Draw(3);
	
	// now blur that bullshit
	gApi->SetRenderTargets(1, &blurBuffer);
	gApi->SetShaderProgram(shaderBlurHoriz);
	gApi->SetTexture(L"textureInput", downSampled);
	gApi->Draw(3);
	gApi->SetRenderTargets(1, &downSampled);
	gApi->SetShaderProgram(shaderBlurVert);
	gApi->SetTexture(L"textureInput", blurBuffer);
	gApi->Draw(3);
	
	// compose to destination buffer
	shaderConstants.avgLum = pow(10.0f, adaptedLuminance);
	// blueshift: mesopic range from -2.3 to 0.7 mcd/m2
	shaderConstants.blueShift = 1.0f - min(mm::max((adaptedLuminance + 2.3f) / (0.7f + 2.3f), 0.0f), 1.0f);

	gApi->SetRenderTargets(1, &dest);
	gApi->SetShaderProgram(shaderCompose);
	gApi->SetPSConstantBuffer(&shaderConstants, sizeof(shaderConstants), 0);
	gApi->SetTexture(L"textureInput", source);
	gApi->SetTexture(L"blurTexture", downSampled);
	gApi->Draw(3);


/*	// display HDR information
	if (elapsedTotal >= 1.0f) {
		std::cout << "Avg. luminance = " << avgLuminance << ", log10(lum) =  " << log10(avgLuminance) << std::endl;
		std::cout << "   Adaptation = " << adaptedLuminance << std::endl;
		std::cout << "   [" << avgLuminance*1.998e-3 << ", " << avgLuminance*1.998 << "]\n";
		std::cout << "   Blueshift = " << shaderConstants.blueShift << std::endl;
		elapsedTotal = 0.0f;
	}*/
}