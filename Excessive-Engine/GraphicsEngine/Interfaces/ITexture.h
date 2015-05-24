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
	virtual void release() = 0;

	virtual bool load(const std::wstring& file_path) = 0;

	virtual void reset() = 0;
};



} // namespace