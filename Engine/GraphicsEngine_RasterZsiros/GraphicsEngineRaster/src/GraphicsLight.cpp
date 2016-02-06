////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Graphics engine's light object
//		implementation
////////////////////////////////////////////////////////////////////////////////


#include "GraphicsLight.h"




// default ctor for default values
cGraphicsLight::cGraphicsLight()
: // init list
type(AMBIENT),
color(0,0,0),
position(0,0,0),
direction(0,0,-1),
atten0(0),
atten1(0),
atten2(0),
range(1),
smallAngle(0.5),
bigAngle(1.0),
cutoff(0.666),
shadowResolution(2048),
shadowQuality(VERY_HIGH)
{
}

// set
void cGraphicsLight::SetType(eLightType type) {
	this->type = type;
}

void cGraphicsLight::SetColor(const mm::vec3& color) {
	this->color = color;
}
void cGraphicsLight::SetPosition(const mm::vec3& pos) {
	this->position = pos;
}
void cGraphicsLight::SetDirection(const mm::vec3& dir) {
	this->direction = dir;
}

void cGraphicsLight::SetAtten(float atten0, float atten1, float atten2) {
	this->atten0 = atten0;
	this->atten1 = atten1;
	this->atten2 = atten2;
}
void cGraphicsLight::SetAtten0(float atten0) {
	this->atten0 = atten0;
}
void cGraphicsLight::SetAtten1(float atten1) {
	this->atten1 = atten1;
}
void cGraphicsLight::SetAtten2(float atten2) {
	this->atten2 = atten2;
}

void cGraphicsLight::SetRange(float range) {
	this->range = range;
}
void cGraphicsLight::SetAngle(float inner, float outer) {
	this->bigAngle = outer;
	this->smallAngle = inner;
}
void cGraphicsLight::SetAngleInner(float inner) {
	smallAngle = inner;
}
void cGraphicsLight::SetAngleOuter(float outer) {
	bigAngle = outer;
}
void cGraphicsLight::SetCutoff(float cutoff) {
	this->cutoff = cutoff;
}

void cGraphicsLight::SetShadowResolution(unsigned pixels) {
	shadowResolution = pixels;
}
void cGraphicsLight::SetShadowQuality(eShadowQuality quality) {
	shadowQuality = quality;
}

// get
auto cGraphicsLight::GetType() const -> eLightType {
	return type;
}
mm::vec3 cGraphicsLight::GetColor() const {
	return color;
}
mm::vec3 cGraphicsLight::GetPosition() const {
	return position;
}
mm::vec3 cGraphicsLight::GetDirection() const {
	return direction;
}

float cGraphicsLight::GetAtten0() const {
	return atten0;
}
float cGraphicsLight::GetAtten1() const {
	return atten1;
}
float cGraphicsLight::GetAtten2() const {
	return atten2;
}

float cGraphicsLight::GetRange() const {
	return range;
}
float cGraphicsLight::GetAngleInner() const {
	return smallAngle;
}
float cGraphicsLight::GetAngleOuter() const {
	return bigAngle;
}
float cGraphicsLight::GetCutoff() const {
	return cutoff;
}

unsigned cGraphicsLight::GetShadowResolution() const {
	return shadowResolution;
}

auto cGraphicsLight::GetShadowQuality() const -> eShadowQuality {
	return shadowQuality;
}

// enable/disable
void cGraphicsLight::Enable(bool val) {
	enabled = val;
}
bool cGraphicsLight::IsEnabled() const {
	return enabled;
}