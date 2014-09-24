#include "Material.h"
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

}


void Material::load(const wchar_t* file_path) {

}