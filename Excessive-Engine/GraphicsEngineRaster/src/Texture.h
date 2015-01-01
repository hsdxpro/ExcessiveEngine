#pragma once

#include "../Module_Interfaces/GraphicsEngine/ITexture.h"

class ITexture;
class IGapi;


class Texture : public graphics::ITexture 
{
public:
	Texture(IGapi* gapi);
	~Texture();

	void acquire();
	void release() override;

	bool load(const wchar_t* file_path) override;

	void reset() override;

	::ITexture* getTexture();
private:
	size_t refcount;

	// gpu resource
	IGapi* gapi;
	::ITexture* tex;
};