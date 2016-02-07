////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	This class processes a complete scene, and deals with shadow map creation
//	as a whole. 
////////////////////////////////////////////////////////////////////////////////


#include "GraphicsEngine.h"
#include "GraphicsEntity.h"
#include "../../Core/src/GAPI.h"
#include <stdexcept>
#include "GraphicsEngine_Raster/Scene.h"

#undef near
#undef far
#undef min
#undef max

////////////////////////////////////////////////////////////////////////////////
// Constructor
cGraphicsEngine::cShadowRenderer::cShadowRenderer(cGraphicsEngine& parent)
:
parent(parent),
gApi(parent.gApi),
shaderDirectional(nullptr),
shaderPoint(nullptr),
shaderSpot(nullptr)
{
	try {
		LoadShaders();
	}
	catch (std::exception& e) {
		throw std::runtime_error(std::string("failed to create shaders: ") + e.what());
	}
}

cGraphicsEngine::cShadowRenderer::~cShadowRenderer() {
	Cleanup();
}

void cGraphicsEngine::cShadowRenderer::Cleanup() {
	SAFE_RELEASE(shaderDirectional);
	SAFE_RELEASE(shaderPoint);
	SAFE_RELEASE(shaderSpot);
}


////////////////////////////////////////////////////////////////////////////////
// Render them shadowmapz
void cGraphicsEngine::cShadowRenderer::RenderShadowMaps(Scene& sceneManager) {
	//// set states
	//tBlendDesc blend = blendDefault;
	//blend[0].writeMask = 0;
	//gApi->SetBlendState(blend);

	//tDepthStencilDesc dsState = depthStencilDefault;
	//dsState.depthCompare = eComparisonFunc::LESS;
	//gApi->SetDepthStencilState(dsState, 0x00);

	//// render lights
	//auto& lights = sceneManager.GetLights();
	//for (auto& v : lights) {
	//	// get light components
	//	auto& light = v->first;
	//	auto& shadowMap = v->second;
	//	// check shadow map parameters
	//	if (!light.enabled && light.shadowQuality!=cGraphicsLight::eShadowQuality::DISABLED) {
	//		continue;
	//	}
	//	//continue;
	//	if (light.shadowResolution < 256) {
	//		light.shadowResolution = 256;
	//	}
	//	
	//	switch (light.type) {
	//		case cGraphicsLight::DIRECTIONAL : {
	//			// set sh map type
	//			shadowMap.SetType(light.type);
	//			shadowMap.ClearUnneeded();
	//			// get map for type
	//			auto& shadowMapDir = *(cShadowMapDir*)&shadowMap;
	//			// init map if not compatible with currently inited
	//			auto resolution = light.shadowResolution;
	//			auto nCascadeQuality = light.shadowQuality;
	//			try {
	//				if (!shadowMapDir.IsValid(gApi, resolution, eFormat::R24_UNORM_X8_TYPELESS, eFormat::D24_UNORM_S8_UINT, nCascadeQuality)) {
	//					shadowMapDir.Init(gApi, resolution, eFormat::R24_UNORM_X8_TYPELESS, eFormat::D24_UNORM_S8_UINT, nCascadeQuality);
	//				}
	//			}
	//			catch (std::exception& e) {
	//				std::cerr << e.what() << std::endl;
	//				break; // breaks switch case label
	//			}

	//			// generate cascade splits
	//			size_t nCascades = shadowMapDir.GetNumCascades();
	//			std::vector<float> cascadeSplits(nCascades + 1, 0.0f);

	//			float near = parent.camera->GetNearPlane();
	//			float far = parent.camera->GetFarPlane();

	//			for (size_t i = 0; i <= nCascades; i++) {
	//				cascadeSplits[i] = near*pow((far / near), float(i) / float(nCascades));
	//			}
	//			for (float& v : cascadeSplits) {
	//				v -= near;
	//				v /= (far - near);
	//			}

	//			// foreach cascade
	//			for (size_t i = 0; i < nCascades; i++) {
	//				// compute transforms
	//				auto& transform = shadowMapDir.GetTransforms()[i];
	//				bool isGoodTransform = shadowMapDir.Transform(
	//						transform.projMat,
	//						transform.viewMat,
	//						light.direction, 
	//						parent.camera->GetViewMatrix(), parent.camera->GetProjMatrix(), 
	//						cascadeSplits[i], cascadeSplits[i+1]);
	//				if (!isGoodTransform)
	//					continue;

	//				// setup render
	//				gApi->SetShaderProgram(shaderDirectional);
	//				ITexture2D* textureSlice = shadowMapDir.GetTexture()->GetArraySlice(i);
	//				gApi->SetRenderTargets(0, nullptr, textureSlice);
	//				gApi->ClearTexture(textureSlice);

	//				// foreach inst grp
	//				for (auto& instgrp : sceneManager.GetInstanceGroups()) {
	//					// set geom params
	//					gApi->SetVertexBuffer(instgrp->geom->GetVertexBuffer());
	//					gApi->SetIndexBuffer(instgrp->geom->GetIndexBuffer());

	//					// render objects
	//					for (auto& entity : instgrp->entities) {
	//						Matrix44 matWorld = entity->GetWorldMatrix();
	//						Matrix44 worldViewProj = matWorld * transform.viewMat * transform.projMat;
	//						gApi->SetVSConstantBuffer(&worldViewProj, sizeof(worldViewProj), 0);
	//						gApi->DrawIndexed(instgrp->geom->GetIndexBuffer()->GetByteSize() / 4);
	//					}
	//				}

	//				textureSlice->Release();
	//			}
	//			break;
	//		}
	//		case cGraphicsLight::SPOT:{

	//			break;
	//		}
	//		case cGraphicsLight::POINT:{

	//			break;
	//		}
	//	}
	//}
}


////////////////////////////////////////////////////////////////////////////////
// Deal with the shaders
void cGraphicsEngine::cShadowRenderer::ReloadShaders() {
	auto Reload = [this](IShaderProgram** prog, const wchar_t* name)->void {
		IShaderProgram* tmp = SafeLoadShader(gApi, name); // it throws on error!
		(*prog)->Release();
		*prog = tmp;
	};
	Reload(&shaderDirectional, L"shaders/shadowmap_render_dir.cg");
}

void cGraphicsEngine::cShadowRenderer::LoadShaders() {
	auto Create = [this](const wchar_t* shader)->IShaderProgram* {
		return SafeLoadShader(gApi, shader);
	};
	try {
		shaderDirectional = Create(L"shaders/shadowmap_render_dir.cg");
	}
	catch (...) {
		UnloadShaders();
		throw;
	}
}

void cGraphicsEngine::cShadowRenderer::UnloadShaders() {
	SAFE_RELEASE(shaderDirectional);
}