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
	/*
	virtual void setVertexFormat() = 0;

	virtual void setVertexData(const void* data, size_t size) = 0;
	virtual bool updateVertexData(const void* data, size_t size, size_t offset) = 0;

	virtual void setIndexData(const uint32_t* indices, size_t n) = 0;
	virtual bool updateIndexData(const uint32_t* indices, size_t n, size_t offset) = 0;

	virtual void setMatIds(const size_t* ids, size_t n) = 0;

	virtual void cancel() = 0;
	virtual bool compile() = 0;
	virtual void reset() = 0;
	*/


	// vertex format
	enum ElementSemantic : uint32_t {
		POSITION	= 1 << 0,
		NORMAL		= 1 << 1,
		TEX0		= 1 << 2,
		TEX1		= 1 << 3,
		TEX2		= 1 << 4,
		TEX3		= 1 << 5,
		TEX4		= 1 << 6,
		TEX5		= 1 << 7,
		TEX6		= 1 << 8,
		TEX7		= 1 << 9,
		COLOR0		= 1 << 10,
		COLOR1		= 1 << 11,
		COLOR2		= 1 << 12,
		COLOR3		= 1 << 13,
		COLOR4		= 1 << 14,
		COLOR5		= 1 << 15,
		COLOR6		= 1 << 16,
		COLOR7		= 1 << 17,
		TANGENT		= 1 << 18,
		BITANGENT	= 1 << 19,
	};

	struct ElementDesc {
		ElementSemantic semantic;
		uint32_t num_components;
	};

	// new modify
	struct MeshData {
		void* vertex_data; // tightly packed, in increasing order by type value, all 32 bit float
		size_t vertex_bytes; // size of the whole vertex_data
		ElementDesc* vertex_elements; // elements of a vertex
		uint32_t vertex_elements_num; // how many elements are there in a vertex
		uint32_t* index_data; // 32 bit, tightly packed
		size_t index_num; // 3 per face
		size_t* mat_ids;
		size_t mat_ids_num; // only beginning of each group, ending calculated, in order
	};

	virtual bool update(MeshData data) = 0;
	virtual bool updateVertexData(const void* data, size_t offset, size_t size) = 0; // both params in bytes, format and total size considered the same
	virtual void reset() = 0;
};




} // namespace