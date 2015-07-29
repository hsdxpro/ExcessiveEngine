#pragma once
// Represent those components which can be placed in a 3D world (pos, rot, scale), attachable to Actor, and to itself
#include <vector>
#include "mymath\mymath.h"
#include "mymath\mm_quat_func.h"
#include "SupportLibrary\Transform3D.h"

class WorldComponent
{
public:
	WorldComponent();
	~WorldComponent();

public:
	virtual void UpdateAfterPhysicsSimulate();

	WorldComponent* AttachTo(WorldComponent* c);
	WorldComponent* Attach(WorldComponent* c);
	WorldComponent* Detach();
	WorldComponent* SetParent(WorldComponent* c);

	virtual void SetPos(const mm::vec3& v);
	virtual void SetRot(const mm::quat& q);
	virtual void SetScaleLocal(const mm::vec3& v);

	void Move(const mm::vec3& v);
	void Rot(const mm::quat& q);
	void Scale(const mm::vec3& v);
	void ScaleLocal(const mm::vec3& v);

	virtual void SetRelPos(const mm::vec3& v);
	virtual void SetRelRot(const mm::quat& q);
	virtual void SetRelScale(const mm::vec3& v);
	void MoveRel(const mm::vec3& v);
	void RotRel(const mm::quat& q);
	void ScaleRel(const mm::vec3& v);

	__inline WorldComponent* GetParent() const {return parent;}

	__inline const mm::vec3  GetScaleLocal() const		{return transform.GetScaleLocal();}
	__inline const mm::mat3& GetSkew()		 const		{return transform.GetSkew();}
	__inline const mm::vec3& GetPos()		 const		{return transform.GetPos();}
	__inline const mm::quat& GetRot()		 const		{return transform.GetRot();}

	__inline const mm::vec3  GetRelScaleLocal() const	{return relTransform.GetScaleLocal();}
	__inline const mm::vec3& GetRelPos()		const	{return relTransform.GetPos();}
	__inline const mm::quat& GetRelRot()		const	{return relTransform.GetRot();}
	

	__inline const Transform3D& GetRelTransform()	const {return relTransform;}
	__inline const Transform3D& GetTransform()		const {return transform;}

	__inline mm::vec3 GetDirFrontNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3( 0,  1,  0));}
	__inline mm::vec3 GetDirBackNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3( 0, -1,  0));}
	__inline mm::vec3 GetDirUpNormed()		const {return mm::rotate_vector(GetRot(), mm::vec3( 0,  0,  1));}
	__inline mm::vec3 GetDirDownNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3( 0,  0, -1));}
	__inline mm::vec3 GetDirRightNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3( 1,  0,  0));}
	__inline mm::vec3 GetDirLeftNormed()	const {return mm::rotate_vector(GetRot(), mm::vec3(-1,  0,  0));}

	__inline std::vector<WorldComponent*>& GetChilds() { return childs; }

protected:
	virtual void _InnerReflectPos() = 0;
	virtual void _InnerReflectRot() = 0;
	virtual void _InnerReflectSkew() = 0;

	void _InnerMoveChildRecursively(WorldComponent* child, const mm::vec3& dMove);
	void _InnerRotChildRecursively(WorldComponent* child, const mm::quat& dRot, const mm::vec3& transformRootPos);
	void _InnerScaleChildRecursively(WorldComponent* child, const mm::vec3& parentPos, const mm::quat& parentRot, const mm::vec3& parentLocalScale,  const mm::mat3& parentSkew, const mm::vec3& dScale, const mm::quat& rootRotInverse);

protected:
	// Child components in the tree structure
	std::vector<WorldComponent*> childs;

	// Parent component
	WorldComponent* parent;

	// World and Relative transformation
	Transform3D transform;
	Transform3D relTransform;
};