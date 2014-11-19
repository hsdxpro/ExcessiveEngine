#pragma once

#include <cstdint>

// namespace
namespace ge {

////////////////////////////////////////////////////////////////////////////////
///	Mesh is a resource representing a triangle mesh.
///
////////////////////////////////////////////////////////////////////////////////


class IMesh
{
public:
	// release
	virtual void release() = 0;

	// load
	virtual void load(const char* file_path) = 0;
	virtual void load(const wchar_t* file_path) = 0;

	// modify
	virtual void setVertexFormat() = 0;

	virtual void setVertexData(const void* data, size_t size) = 0;
	virtual bool updateVertexData(const void* data, size_t size, size_t offset) = 0;

	virtual void setIndexData(const uint32_t* indices, size_t n) = 0;
	virtual bool updateIndexData(const uint32_t* indices, size_t n, size_t offset) = 0;

	virtual void setMatIds(const size_t* ids, size_t n) = 0;

	virtual void cancel() = 0;
	virtual bool compile() = 0;
	virtual void reset() = 0;
};




} // namespace