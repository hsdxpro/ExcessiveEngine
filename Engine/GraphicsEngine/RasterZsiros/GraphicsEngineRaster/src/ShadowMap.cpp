////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Small shadow map texture and misc management helper class
//
////////////////////////////////////////////////////////////////////////////////


#include "ShadowMap.h"
#include "../../Core/src/GAPI.h"
#include <algorithm>
#include "../../Core/src/math/vec3.h"
#include <algorithm>

#undef min
#undef max

mm::mat4 Matrix44ViewLH2(const mm::vec3& eye, const mm::vec3& target, const mm::vec3& up)
{
	mm::vec3 baseFront = mymath::normalize(target - eye);
	mm::vec3 baseRight = mymath::normalize(mymath::cross(baseFront, up));
	mm::vec3 baseUp = mymath::cross(baseRight, baseFront);

	// Create a 4x4 orientation matrix from the right, up, and at vectors
	// TRANPOSE of ROT
	return mm::mat4(baseRight.x, baseUp.x, baseFront.x, 0,
		baseRight.y, baseUp.y, baseFront.y, 0,
		baseRight.z, baseUp.z, baseFront.z, 0,
		-mm::dot(baseRight, eye), -mm::dot(baseUp, eye), -mm::dot(baseFront, eye), 1);
}

////////////////////////////////////////////////////////////////////////////////
// Directional light

// Contructor
cShadowMapDir::cShadowMapDir() : texture(nullptr) {
}
cShadowMapDir::cShadowMapDir(cGraphicsApiD3D11* gApi, unsigned resolution, eFormat readFormat, eFormat depthFormat, int cascades) {
	Init(gApi, resolution, readFormat, depthFormat, cascades);
}
cShadowMapDir::~cShadowMapDir() {
	Clear();
}

// Init
void cShadowMapDir::Init(cGraphicsApiD3D11* gApi, unsigned resolution, eFormat readFormat, eFormat depthFormat, int cascades) {
	assert(0 < cascades);

	Clear();

	// set values
	this->cascades = cascades;
	this->depthFormat = depthFormat;
	this->readFormat = readFormat;
	this->resolution = resolution;
	this->gApi = gApi;

	// create lightspace transform table
	maps.resize(cascades);

	// create texture
	ITexture2D::tDesc desc;
	desc.arraySize = cascades;
	desc.bind = (int)eBind::DEPTH_STENCIL | (int)eBind::SHADER_RESOURCE;
	desc.format = readFormat;
	desc.depthFormat = depthFormat;
	desc.width = desc.height = resolution;
	desc.mipLevels = 1;
	desc.usage = eUsage::DEFAULT;

	auto r = gApi->CreateTexture(&texture, desc);
	if (r != eGapiResult::OK) {
		Clear();
		throw std::runtime_error("failed to create textures");
	}
}
bool cShadowMapDir::IsValid(cGraphicsApiD3D11* gApi, unsigned resolution, eFormat readFormat, eFormat depthFormat, int cascades) {
	return (
		this->cascades == cascades &&
		this->depthFormat == depthFormat &&
		this->readFormat == readFormat &&
		this->resolution == resolution &&
		this->gApi == gApi &&
		operator bool()
	);
}
void cShadowMapDir::Clear() {
	SAFE_RELEASE(texture);
	cascades = 0;
	resolution = 0;
	maps.clear();
}

cShadowMapDir::operator bool() {
	return texture != nullptr;
}

// Get
unsigned cShadowMapDir::GetResolution() const {
	return resolution;
}
eFormat cShadowMapDir::GetReadFormat() const {
	return readFormat;
}
eFormat cShadowMapDir::GetDepthFormat() const {
	return depthFormat;
}
int cShadowMapDir::GetNumCascades() const {
	return cascades;
}
ITexture2D* cShadowMapDir::GetTexture() {
	return texture;
}
const ITexture2D* cShadowMapDir::GetTexture() const {
	return texture;
}
auto cShadowMapDir::GetTransforms() const -> const std::vector<tLightspace>& {
	return maps;
}



bool cShadowMapDir::Transform(
	mm::mat4& projOut,
	mm::mat4& viewOut,
	const mm::vec3& lightDir,
	const mm::mat4& cameraView,
	const mm::mat4& cameraProj,
	float nearClip,
	float farClip)
{
	// check out that bullcrap :)
	assert(nearClip < farClip);
	assert(0.0f <= nearClip);
	assert(farClip <= 1.0f);

	// get the frustum's 8 points
	// layout is like:
	// 3,2 - these are array indices
	// 0,1 - 2d "schematic" when looking from camera
	mm::vec3 frustumPoints[8] = {
		{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0},
		{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1},
	};
	// inverse transform them from clip space
	mm::mat4 invCameraViewProj = mm::inverse( cameraProj * cameraView );
	for (auto& v : frustumPoints)
	{
		mm::vec4 v4 = mm::vec4(v, 1.0f);
		v4 = invCameraViewProj * v4;
		v4 /= v4.w;
		v = v4.xyz;
	}
	// interpolate for near and far clips
	
	//float shadowBoundMaxSize = 50;
	mm::vec3 center(0, 0, 0);
	for ( int i = 0; i < 4; i++ ) 
	{
		mm::vec3 tmp = frustumPoints[i];
		mm::vec3 nearr = frustumPoints[i];
		mm::vec3 farr = frustumPoints[i + 4];
		frustumPoints[i] = (1 - nearClip)*nearr + nearClip*farr;
		frustumPoints[i + 4] = (1 - farClip)*nearr + farClip*farr;

		//c3 nearToFarVec = frustumPoints[i + 4] - frustumPoints[i];
		//fLen = mm::length(nearToFarVec);
		//
		//en > 0 && fLen > shadowBoundMaxSize )
		//
		//oat decrementMultip = shadowBoundMaxSize / fLen;
		//ustumPoints[i + 4] = frustumPoints[i] + nearToFarVec * decrementMultip;
		//
		//
		// += frustumPoints[i];
		// += frustumPoints[i + 4];
	}


	//center = { 0,0,0 };
	mm::mat4 viewMat = Matrix44ViewLH2(center, center + lightDir, { 0,0,1 });

	for (auto& v : frustumPoints)
	{
		v = (viewMat * mm::vec4(v, 1.0f)).xyz;
	}

	// rotate them pointz as if lightDir was (0,0,-1)
	//mm::vec3 targetLigthDir(0, 0, 1);
	//mm::vec3 ld = mm::normalize(lightDir);// Normalize();0
	//auto cp = mm::cross(ld, targetLigthDir);
	//auto dp = 1.0f + mm::dot(ld, targetLigthDir);
	//mm::quat rot;
	//rot.value.x = cp.x;
	//rot.value.y = cp.y;
	//rot.value.z = cp.z;
	//rot.value.w = dp;
	//
	//rot = mm::normalize(rot);// .Normalize();
	//
	//for (auto& v : frustumPoints) {
	//	v = mm::rotate_vector(rot, v);
	//}
	//
	//// this rotation is the view matrix
	//mm::mat4 viewMat = mm::mat4(rot);

	// fuck that, an axis aligned bounding rect's gonna be good
#pragma message("QUALITY WARN: replace this with smallest area arbitrary bounding rectangle")
	mm::vec3 limitMin = mm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	mm::vec3 limitMax = mm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (auto& v : frustumPoints)
	{
		limitMin.x = std::min(v.x, limitMin.x);
		limitMin.y = std::min(v.y, limitMin.y);
		limitMin.z = std::min(v.z, limitMin.z);

		limitMax.x = std::max(v.x, limitMax.x);
		limitMax.y = std::max(v.y, limitMax.y);
		limitMax.z = std::max(v.z, limitMax.z);
	}

	//limitMin.z = limitMax.z - 1000.0f;
	
	// check if limits have real volume or they are just 2D
	// in the latter case, no projection exists
	mm::vec3 volumeDim = limitMax - limitMin;
	static const float epsilon = 1e-25f;
	if (!(epsilon < volumeDim.x && epsilon < volumeDim.y && epsilon < volumeDim.z)) {
		return false;
	}

	// now make some projection from this
	//Matrix44 projMat = Matrix44ProjOrtographic(0.1f, 1000.0f, limitMin.x, limitMax.x, limitMin.y, limitMax.y);
	Matrix44 projMat = Matrix44ProjOrtographic(0.1f, 1000.0f, -20, 20, -20, 20);
	//Matrix44 projMat = Matrix44ProjOrtographic(0.1f, 1000.0f, -1000, 1000, -1000, 1000);

	// set output matrices
	viewOut = viewMat;
	memcpy((float*)&projOut[0], (float*)&projMat[0], sizeof(float) * 16);

	// so far we've succeeded
	return true;
}
