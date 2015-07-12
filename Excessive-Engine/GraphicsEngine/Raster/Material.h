#pragma once
#include <mymath/mymath.h>
#include <vector>
#include "../IMaterial.h"

class Texture;


class Material : public graphics::IMaterial
{
public:
	// ctor, dtor, release
	Material();
	~Material();

	void Acquire();
	void Release() override;

	// load
	void Load(const wchar_t* file_path) override;

	void Reset() override;

	// modify
	SubMaterial& AddSubMaterial() override;
	i32 GetNumSubMaterials() const override;
	void SetNumSubMaterials(i32 n) override;
	void deleteSubMaterial(i32 index) override;

	SubMaterial& GetSubMaterial(i32 index) override;
	const SubMaterial& GetSubMaterial(i32 index) const override;

	SubMaterial& operator[](i32 index);
	const SubMaterial& operator[](i32 index) const;
private:
	size_t refcount;
	std::vector<SubMaterial> sub_materials;
};