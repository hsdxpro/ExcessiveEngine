////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	This class processes a complete scene, and deals with shadow map creation
//	as a whole. 
////////////////////////////////////////////////////////////////////////////////


#include "GraphicsEngine.h"
#include "GraphicsEntity.h"
#include "../../Core/src/GAPI.h"
#include "GraphicsEngine/Raster/Scene.h"
#include "GraphicsEngine/ILight.h"
#include <stdexcept>

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
	tBlendDesc blend = blendDefault;
	blend[0].writeMask = 0;
	gApi->SetBlendState(blend);

	tDepthStencilDesc dsState = depthStencilDefault;
	dsState.depthCompare = eComparisonFunc::LESS;
	gApi->SetDepthStencilState(dsState, 0x00);

	// render lights
	auto& lights = sceneManager.GetLights();
	for (auto& v : lights) {
		// get light components
		auto& light = *v;
		
		auto* shadowMap = light.GetShadowMap();

		// check shadow map parameters
		//if (!light.enabled && light.shadowQuality != cGraphicsLight::eShadowQuality::DISABLED) {
		//	continue;
		//}
		//continue;
		//if (light.shadowResolution < 256) {
		//	light.shadowResolution = 256;
		//}
		
		// light type
		//switch (light.type) {
		//	case cGraphicsLight::DIRECTIONAL : {
				// set sh map type
				//shadowMap->SetType(light.type);
				//shadowMap->ClearUnneeded();
				// get map for type
				auto& shadowMapDir = *(cShadowMapDir*)shadowMap;
				// init map if not compatible with currently inited
				//auto resolution = 2048;//== light.shadowResolution;
				//auto nCascadeQuality = light.shadowQuality;
				//try {
				//	if (!shadowMapDir.IsValid(gApi, resolution, eFormat::R24_UNORM_X8_TYPELESS, eFormat::D24_UNORM_S8_UINT, nCascadeQuality)) {
				//		shadowMapDir.Init(gApi, resolution, eFormat::R24_UNORM_X8_TYPELESS, eFormat::D24_UNORM_S8_UINT, nCascadeQuality);
				//	}
				//}
				//catch (std::exception& e) {
				//	std::cerr << e.what() << std::endl;
				//	break; // breaks switch case label
				//}

				// generate cascade splits
				size_t nCascades = shadowMapDir.GetNumCascades();
				std::vector<float> cascadeSplits(nCascades + 1, 0.0f);

				float near = parent.camera->GetNearPlane();
				float far = parent.camera->GetFarPlane();

				for (size_t i = 0; i <= nCascades; i++) {
					cascadeSplits[i] = near*pow((far / near), float(i) / float(nCascades));
				}
				for (float& v : cascadeSplits) {
					v -= near;
					v /= (far - near);
				}

				auto win = parent.GetTargetWindow();
				float aspectRatio = (float)win->GetClientWidth() / win->GetClientHeight();

				// foreach cascade
				for (size_t i = 0; i < nCascades; i++) {
					// compute transforms
					auto& transform = shadowMapDir.GetTransforms()[i];
					bool isGoodTransform = shadowMapDir.Transform(
							transform.projMat,
							transform.viewMat,
							light.GetDirection(), 
							sceneManager.GetCamera()->GetViewMatrix(), sceneManager.GetCamera()->GetProjMatrix(aspectRatio),
							cascadeSplits[i], cascadeSplits[i+1]);
					if (!isGoodTransform)
						continue;

					// setup render
					gApi->SetShaderProgram(shaderDirectional);
					ITexture2D* textureSlice = shadowMapDir.GetTexture()->GetArraySlice(i);
					gApi->SetRenderTargets(0, nullptr, textureSlice);
					gApi->ClearTexture(textureSlice);

					
					// foreach inst grp
					for (auto& entity : sceneManager.GetEntities())
					{
						Mesh* mesh = (Mesh*)entity->GetMesh();
						Material& mtl = *(Material*)entity->GetMaterial();

						// set geom params
						for (int j = 0; j < mesh->GetNumVertexBuffers(); j++)
						{
							auto stream = mesh->GetVertexBuffers()[j];
							gApi->SetVertexBuffers(&stream.vb, &stream.stride, &stream.offset, j, 1);
						}

						gApi->SetIndexBuffer(mesh->GetIndexBuffer());

						mm::mat4 posMat = mm::create_translation(entity->GetPos());
						mm::mat4 rotMat = mm::mat4(entity->GetRot());
						mm::mat4 skewMat = mm::mat4(entity->GetSkew());

						mm::mat4 worldMat = posMat * (rotMat * skewMat);

						//auto win = parent.GetTargetWindow();
						//float aspectRatio = (float)win->GetClientWidth() / win->GetClientHeight();
						//mm::mat4 projMat = sceneManager.GetCamera()->GetProjMatrix(aspectRatio);
						//mm::mat4 viewMat = sceneManager.GetCamera()->GetViewMatrix();
						mm::mat4 viewProjMat = transform.projMat * transform.viewMat;// projMat * viewMat;
						//mm::mat4 invViewProjMat = mm::inverse(viewProjMat);
						//mm::mat4 invProjMat = mm::inverse(projMat);
						mm::mat4 worldViewProj = viewProjMat * worldMat;
						gApi->SetVSConstantBuffer(&worldViewProj, sizeof(worldViewProj), 0);

						for (auto& matGroup : mesh->GetMaterialIds())
						{
							gApi->DrawIndexed((matGroup.endFace - matGroup.beginFace) * 3, matGroup.beginFace * 3);
						}
						
					}

					//gGapi->SaveTextureToFile(textureSlice, ITexture2D::BMP, "teszt.bmp");
					textureSlice->Release();
				}
				//break;
			//}
			//case cGraphicsLight::SPOT:{
			//
			//	break;
			//}
			//case cGraphicsLight::POINT:{
			//
			//	break;
			//}
		//}
	}
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