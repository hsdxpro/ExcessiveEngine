#pragma once
#include "../../Interfaces/ITexture.h"

class ITextureGapi;
class IGapi;


class Texture : public graphics::ITexture 
{
public:
	Texture(IGapi* gapi);
	~Texture();

	void acquire();
	void release() override;

	bool load(const std::wstring& file_path) override;

	void reset() override;

	ITextureGapi* getTexture();

private:
	size_t refcount;

	// gpu resource
	IGapi* gapi;
	ITextureGapi* tex;
};