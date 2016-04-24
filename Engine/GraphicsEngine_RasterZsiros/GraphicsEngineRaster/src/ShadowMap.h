////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Small shadow map texture and misc management helper class
//
////////////////////////////////////////////////////////////////////////////////

// DOC
////////////////////////////////////////
// static ViewProjMatrix
//	@return: if it could create the matrices
//	@param: light: takes positional & type information from this object
//	@param: nearClip, farClip [0.0, 1.0]: clips the projection's frustum linearly
//			Use these params for cascaded shadow maps.
//			Example: nearClip=0.0f, farClip=0.5f -> close to camera part of frustum

#pragma once
 
#include <vector>
#include "../../core/src/math/matrix44.h"
#include "../../core/src/math/quat.h"
#include "../../core/src/math/vec3.h"
#include "../../Core/src/ResourceProperty.h"
#include "GraphicsLight.h"
#include "../../GraphicsApiD3D11/src/GraphicsApiD3D11.h"

class ITexture2D;
class IGraphicsApi;


////////////////////////////////////////////////////////////////////////////////
// Dir lights
class cShadowMapDir {
public:
	// ctor
	cShadowMapDir();
	cShadowMapDir(cGraphicsApiD3D11* gApi, unsigned resolution, eFormat readFormat, eFormat depthFormat, int cascades = 1);
	~cShadowMapDir();

	//cShadowMapDir(const cShadowMapDir&);
	//cShadowMapDir& operator=(const cShadowMapDir&);

	// Init
	void Init(cGraphicsApiD3D11* gApi, unsigned resolution, eFormat readFormat, eFormat depthFormat, int cascades = 1);
	bool IsValid(cGraphicsApiD3D11* gApi, unsigned resolution, eFormat readFormat, eFormat depthFormat, int cascades = 1);
	void Clear();

	operator bool();

	// Map type
	struct tLightspace {
		mutable mm::mat4 viewMat;
		mutable mm::mat4 projMat;
	};

	// Get
	unsigned GetResolution() const;
	eFormat GetReadFormat() const;
	eFormat GetDepthFormat() const;
	int GetNumCascades() const;
	ITexture2D* GetTexture();
	const ITexture2D* GetTexture() const;
	const std::vector<tLightspace>& GetTransforms() const;

	// Transform
	static bool Transform(
		mm::mat4& projOut,
		mm::mat4& viewOut,
		const mm::vec3& lightDir,
		const mm::mat4& cameraView,
		const mm::mat4& cameraProj,
		float nearClip = 0.0f,
		float farClip = 1.0f);
private:
	unsigned resolution;
	cGraphicsApiD3D11* gApi;
	eFormat readFormat, depthFormat;
	int cascades;
	std::vector<tLightspace> maps;
	ITexture2D* texture;
};


////////////////////////////////////////////////////////////////////////////////
//	Shadow map helper class
class cShadowMap : public cShadowMapDir {
public:
	// type
	void SetType(cGraphicsLight::eLightType type) {
		this->type = type;
	}
	cGraphicsLight::eLightType GetType() {
		return type;
	}
	// clear
	void Clear() {
		((cShadowMapDir*)this)->Clear();
	}
	void ClearUnneeded() {
		switch (type) {
			case cGraphicsLight::DIRECTIONAL:{

				break;
			}
			case cGraphicsLight::SPOT:{
				((cShadowMapDir*)this)->Clear();
				break;
			}
			case cGraphicsLight::POINT:{
				((cShadowMapDir*)this)->Clear();
				break;
			}
			default:
				Clear();
		}
	}
private:
	cGraphicsLight::eLightType type;
};