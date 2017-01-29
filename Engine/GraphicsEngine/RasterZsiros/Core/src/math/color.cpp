#include "color.h"
 
Colorf::Colorf() 
:r(0.0f), g(0.0f), b(0.0f), a(0.0f)
{}

Colorf::Colorf(float m) 
:r(m), g(m), b(m), a(1.0f) 
{}

Colorf::Colorf(float r, float g, float b, float a) 
:r(r), g(g), b(b), a(a)
{}

Colorf::Colorf(float r, float g, float b) 
:r(r), g(g), b(b), a(1.0f)
{}

Colorf& Colorf::operator *= (const Colorf& c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}
Colorf& Colorf::operator /= (const Colorf& c) {
	r /= c.r;
	g /= c.g;
	b /= c.b;
	return *this;
}

Colorf& Colorf::operator += (const Colorf& c) {
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

Colorf& Colorf::operator -= (const Colorf& c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return *this;
}

Colorf& Colorf::operator *= (const float& s) {
	r *= s;
	g *= s;
	b *= s;
	return *this;
}
Colorf& Colorf::operator /= (float s) {
	s=1.0f / s;
	r *= s;
	g *= s;
	b *= s;
	return *this;
}

Colorf Colorf::operator * (const Colorf& c) {
	Colorf cr = *this;
	return cr *= c;
}

Colorf Colorf::operator / (const Colorf& c) {
	Colorf cr = *this;
	return cr /= c;
}

Colorf Colorf::operator + (const Colorf& c) {
	Colorf cr = *this;
	return cr += c;
}

Colorf Colorf::operator - (const Colorf& c) {
	Colorf cr = *this;
	return cr -= c;
}

Colorf Colorf::operator * (const float& s) {
	Colorf cr = *this;
	return cr *= s;
}
Colorf Colorf::operator / (const float& s) {
	Colorf cr = *this;
	return cr /= s;
}

float Colorf::lightness() {
	return (r + g + b) * 0.3333333f;
}

Colorf& Colorf::tomonochrome() {
	(*this) *= this->lightness();
	return *this;
}