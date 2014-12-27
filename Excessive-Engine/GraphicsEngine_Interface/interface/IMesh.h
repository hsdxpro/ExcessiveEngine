#pragma once


// namespace
namespace graphics {

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
	virtual void load(const wchar_t* file_path) = 0;

	// vertex format
	enum ElementSemantic : u32 {
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
		u32 num_components;
	};


	/// Describes a material group.
	/// Declares a range of faces to have a given ID.
	/// A face consists of 3 indices, that is, it's a triangle.
	struct MaterialGroup {
		u32 beginFace; ///< index of the first face (not index) having this id
		u32 endFace; ///< index of the face (not included in range)
		i32 id; ///< identifier of the group
	};

	// new modify
	struct MeshData {
		void* vertex_data; // tightly packed, in increasing order by type value, all 32 bit float
		u32 vertex_bytes; // size of the whole vertex_data
		ElementDesc* vertex_elements; // elements of a vertex
		u32 vertex_elements_num; // how many elements are there in a vertex
		u32* index_data; // 32 bit, tightly packed
		u32 index_num; // 3 per face
		MaterialGroup* mat_ids;
		u32 mat_ids_num; // only beginning of each group, ending calculated, in order
	};

	virtual bool update(MeshData data) = 0;
	virtual bool updateVertexData(const void* data, u32 offset, u32 size) = 0; // both params in bytes, format and total size considered the same
	virtual void reset() = 0;
};




} // namespace