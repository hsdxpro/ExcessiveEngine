#pragma once
#include "..\ITexture.h"

class ITextureGapi;
class IGapi;


class Texture : public graphics::ITexture 
{
public:
	Texture(IGapi* gapi);
	~Texture();

	void Acquire();
	void Release() override;

	bool Load(const std::wstring& file_path) override;

	void Reset() override;

	ITextureGapi* GetTexture();

private:
	size_t refcount;

	// gpu resource
	IGapi* gapi;
	ITextureGapi* tex;
};