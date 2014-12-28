#include "Texture.h"
#include <memory>
#include <iostream> // for debug only
#include <cstring>
#include "../GraphicsApi_Interface/interface/ITexture.h"
#include "../GraphicsApi_Interface/interface/IGapi.h"
#include <SFML/Graphics/Image.hpp>

#include <fstream>


////////////////////////////////////////////////////////////////////////////////
// Constructor & destructor

Texture::Texture(IGapi* gapi) 
	: gapi(gapi), tex(nullptr)
{
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

bool Texture::load(const wchar_t* file_path) {
	// clean up old contents, if any
	if (tex) {
		reset();
	}

	// load image with sfml
	size_t len = wcslen(file_path);
	char* ansiPath = new char[len+1];
	size_t conved;
	wcstombs_s(&conved, ansiPath, len + 1, file_path, len + 1);

	sf::Image im;
	bool isLoaded = im.loadFromFile(ansiPath);
	delete[] ansiPath;
	if (!isLoaded) {
		return false;
	}

	rTexture texdata;
	texdata.width = im.getSize().x;
	texdata.height = im.getSize().y;
	texdata.depth = 1;
	texdata.format = RGBA8;
	texdata.is_cubemap = false;
	texdata.is_layered = false;
	texdata.num_levels = 1;

	tex = gapi->createTexture(texdata);
	if (!tex) {
		return false;
	}

	rTextureUpdate texupdata;
	texupdata.data = (void*)im.getPixelsPtr();
	texupdata.depth = texdata.depth;
	texupdata.format = texdata.format;
	texupdata.width = texdata.width;
	texupdata.height = texdata.height;
	texupdata.level = 0;
	texupdata.x_offset = 0;
	texupdata.y_offset = 0;
	texupdata.z_offset = 0;

	gapi->WriteTexture(tex, texupdata);

	return true;
}


void Texture::reset() {
	if (tex) {
		tex->destroy();
		tex = nullptr;
	}
}


////////////////////////////////////////////////////////////////////////////////
// internal accessors
::ITexture* Texture::getTexture() {
	return tex;
}