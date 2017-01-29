////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	This is an interface to the resource manager for creating and registering
//	geometry data from outside the graphics engine. Geometries are made available
//	for use to create scene entites.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include "../../Core/src/vertexformat.h"

class IGeometryRef {
public:
	virtual void Release() = 0;
};


class IGeometryBuilder {
public:
	struct tMatGroup {
		size_t id, indexOffset, indexCount;
	};

	// vertex data
	virtual void SetVertexFormat(cVertexFormat format) = 0;
	virtual void InsertVertices(const void* verts, size_t bytes, size_t offset = 0) = 0;
	virtual void EraseVertices(size_t offset, size_t bytes) = 0;
	virtual void* GetVertices() = 0;
	virtual size_t GetNumVertices() = 0;
	virtual cVertexFormat GetVertexFormat() const = 0;
	// index data
	virtual void InsertIndices(uint32_t* indices, size_t count, size_t position = 0) = 0;
	virtual void EraseIndices(size_t position, size_t count) = 0;
	virtual uint32_t* GetIndices() = 0;
	virtual size_t GetNumIndices() = 0;
	// material groups
	virtual void AddMatGroup(size_t id, size_t indexOffset, size_t indexCount) = 0;
	virtual void EraseMatGroup(size_t idx) = 0;
	virtual tMatGroup* GetMatGroups() = 0;
	virtual size_t GetNumMatGroups() = 0;

	// file handling
	virtual void LoadFile(const wchar_t* path) = 0;

	// clone
	virtual void Clone(IGeometryBuilder* other) = 0;

	// submit or reset
	virtual IGeometryRef* Submit(const wchar_t* name) = 0;
	virtual void Reset() = 0;

	// politely ask for an error message
	virtual const char* YUNoWorkBitch() = 0;

	// release
	virtual void Release() = 0;
};