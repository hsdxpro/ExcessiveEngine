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
	Matrix44& projOut,
	Matrix44& viewOut,
	const Vec3& lightDir,
	const Matrix44& cameraView,
	const Matrix44& cameraProj,
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
	Vec3 frustumPoints[8] = {
		{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0},
		{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1},
	};
	// inverse transform them from clip space
	Matrix44 invCameraViewProj = Matrix44Inverse(cameraView*cameraProj);
	for (auto& v : frustumPoints) {
		Vec4 v4 = v;
		v4 *= invCameraViewProj;
		v4 /= v4.w;
		v = v4;
	}
	// interpolate for near and far clips
	
	for ( int i = 0; i < 4; i++ ) 
	{
		Vec3 tmp = frustumPoints[i];
		Vec3 nearr = frustumPoints[i];
		Vec3 farr = frustumPoints[i + 4];
		frustumPoints[i] = (1 - nearClip)*nearr + nearClip*farr;
		frustumPoints[i + 4] = (1 - farClip)*nearr + farClip*farr;
	}

	// rotate them pointz as if lightDir was (0,0,-1)
	Vec3 targetLigthDir(0, 0, 1);
	Vec3 ld = Normalize(lightDir);
	auto cp = Cross(ld, targetLigthDir);
	auto dp = 1.0f + Dot(ld, targetLigthDir);
	Quat rot(cp.x, cp.y, cp.z, dp);
	rot.Normalize();

	for (auto& v : frustumPoints) {
		v *= rot;
	}
	// this rotation is the view matrix
	Matrix44 viewMat = rot;

	// fuck that, an axis aligned bounding rect's gonna be good
#pragma message("QUALITY WARN: replace this with smallest area arbitrary bounding rectangle")
	Vec3 limitMin = frustumPoints[0],
		limitMax = frustumPoints[0];
	for (auto& v : frustumPoints) {
		limitMin.x = std::min(v.x, limitMin.x);
		limitMin.y = std::min(v.y, limitMin.y);
		limitMin.z = std::min(v.z, limitMin.z);

		limitMax.x = std::max(v.x, limitMax.x);
		limitMax.y = std::max(v.y, limitMax.y);
		limitMax.z = std::max(v.z, limitMax.z);
	}

	limitMin.z = limitMax.z - 1000.0f;
	
	// check if limits have real volume or they are just 2D
	// in the latter case, no projection exists
	Vec3 volumeDim = limitMax - limitMin;
	static const float epsilon = 1e-25f;
	if (!(epsilon < volumeDim.x && epsilon < volumeDim.y && epsilon < volumeDim.z)) {
		return false;
	}

	// now make some projection from this
	Matrix44 projMat = Matrix44ProjOrtographic(limitMin.z, limitMax.z, limitMin.x, limitMax.x, limitMin.y, limitMax.y);

	// set output matrices
	viewOut = viewMat;
	projOut = projMat;

	// so far we've succeeded
	return true;
}
