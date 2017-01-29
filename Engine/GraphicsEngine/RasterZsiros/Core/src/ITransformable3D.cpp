#include "ITransformable3D.h"

void ITransformable3D::SetWorldTransform(ITransformable3D* t) {
	SetPos(t->GetPos());
	SetRot(t->GetRot());
	SetScale(t->GetScale());
}