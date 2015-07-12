#pragma once
#include <string>

// namespace
namespace graphics {

////////////////////////////////////////////////////////////////////////////////
///	Texture is a resource representing an image/bitmap.
///
////////////////////////////////////////////////////////////////////////////////

class ITexture
{
public:
	virtual void Release() = 0;

	virtual bool Load(const std::wstring& file_path) = 0;

	virtual void Reset() = 0;
};



} // namespace