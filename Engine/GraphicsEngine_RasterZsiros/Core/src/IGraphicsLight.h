////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Scene light interface
////////////////////////////////////////////////////////////////////////////////

#include "../../Core/src/math/vec4.h"
#include "../../Core/src/math/vec3.h"
#include "mymath/mymath.h"



class IGraphicsLight {
public:
	//virtual ~IGraphicsLight() {}; // should NOT be deleted or released, but RemoveLighted

	// light types
	enum eLightType {
		AMBIENT = 1,
		POINT,
		SPOT,
		DIRECTIONAL,
	};
	enum eShadowQuality {
		DISABLED = 0,
		LOW = 1,
		MEDIUM = 2,
		HIGH = 3,
		VERY_HIGH = 4,
	};

	// set
	virtual void SetType(eLightType type) = 0;

	virtual void SetColor(const mm::vec3& color) = 0;
	virtual void SetPosition(const mm::vec3& pos) = 0;
	virtual void SetDirection(const mm::vec3& dir) = 0;

	virtual void SetAtten(float atten0, float atten1, float atten2) = 0;
	virtual void SetAtten0(float atten0) = 0;
	virtual void SetAtten1(float atten1) = 0;
	virtual void SetAtten2(float atten2) = 0;

	virtual void SetRange(float range) = 0;
	virtual void SetAngle(float inner, float outer) = 0;
	virtual void SetAngleInner(float inner) = 0;
	virtual void SetAngleOuter(float outer) = 0;
	virtual void SetCutoff(float cutoff) = 0;

	virtual void SetShadowResolution(unsigned pixels) = 0;
	virtual void SetShadowQuality(eShadowQuality quality) = 0;

	// get
	virtual eLightType GetType() const = 0;
	virtual mm::vec3 GetColor() const = 0;
	virtual mm::vec3 GetPosition() const = 0;
	virtual mm::vec3 GetDirection() const = 0;

	virtual float GetAtten0() const = 0;
	virtual float GetAtten1() const = 0;
	virtual float GetAtten2() const = 0;

	virtual float GetRange() const = 0;
	virtual float GetAngleInner() const = 0;
	virtual float GetAngleOuter() const = 0;
	virtual float GetCutoff() const = 0;

	virtual unsigned GetShadowResolution() const = 0;
	virtual eShadowQuality GetShadowQuality() const = 0;

	// enable/disable
	virtual void Enable(bool val) = 0;
	virtual bool IsEnabled() const = 0;
};
