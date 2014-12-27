#pragma once

#include "..\Common\src\BasicTypes.h"

// namespace
namespace graphics {

class ITexture;

////////////////////////////////////////////////////////////////////////////////
///	Material is a resource representing surface properties.
///
////////////////////////////////////////////////////////////////////////////////

class IMaterial 
{
public:
	// sub-material struct
	struct SubMaterial
	{
		SubMaterial() : // default ctor won't hurt, but don't rely on it!
			base(0, 0, 0, 1), emissive(0, 0, 0, 1), roughness(1), metallic(0), specular(0.5),
			t_diffuse(nullptr), t_specular(nullptr), t_normal(nullptr), t_displace(nullptr) {}
		mm::vec4 base, emissive;
		float roughness, metallic/*a*/, specular;
		ITexture *t_diffuse, *t_specular, *t_normal, *t_displace;
	};

	// release
	virtual void release() = 0;

	// load
	virtual void load(const wchar_t* file_path) = 0;

	virtual void reset() = 0;

	// manage contents
	virtual SubMaterial& addSubMaterial() = 0; // returns ref
	virtual i32 getNumSubMaterials() const = 0;
	virtual void setNumSubMaterials(i32 n) = 0; // warning! stuff is deleted
	virtual void deleteSubMaterial(i32 index) = 0;
	virtual SubMaterial& getSubMaterial(i32 index) = 0;
	virtual const SubMaterial& getSubMaterial(i32 index) const = 0;
};

} // namespace