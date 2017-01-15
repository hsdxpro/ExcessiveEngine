////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer tEAM
////////////////////////////////////////////////////////////////////////////////
//	General post-processor for the renderer
//	Supported effects:
//		- depth of field
//		- motion blur
////////////////////////////////////////////////////////////////////////////////

#include "GraphicsEngine.h"

// TODO REMOVE IT OR I KILL MYSELF
#include "GraphicsEntity.h"
#include "..\..\Core\src\math\Vec2.h"
#include "GraphicsEngine_Raster\Material.h"

cGraphicsEngine::cPostProcessor::cPostProcessor(cGraphicsEngine& parent)
:parent(parent), gApi(parent.gApi)
{
	// Motion blur shaders
	shaderMB = nullptr;
	shaderMBCamera2DVelocity = nullptr;
	shaderMBObject2DVelocity = nullptr;

	// Dof + Focal plane adaption shaders
	shaderFocalPlaneAdaption = nullptr;
	shaderDOF = nullptr;
	shaderDOFBokeh = nullptr;

	// Antialiasing shader
	shaderFXAA = nullptr;

	// Screen Space Local Reflection shader
	shaderSSAR = nullptr;
	shaderSSVR = nullptr;

	// Input textures
	inputTexColor = nullptr;
	inputTexDepth = nullptr;
	inputTexNormal = nullptr;

	// Output textures
	outputTexColor = nullptr;
	outputTexVelocity2D = nullptr;
	outputTexDepthStencil = nullptr;

	// Helper textures
	focalPlaneTexA = nullptr;
	focalPlaneTexB = nullptr;

	// create shaders
	try {
		ReloadShaders();
	}
	catch (std::exception& e) {
		Cleanup();
		throw std::runtime_error(std::string("cPostProcessor: Failed to create shaders:\n") + e.what());
	}

	try {
		ReallocBuffers();
	} 
	catch (std::exception& e) {
		Cleanup();
		throw std::runtime_error(std::string("cPostProcessor: Failed to realloc buffers\n") + e.what());
	}
}

cGraphicsEngine::cPostProcessor::~cPostProcessor() {
	// release all textures
	Cleanup();
}

// Motion Blur
void cGraphicsEngine::cPostProcessor::ProcessMB(float frameDeltaTime, const Camera& cam, Scene& scene, float aspectRatio) 
{
	mm::mat4 projMat = cam.GetProjMatrix(aspectRatio);
	mm::mat4 viewMat = cam.GetViewMatrix();
	mm::mat4 viewProjMat = projMat * viewMat;

	//-------------------------------------------------------------------------------------//
	// --------- FIRST PASS : FED CAMERA BASED SKY MOTION BLUR TO VELOCITYBUFFER ----------//
	//-------------------------------------------------------------------------------------//
	// Apply to 0x00 sky stencil pixels..
	//(StencilRef & StencilMask) CompFunc (StencilBufferValue & StencilMask)
	// 0x00 & 0x11 == ( 0x00 && 0x11) // SKY	: CATCHED
	// 0x00 & 0x11 == ( 0x01 && 0x11) // OBJECT : DROPPED
	tDepthStencilDesc dsd;
		dsd.depthEnable = false;
		dsd.stencilEnable = true;
		dsd.stencilOpBackFace.stencilCompare = eComparisonFunc::EQUAL;
		dsd.stencilReadMask = 0x01;
		dsd.stencilOpFrontFace = dsd.stencilOpBackFace;
	gApi->SetDepthStencilState(dsd, 0x00);

	struct s
	{
		mm::mat4 viewProj;
		mm::mat4 invViewProj;
		mm::mat4 prevViewProj;
		mm::vec3 camPos; float pad;
		mm::vec2 InvframeDeltaTimeDiv2DivInputRes;
		mm::vec2 invFourPercentInputRes;
		mm::vec2 negInvFourPercentInputRes;
	} mbConstants;

	//mm::mat4 viewMat = cam.GetViewMatrix();
	//mm::mat4 projMat = cam.GetProjMatrix();
	mbConstants.viewProj = viewProjMat;// viewMat * projMat;
	mbConstants.invViewProj = mm::inverse(mbConstants.viewProj);
	mbConstants.prevViewProj = projMat * lastViewMat;
	mbConstants.camPos = mm::vec3( cam.GetPos().x, cam.GetPos().y, cam.GetPos().z );

	
	mbConstants.InvframeDeltaTimeDiv2DivInputRes = mm::vec2(1.0f / (frameDeltaTime * 2.0f * inputTexDepth->GetWidth()),
															1.0f / (frameDeltaTime * 2.0f * inputTexDepth->GetHeight()));

	// Increase to 60 ^^
	mbConstants.invFourPercentInputRes = mm::vec2(60.0f / inputTexDepth->GetWidth(), 60.0f / inputTexDepth->GetHeight());
	mbConstants.negInvFourPercentInputRes = -mbConstants.invFourPercentInputRes;

	mbConstants.InvframeDeltaTimeDiv2DivInputRes *= 16.0f; // Motion blur BOOST

	gApi->SetRenderTargets(1, &outputTexVelocity2D, outputTexDepthStencil);
	gApi->SetShaderProgram(shaderMBCamera2DVelocity);
	gApi->SetTexture(L"depthTexture", inputTexDepth);

	gApi->SetPSConstantBuffer(&mbConstants, sizeof(mbConstants), 0);
	gApi->SetVSConstantBuffer(&mbConstants, sizeof(mbConstants), 0);

	// Current cam became last
	lastViewMat = viewMat;

	gApi->Draw(3);
	

	//-------------------------------------------------------------------------------------//
	// ----------- SECOND PASS : FED OBJECT BASED MOTION BLUR TO VELOCITYBUFFER ------------//
	//-------------------------------------------------------------------------------------//

	// We catch on the objects and NOT SKY
	//(StencilRef & StencilMask) CompFunc (StencilBufferValue & StencilMask)
	// 0x01 & 0x01 == ( 0x00 && 0x01) // SKY	: DROPPED
	// 0x01 & 0x01 == ( 0x01 && 0x01) // OBJECT : CATCHED
	//dsd.stencilReadMask = 0x01;
	tDepthStencilDesc dsd2;
	dsd2.depthEnable = true;
	dsd2.stencilEnable = true;
	dsd2.depthCompare = eComparisonFunc::LESS_EQUAL;
	dsd2.stencilOpBackFace.stencilCompare = eComparisonFunc::EQUAL;
	dsd2.stencilReadMask = 0x01;
	dsd2.stencilOpFrontFace = dsd.stencilOpBackFace;

	gApi->ClearTexture(outputTexDepthStencil, eClearFlag::DEPTH);

	gApi->SetDepthStencilState(dsd2, 0x01);

	gApi->SetRenderTargets(1, &outputTexVelocity2D, outputTexDepthStencil);
	gApi->SetShaderProgram(shaderMBObject2DVelocity);
	
	struct s2 {
		mm::mat4 currWorldViewProj;
		mm::mat4 prevWorldViewProj;
		mm::vec2 InvframeDeltaTimeDiv2DivInputRes;
		mm::vec2 invFourPercentInputRes;
		mm::vec2 negInvFourPercentInputRes;
	} mbObjConstants;

	mbObjConstants.InvframeDeltaTimeDiv2DivInputRes = mbConstants.InvframeDeltaTimeDiv2DivInputRes;
	mbObjConstants.negInvFourPercentInputRes = mbConstants.negInvFourPercentInputRes;
	mbObjConstants.invFourPercentInputRes = mbConstants.invFourPercentInputRes;


	for (auto& entity : scene.GetEntities())
	{
		Mesh* mesh = (Mesh*)entity->GetMesh();

		if (!mesh)
			continue;

		size_t indexCount = 0;
		for (auto& a : mesh->GetMaterialIds())
		{
			indexCount += (a.endFace - a.beginFace) * 3;
		}

		// Set Geometry
		gApi->SetIndexBuffer(mesh->GetIndexBuffer());

		for (int i = 0; i < mesh->GetNumVertexBuffers(); i++)
		{
			auto stream = mesh->GetVertexBuffers()[i];
			gApi->SetVertexBuffers(&stream.vb, &stream.stride, &stream.offset, i, 1);
		}

		// Entity world matrix
		mm::mat4 posMat = mm::create_translation(entity->GetPos());
		mm::mat4 rotMat = mm::mat4(entity->GetRot());
		mm::mat4 skewMat = mm::mat4(entity->GetSkew());

		mm::mat4 worldMat = posMat * (rotMat * skewMat);

		//Matrix44 worldMat = entity->GetWorldMatrix();

		mbObjConstants.currWorldViewProj = mbConstants.viewProj * worldMat;

		// Get Prev WorldViewProj mat ( UGLY, SO SLOW, TOO MANY COPY)
		auto it = entityPrevWorldViewProjs.find(entity);
		if (it == entityPrevWorldViewProjs.end()) {
			mbObjConstants.prevWorldViewProj = mbObjConstants.currWorldViewProj;
		}
		else
			mbObjConstants.prevWorldViewProj = it->second;

		gApi->SetVSConstantBuffer(&mbObjConstants, sizeof(mbObjConstants), 0);
		gApi->SetPSConstantBuffer(&mbObjConstants, sizeof(mbObjConstants), 0);

		// draw
		gApi->DrawIndexed(indexCount);

		// curr worldviewproj become prev
		entityPrevWorldViewProjs[entity] = mbObjConstants.currWorldViewProj;
	}
	
	// Default
	tDepthStencilDesc dsdDefault;
		dsdDefault.stencilEnable = false;
		dsdDefault.depthEnable = false;
	gApi->SetDepthStencilState(dsdDefault, 0x00);

	//-------------------------------------------------------------------------------------//
	// -------------------THIRD PASS : USE VELOCITY BUFFER FOR BLURING---------------------//
	//-------------------------------------------------------------------------------------//
	gApi->SetRenderTargets(1, &outputTexColor, nullptr);
	gApi->SetShaderProgram(shaderMB);
	gApi->SetTexture(L"velocity2DTex", outputTexVelocity2D);
	gApi->SetTexture(L"inputTexture", inputTexColor);
	gApi->Draw(3);
}

void cGraphicsEngine::cPostProcessor::ProcessDOF(float frameDeltaTime, const Camera& cam, float aspectRatio) 
{
	mm::mat4 projMat = cam.GetProjMatrix(aspectRatio);
	mm::mat4 viewMat = cam.GetViewMatrix();
	mm::mat4 viewProjMat = projMat * viewMat;

	// These constants shared across two DOF specific shaders...
	// Pass1 : FocalLength adaption, only uses invViewProj, camPos
	// Pass2 : CoC calc and blur, uses all of these shits
	struct tDofConstants
	{
		mm::mat4 invViewProj;
		mm::vec3 camPos;
		float	 frameDeltaTime; // just shader :dof_focal_plane_adaption  use it
		float	 minFocalDist;	 // just shader :dof_focal_plane_adaption  use it
		float	 maxFocalDist;	 // just shader :dof_focal_plane_adaption  use it
		float	 focalAdaptSpeed;// just shader :dof_focal_plane_adaption  use it
		float	 nearPlane;
		mm::mat4 invView;
		float	 retinaLensDist;
		float	 invRetinaRadius;
		float	 inputTexWidth;
		float	 invTexWidth;
		float	 invTexHeight;
		float	 minusInvTexWidth;
		float	 minusInvTexHeight;
		float	 aperture;
		int		 quality;
	} dofConstants;

	dofConstants.invRetinaRadius = 26.793927f;// That magic number (27.793927) normalizes (a CoC that belonging to an average sized human eye and lens) into [0,1]
	dofConstants.inputTexWidth = (float)inputTexColor->GetWidth();
	dofConstants.invViewProj = mm::inverse(viewProjMat);
	dofConstants.invTexWidth = 1.0f / inputTexColor->GetWidth();
	dofConstants.invTexHeight = 1.0f / inputTexColor->GetHeight();
	dofConstants.minusInvTexWidth = -dofConstants.invTexWidth;
	dofConstants.minusInvTexHeight = -dofConstants.invTexHeight;
	dofConstants.frameDeltaTime = frameDeltaTime;
	dofConstants.retinaLensDist = 0.019f;
	dofConstants.aperture = 0.025f; // Linearly increases CoC
	dofConstants.camPos = mm::vec3(cam.GetPos().x, cam.GetPos().y, cam.GetPos().z);
	dofConstants.quality = 8;
	dofConstants.minFocalDist = 0;
	dofConstants.maxFocalDist = 3000;
	dofConstants.focalAdaptSpeed = 6.0f;
	dofConstants.nearPlane = cam.GetNearPlane();
	dofConstants.invView = mm::inverse(viewMat);

	// Set it for shaders to use
	gApi->SetPSConstantBuffer(&dofConstants, sizeof(dofConstants), 0);
	gApi->SetVSConstantBuffer(&dofConstants, sizeof(dofConstants), 0);



	//-------------------------------------------------------------------------------------//
	// --------------------------- FIRST PASS : ADAPT FOCAL PLANE -------------------------//
	//-------------------------------------------------------------------------------------//
	gApi->SetShaderProgram(shaderFocalPlaneAdaption);
	gApi->SetRenderTargets(1, &focalPlaneTexB);
	gApi->SetTexture(L"inputFocalPlaneTex", focalPlaneTexA);	// Input focal plane
	gApi->SetTexture(L"depthTexture", inputTexDepth);			// Input depth  (need posW)
	gApi->Draw(3);

	// So the result now in 'A', next time we use that to render again into B, then use a....
	std::swap(focalPlaneTexA, focalPlaneTexB);



	//-------------------------------------------------------------------------------------//
	//----------------Circle Of Confusion Calc  AND  Bluring Based On It ------------------//
	//-------------------------------------------------------------------------------------//
	gApi->SetRenderTargets(1, &outputTexColor, nullptr);
	gApi->SetShaderProgram(shaderDOF);
	gApi->SetTexture(L"adaptedFocalPlaneTex", focalPlaneTexA);	// Input focal plane
	gApi->SetTexture(L"inputTexture", inputTexColor);			// Input color
	gApi->SetTexture(L"depthTexture", inputTexDepth);			// Input depth (need posW)
	gApi->Draw(3);
}


// FXAA
void cGraphicsEngine::cPostProcessor::ProcessFXAA() 
{
	gApi->SetRenderTargets(1, &outputTexColor, nullptr);
	gApi->SetShaderProgram(shaderFXAA);

	Vec4 invResolution(	1.0f / outputTexColor->GetWidth(),
						1.0f / outputTexColor->GetHeight(),
						0, // pad
						0);// pad
	
	gApi->SetPSConstantBuffer(&invResolution, sizeof(Vec4), 0);
	gApi->SetTexture(L"inputTexture", inputTexColor);
	gApi->Draw(3);
}

void cGraphicsEngine::cPostProcessor::ProcessSSAR(const Camera& cam, float aspectRatio)
{
	mm::mat4 projMat = cam.GetProjMatrix(aspectRatio);
	mm::mat4 viewMat = cam.GetViewMatrix();
	mm::mat4 viewProjMat = projMat * viewMat;

	gApi->SetRenderTargets(1, &outputTexColor, nullptr);
	gApi->SetShaderProgram(shaderSSAR);

	struct tDofConstants
	{
		mm::mat4 invViewProj;
		mm::mat4 viewProj;
		mm::mat4 view;
		mm::mat4 invView;
		mm::vec3 camPos;		float _pad0;
		mm::vec2 invOutputRes;
		float	 farPlane;
		float	 stepLength;
		int		 maxRange;
	} sslrConstants;

	sslrConstants.view = viewMat;// cam.GetViewMatrix();
	sslrConstants.viewProj = viewProjMat;// sslrConstants.view *  cam.GetProjMatrix();
	sslrConstants.invViewProj = mm::inverse(viewProjMat);
	sslrConstants.camPos = mm::vec3(cam.GetPos().x, cam.GetPos().y, cam.GetPos().z);
	sslrConstants.invView = mm::inverse(viewMat);
	sslrConstants.invOutputRes = mm::vec2( 1.f / outputTexColor->GetWidth(), 1.f / outputTexColor->GetHeight());
	sslrConstants.farPlane = cam.GetFarPlane();
	sslrConstants.stepLength = 0.25f;
	sslrConstants.maxRange = 80;

	// Set it for shaders to use
	gApi->SetVSConstantBuffer(&sslrConstants, sizeof(sslrConstants), 0);
	gApi->SetPSConstantBuffer(&sslrConstants, sizeof(sslrConstants), 0);
	gApi->SetTexture(0, inputTexColor);
	gApi->SetTexture(1, inputTexDepth);
	gApi->SetTexture(2, inputTexNormal);
	gApi->Draw(3);
}

void cGraphicsEngine::cPostProcessor::ProcessSSVR(const Camera& cam, float aspectRatio)
{
	mm::mat4 projMat = cam.GetProjMatrix(aspectRatio);
	mm::mat4 viewMat = cam.GetViewMatrix();
	mm::mat4 viewProjMat = projMat * viewMat;

	gApi->SetRenderTargets(1, &outputTexColor, nullptr);
	gApi->SetShaderProgram(shaderSSVR);

	struct constants
	{
		mm::mat4 invViewProj;
		mm::mat4 viewProj;
		mm::mat4 view;
		mm::mat4 invView;
		mm::vec3 camPos;		float _pad0;
		mm::vec2 invOutputRes;
		float	 farPlane;
		float	 stepLength;
		int		 maxRange;
	} c;

	c.view = viewMat;// cam.GetViewMatrix();
	c.viewProj = viewProjMat;// c.view *  cam.GetProjMatrix();
	c.invViewProj = mm::inverse(c.viewProj);
	c.camPos = mm::vec3(cam.GetPos().x, cam.GetPos().y, cam.GetPos().z);
	c.invView = mm::inverse(c.view);
	c.invOutputRes = mm::vec2(1.f / outputTexColor->GetWidth(), 1.f / outputTexColor->GetHeight());
	c.farPlane = cam.GetFarPlane();
	c.stepLength = 0.25f;
	c.maxRange = 80;

	// Set it for shaders to use
	gApi->SetVSConstantBuffer(&c, sizeof(c), 0);
	gApi->SetPSConstantBuffer(&c, sizeof(c), 0);
	gApi->SetTexture(0, inputTexColor);
	gApi->SetTexture(1, inputTexDepth);
	gApi->SetTexture(2, inputTexNormal);
	gApi->Draw(3);
}

// Set inputs
void cGraphicsEngine::cPostProcessor::SetInputMB(ITexture2D* color, ITexture2D* depth) {
	assert(color); assert(depth);
	inputTexColor = color; inputTexDepth = depth;
}

void cGraphicsEngine::cPostProcessor::SetInputDOF(ITexture2D* color, ITexture2D* depth) {
	assert(color); assert(depth);
	inputTexColor = color; inputTexDepth = depth;
}

// If FXAA shader defines that it takes luminance from alpha channel, then pass appropriate texture please..
void cGraphicsEngine::cPostProcessor::SetInputFXAA(ITexture2D* color) {
	assert(color);
	inputTexColor = color;
}

void cGraphicsEngine::cPostProcessor::SetInputSSAR(ITexture2D* color, ITexture2D* depth, ITexture2D* normal) {
	inputTexColor = color;
	inputTexDepth = depth;
	inputTexNormal = normal;
}

void cGraphicsEngine::cPostProcessor::SetInputSSVR(ITexture2D* color, ITexture2D* depth, ITexture2D* normal) {
	inputTexColor = color;
	inputTexDepth = depth;
	inputTexNormal = normal;
}

void cGraphicsEngine::cPostProcessor::SetOutputMB(ITexture2D* color, ITexture2D* velocity2D, ITexture2D* depth) {
	assert(color); assert(velocity2D);
	outputTexColor = color;
	outputTexVelocity2D = velocity2D;
	outputTexDepthStencil = depth;
}

// Set outputs
void cGraphicsEngine::cPostProcessor::SetOutputDOF(ITexture2D* color) {
	assert(color);
	outputTexColor = color;
}

void cGraphicsEngine::cPostProcessor::SetOutputFXAA(ITexture2D* color) {
	assert(color);
	outputTexColor = color;
}

void cGraphicsEngine::cPostProcessor::SetOutputSSAR(ITexture2D* color) {
	assert(color);
	outputTexColor = color;
}

void cGraphicsEngine::cPostProcessor::SetOutputSSVR(ITexture2D* color) {
	assert(color);
	outputTexColor = color;
}


// INNNNERR
void cGraphicsEngine::cPostProcessor::Cleanup() {
	// Release shaders
	UnloadShaders();

	// Release textures
	SAFE_RELEASE(focalPlaneTexA);
	SAFE_RELEASE(focalPlaneTexB);
}

// unload shaders
void cGraphicsEngine::cPostProcessor::UnloadShaders() {
	SAFE_RELEASE(shaderMB);
	SAFE_RELEASE(shaderMBCamera2DVelocity);
	SAFE_RELEASE(shaderMBObject2DVelocity);
	SAFE_RELEASE(shaderDOF);
	SAFE_RELEASE(shaderFocalPlaneAdaption);
	SAFE_RELEASE(shaderFXAA);
	SAFE_RELEASE(shaderSSAR);
	SAFE_RELEASE(shaderSSVR);
	SAFE_RELEASE(shaderDOFBokeh);
}

// reload shaders
void cGraphicsEngine::cPostProcessor::ReloadShaders() {
	auto Reload = [this](IShaderProgram** prog, const wchar_t* name)->void {
		IShaderProgram* tmp = SafeLoadShader(gApi, name); // it throws on error!
		if(*prog) (*prog)->Release();
		*prog = tmp;
	};

	try {
		Reload(&shaderMB,					L"shaders/motion_blur.cg");
		Reload(&shaderMBCamera2DVelocity,	L"shaders/motion_blur_camera_2dvelocity.cg");
		Reload(&shaderMBObject2DVelocity,	L"shaders/motion_blur_object_2dvelocity.cg");
		Reload(&shaderDOF,					L"shaders/dof.cg");
		Reload(&shaderFocalPlaneAdaption,	L"shaders/dof_focal_plane_adaption.cg");
		Reload(&shaderFXAA,					L"shaders/fxaa.cg");
		Reload(&shaderSSAR,					L"shaders/ssar.cg");
		Reload(&shaderSSVR,					L"shaders/ssvr.cg");
		Reload(&shaderDOFBokeh,				L"shaders/dof_bokeh.cg");
	}
	catch (...) {
		UnloadShaders();
		throw;
	}
}

eGapiResult cGraphicsEngine::cPostProcessor::ReallocBuffers() {
	SAFE_RELEASE(focalPlaneTexA);
	SAFE_RELEASE(focalPlaneTexB);

	ITexture2D::tDesc d;
		d.arraySize = 1;
		d.bind = (int)eBind::RENDER_TARGET | (int)eBind::SHADER_RESOURCE;
		d.format = eFormat::R16_FLOAT;
		d.width = 1;
		d.height = 1;
		d.mipLevels = 1;
		d.usage = eUsage::DEFAULT;

	eGapiResult errCode = gApi->CreateTexture(&focalPlaneTexA, d);
	if (errCode != eGapiResult::OK) {
		throw std::runtime_error("cPostProcessor: Failed to realloc 1x1 focalPlane texture A");
		return errCode;
	}

	errCode = gApi->CreateTexture(&focalPlaneTexB, d);
	if (errCode != eGapiResult::OK) {
		throw std::runtime_error("cPostProcessor: Failed to realloc 1x1 focalPlane texture B");
		return errCode;
	}

	// Important to clear Textures because adaption start from dist 0
	gApi->ClearTexture(focalPlaneTexA);
	gApi->ClearTexture(focalPlaneTexB);
	return eGapiResult::OK;
}