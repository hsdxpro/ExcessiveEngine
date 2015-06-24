#include "Material.h"
#include <cassert>
#include <cstring>
#include <memory>
#include <iostream> // for debug only!!


////////////////////////////////////////////////////////////////////////////////
// Constructor & destructor

Material::Material() {
	refcount = 1;
}

Material::~Material() {
	std::cout << "Material @" << this << ": kthxbai";
}


////////////////////////////////////////////////////////////////////////////////
// lifecycle

void Material::Acquire() {
	refcount++;
}

void Material::Release() {
	refcount--;
	if (refcount == 0) {
		delete this;
	}
}


////////////////////////////////////////////////////////////////////////////////
// load

void Material::Load(const wchar_t* file_path) {
	std::cout << "ha kirakod a faszod akkor sincs load" << std::endl;
}

void Material::Reset() {
	sub_materials.clear();
}

////////////////////////////////////////////////////////////////////////////////
// modify
auto Material::AddSubMaterial() -> SubMaterial& {
	sub_materials.push_back(SubMaterial());
	return sub_materials[sub_materials.size() - 1];
}
 
i32 Material::GetNumSubMaterials() const {
	return (int)sub_materials.size();
}

void Material::SetNumSubMaterials(i32 n) {
	Reset();
	sub_materials.resize(n);
}

void Material::deleteSubMaterial(i32 index) {
	assert(index < (int)sub_materials.size());
	sub_materials.erase(sub_materials.begin() + index);
}


auto Material::GetSubMaterial(i32 index) -> SubMaterial& {
	assert(index < (int)sub_materials.size());
	return sub_materials[index];
}

auto Material::GetSubMaterial(i32 index) const -> const SubMaterial&{
	assert(index < (int)sub_materials.size());
	return sub_materials[index];
}


auto Material::operator[](i32 index) -> SubMaterial& {
	return GetSubMaterial(index);
}
auto Material::operator[](i32 index) const -> const SubMaterial&{
	return GetSubMaterial(index);
}