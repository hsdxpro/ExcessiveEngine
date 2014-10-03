#include "Texture.h"
#include <memory>
#include <iostream> // for debug only


////////////////////////////////////////////////////////////////////////////////
// Constructor & destructor

Texture::Texture() {
	refcount = 1;
}

Texture::~Texture() {
	std::cout << "Texture @" << this << ": kthxbai";
}


////////////////////////////////////////////////////////////////////////////////
// lifecycle

void Texture::acquire() {
	refcount++;
}

void Texture::release() {
	refcount--;
	if (refcount == 0) {
		delete this;
	}
}


////////////////////////////////////////////////////////////////////////////////
// load

void Texture::load(const char* file_path) {
	size_t s = strlen(file_path);
	auto wstr = std::make_unique<wchar_t>(s);
	return load(wstr.get());
}

void Texture::load(const wchar_t* file_path) {

}