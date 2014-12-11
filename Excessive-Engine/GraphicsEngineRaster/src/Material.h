#pragma once

#include <mymath/mymath.h>
#include <vector>
#include <cstdint>
#include "../GraphicsEngine_Interface/interface/IMaterial.h"

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
	void load(const char* file_path) override;
	void load(const wchar_t* file_path) override;

	void reset() override;

	// modify
	SubMaterial& addSubMaterial() override;
	int32_t getNumSubMaterials() const override;
	void setNumSubMaterials(int32_t n) override;
	void deleteSubMaterial(int32_t index) override;

	SubMaterial& getSubMaterial(int32_t index) override;
	const SubMaterial& getSubMaterial(int32_t index) const override;

	SubMaterial& operator[](int32_t index);
	const SubMaterial& operator[](int32_t index) const;
private:
	size_t refcount;
	std::vector<SubMaterial> sub_materials;
};