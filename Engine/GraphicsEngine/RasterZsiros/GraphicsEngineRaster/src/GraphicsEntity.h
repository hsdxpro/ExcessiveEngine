////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	Scene entity for Graphics Engine
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../../Core/src/IGraphicsEntity.h"
#include "../../Core/src/Transform3D.h"

class cInstanceGroup;


class cGraphicsEntity : public IGraphicsEntity, public cTransform3D {
	friend class cSceneManager;
	friend class cGraphicsEngine;
public:
	cGraphicsEntity();

	void SetPos(const Vec3& p) override;
	void SetPos(float x, float y, float z) override;

	void SetRot(const Quat& r);
	void SetRot(float x, float y, float z, float w) override;

	void SetScale(const Vec3& s) override;
	void SetScale(float x, float y, float z) override;

	const Vec3& GetPos() const override;
	const Quat& GetRot() const override;
	const Vec3& GetScale() const override;
private:
	cInstanceGroup* instanceGroup;
	bool isVisible;
};
