#include "Transform3D.h"

cTransform3D::cTransform3D()
:scale(1, 1, 1) {
}

Matrix44 cTransform3D::GetWorldMatrix() const {
	Matrix44 world;
		world.PostRotate(this->rot);
		world.PostScale(this->scale);
		world.PostTranslate(this->pos);
	return world;
}