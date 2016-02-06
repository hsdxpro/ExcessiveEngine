#include "Texture.h"
#include "GraphicsApi/IGapi.h"
#include "GraphicsApi/ITextureGapi.h"
#include <SFML/Graphics/Image.hpp>

#include <memory>
#include <iostream> // for debug only
#include <cstring>
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

void Texture::Acquire() {
	refcount++;
}

void Texture::Release() {
	refcount--;
	if (refcount == 0) {
		delete this;
	}
}


////////////////////////////////////////////////////////////////////////////////
// load

bool Texture::Load(const std::string& file_path) {
	// clean up old contents, if any
	if (tex) {
		Reset();
	}

	//const wchar_t* path = file_path.c_str();
	//
	//// load image with sfml
	//size_t len = wcslen(path);
	//char* ansiPath = new char[len+1];
	//size_t conved;
	//wcstombs_s(&conved, ansiPath, len + 1, path, len + 1);

	sf::Image im;
	bool isLoaded = im.loadFromFile(file_path.c_str());
	
	if (!isLoaded) {
		return false;
	}

	rTextureGapi texdata;
	texdata.width = im.getSize().x;
	texdata.height = im.getSize().y;
	texdata.depth = 1;
	texdata.format = eTextureFormat::RGBA8;
	texdata.is_cubemap = false;
	texdata.is_layered = false;
	texdata.num_levels = 1;

	tex = gapi->CreateTexture(texdata);
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


void Texture::Reset() {
	if (tex) {
		tex->Release();
		tex = nullptr;
	}
}


////////////////////////////////////////////////////////////////////////////////
// internal accessors
ITextureGapi* Texture::GetTexture() {
	return tex;
}