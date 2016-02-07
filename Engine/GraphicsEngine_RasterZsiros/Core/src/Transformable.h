////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	May wrap any object that has Pos-Rot-Scale transform functions.
//
////////////////////////////////////////////////////////////////////////////////

#include "math/vec3.h"
#include "math/quat.h"

////////////////////////////////////////////////////////////////////////////////
//	Helper classes

// base interface
class cTransWrapperBase {
public:
	cTransWrapperBase() : target(nullptr) {
	}

	virtual void SetPos(const Vec3& p) = 0;
	virtual void SetPos(float x, float y, float z) = 0;

	virtual void SetRot(const Quat& r) = 0;
	virtual void SetRot(float x, float y, float z, float w) = 0;

	virtual void SetScale(const Vec3& s) = 0;
	virtual void SetScale(float x, float y, float z) = 0;

	virtual void Copy(const cTransWrapperBase& other) = 0;

	virtual const Vec3& GetPos() const = 0;
	virtual const Quat& GetRot() const = 0;
	virtual const Vec3& GetScale() const = 0;
protected:
	void* target;
};

// template interface
template <class T>
class cTransWrapper : public cTransWrapperBase {
public:
	// ctor
	cTransWrapper(T* target) {
		this->target = (void*)target;
	}

	inline void Copy(const cTransWrapperBase& other) override {
		SetPos(other.GetPos());
		SetRot(other.GetRot());
		SetScale(other.GetScale());
	}

	// pos
	inline void SetPos(const Vec3& p) override {
		((T*)target)->SetPos(p);
	}
	inline void SetPos(float x, float y, float z) override {
		((T*)target)->SetPos(Vec3(x, y, z));
	}

	// scale
	inline void SetRot(const Quat& r) override {
		((T*)target)->SetRot(r);
	}
	inline void SetRot(float x, float y, float z, float w) override {
		((T*)target)->SetRot(Quat(x, y, z, w));
	}

	// rot
	inline void SetScale(const Vec3& s) override {
		((T*)target)->SetScale(s);
	}
	inline void SetScale(float x, float y, float z) override {
		((T*)target)->SetScale(Vec3(x, y, z));
	}

	// getters
	inline const Vec3& GetPos() const override {
		return ((T*)target)->GetPos();
	}
	inline const Quat& GetRot() const override {
		return ((T*)target)->GetRot();
	}
	inline const Vec3& GetScale() const override {
		return ((T*)target)->GetScale();
	}
};


////////////////////////////////////////////////////////////////////////////////
//	Actual transformable object class
class cTransformable {
public:
	// ctors
	cTransformable() {
		return;
	}
	template <class T>
	cTransformable(T* target);

	// operators
	template <class T> 
	cTransformable& operator=(T* target);
	
	// noooo, this is just bad
	cTransformable* operator->() {
		return this;
	}

	// bullshit boilerplate
	inline void Copy(const cTransformable& other) {
		((cTransWrapperBase*)wrapperDummy)->Copy(*(cTransWrapperBase*)other.wrapperDummy);
	}


	// pos
	inline void SetPos(const Vec3& p) {
		((cTransWrapperBase*)wrapperDummy)->SetPos(p);
	}
	inline void SetPos(float x, float y, float z) {
		((cTransWrapperBase*)wrapperDummy)->SetPos(x, y, z);
	}

	// scale
	inline void SetRot(const Quat& r) {
		((cTransWrapperBase*)wrapperDummy)->SetRot(r);
	}
	inline void SetRot(float x, float y, float z, float w) {
		((cTransWrapperBase*)wrapperDummy)->SetRot(x, y, z, w);
	}

	// rot
	inline void SetScale(const Vec3& s) {
		((cTransWrapperBase*)wrapperDummy)->SetScale(s);
	}
	inline void SetScale(float x, float y, float z) {
		((cTransWrapperBase*)wrapperDummy)->SetScale(x, y, z);
	}

	// getters
	inline const Vec3& GetPos() const {
		return ((cTransWrapperBase*)wrapperDummy)->GetPos();
	}
	inline const Quat& GetRot() const {
		return ((cTransWrapperBase*)wrapperDummy)->GetRot();
	}
	inline const Vec3& GetScale() const {
		((cTransWrapperBase*)wrapperDummy)->GetScale();
	}
private:
	char wrapperDummy[sizeof(cTransWrapper<cTransWrapperBase>)];
};


template <class T>
cTransformable::cTransformable(T* target) {
	new (wrapperDummy)cTransWrapper<T>(target);
}

template <class T>
cTransformable& cTransformable::operator=(T* target) {
	new (wrapperDummy)cTransWrapper<T>(target);

	return *this;
}