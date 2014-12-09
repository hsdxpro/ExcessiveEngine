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

void Material::acquire() {
	refcount++;
}

void Material::release() {
	refcount--;
	if (refcount == 0) {
		delete this;
	}
}


////////////////////////////////////////////////////////////////////////////////
// load

void Material::load(const char* file_path) {
	size_t s = strlen(file_path);
	auto wstr = std::make_unique<wchar_t>(s);
	return load(wstr.get());
}

void Material::load(const wchar_t* file_path) {
	std::cout << "ha kirakod a faszod akkor sincs load" << std::endl;
}

void Material::reset() {
	sub_materials.clear();
}

////////////////////////////////////////////////////////////////////////////////
// modify
auto Material::addSubMaterial() -> SubMaterial& {
	sub_materials.push_back(SubMaterial());
	return sub_materials[sub_materials.size() - 1];
}
 
int32_t Material::getNumSubMaterials() const {
	return (int)sub_materials.size();
}

void Material::setNumSubMaterials(int32_t n) {
	reset();
	sub_materials.resize(n);
}

void Material::deleteSubMaterial(int32_t index) {
	assert(index < (int)sub_materials.size());
	sub_materials.erase(sub_materials.begin() + index);
}


auto Material::getSubMaterial(int32_t index) -> SubMaterial& {
	assert(index < (int)sub_materials.size());
	return sub_materials[index];
}

auto Material::getSubMaterial(int32_t index) const -> const SubMaterial&{
	assert(index < (int)sub_materials.size());
	return sub_materials[index];
}


auto Material::operator[](int32_t index) -> SubMaterial& {
	return getSubMaterial(index);
}
auto Material::operator[](int32_t index) const -> const SubMaterial&{
	return getSubMaterial(index);
}