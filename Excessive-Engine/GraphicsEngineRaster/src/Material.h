#pragma once
#include "../Module_Interfaces/GraphicsEngine/IMaterial.h"

#include <mymath/mymath.h>
#include <vector>

class Texture;


class Material : public graphics::IMaterial
{
public:
	// ctor, dtor, release
	Material();
	~Material();

	void acquire();
	void release() override;

	// load
	void load(const wchar_t* file_path) override;

	void reset() override;

	// modify
	SubMaterial& addSubMaterial() override;
	i32 getNumSubMaterials() const override;
	void setNumSubMaterials(i32 n) override;
	void deleteSubMaterial(i32 index) override;

	SubMaterial& getSubMaterial(i32 index) override;
	const SubMaterial& getSubMaterial(i32 index) const override;

	SubMaterial& operator[](i32 index);
	const SubMaterial& operator[](i32 index) const;
private:
	size_t refcount;
	std::vector<SubMaterial> sub_materials;
};