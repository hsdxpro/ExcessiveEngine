#include "Vec2.h"

Vec2::Vec2(float X, float Y)
:x(X), y(Y) {
}

Vec2::Vec2()
:x(0.0f), y(0.0f) {
}

Vec2 Vec2::operator - (const Vec2& p) const {
	return Vec2(x - p.x, y - p.y);
}

Vec2 Vec2::operator + (const Vec2& p) const {
	return Vec2(x + p.x, y + p.y);
}

Vec2 Vec2::operator / (const Vec2& p) const {
	return Vec2(x / p.x, y / p.y);
}

Vec2 Vec2::operator * (const Vec2& p) const {
	return Vec2(x * p.x, y * p.y);
}

Vec2 Vec2::operator - () const {
	return Vec2(-x, -y);
}

bool Vec2::operator == (const Vec2& p) const {
	return x == p.x && y == p.y;
}

Vec2& Vec2::operator *= (float s) {
	x *= s; y *= s; return *this;
}

Vec2& Vec2::operator /= (float s) {
	x /= s; y /= s; return *this;
}