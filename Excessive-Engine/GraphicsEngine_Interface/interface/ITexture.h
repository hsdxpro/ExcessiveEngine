#pragma once

// namespace
namespace ge {

////////////////////////////////////////////////////////////////////////////////
///	Texture is a resource representing an image/bitmap.
///
////////////////////////////////////////////////////////////////////////////////

class ITexture
{
public:
	virtual void release() = 0;

	virtual void load(const char* file_path) = 0;
	virtual void load(const wchar_t* file_path) = 0;
};



} // namespace