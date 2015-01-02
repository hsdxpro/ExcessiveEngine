#pragma once

#include "../../Interfaces/IMesh.h"

#include <vector>
#include <limits>


class IVertexBuffer;
class IIndexBuffer;
class IGapi;

////////////////////////////////////////////////////////////////////////////////
// Notes
//
// Update this if you change something. Really.
//
// What is this class?
//	This class manages triangle mesh data.
//	Data is stored in vertex arrays and index arrays. For the index array, each
//	face has a material group, which is optional when rendering.
//
// Material groups
//	The id's are in an array. A i-th group is defined as the range [face[mtl_id[i]], face[mtl_id[i+1]]).
//	The (i+1)th element is in the list.
//
// Vertex format
//	A vertex element's format is always defined by it's semantic, though number
//	of components in the data vector may vary.
//	The data format can be accessed by the corresponding function.
//	Vertex elements may be distributed accross several streams.
//	A short 64 bit id can be used for hashing vertex formats.
// Format by semantic:
//	<semantic>	<type>	<width>	<num comp.>
//	POSITION	float	32		3
//	NORMAL		unorm	16		2
//	TEXi		unorm	16		changeable
//	COLORi		float	32		changeable
//	TANGENT		unorm	16		2
//	BITANGENT	unorm	16		2
//	Check packing function in .cpp for more info.
// Devs (internal behaviour):
//	The vertex elements are always in increasing order by their semantics (integer value of enum).
//	That is, a smaller semantic is either at smaller offset in the same buffer, or
//	in a different buffer with smaller index. Otherwise stream can have any number of elements.
//	These restriction allow the calculation of the 'short id', see its function.
//
////////////////////////////////////////////////////////////////////////////////


class Mesh : public graphics::IMesh
{
// public interface
public:
	// ctor, dtor
	Mesh(IGapi* gapi);
	~Mesh();

	////////////////////////////////////
	// interface

	// refcount / release
	void acquire();
	void release() override;

	// load
	void load(const wchar_t* file_path) override; // it is supposed to return an error code

	// new modify
	bool update(MeshData data) override;
	bool updateVertexData(const void* data, u32 offset, u32 size) override; // both params in bytes, format and total size considered the same

	void reset() override;

	// interface end
	////////////////////////////////////

	// format of the data stored

	enum ElementType : u32 {
		FLOAT = 1,
		SINT,
		UINT,
		SNORM,
		UNORM,
	};

	// helps lookup of vb by element semantic
	struct ElementInfo {
		IVertexBuffer* buffer;
		u32 offset; // offset from beginning of vertex
		ElementSemantic semantic;
		ElementType type;
		u32 width;
		u32 num_components;
	};

	// defines a stream
	struct VertexStream {
		IVertexBuffer* vb;
		u32 elements;
	};

	// getters
	bool getElementBySemantic(ElementInfo& info, ElementSemantic semantic) const;
	int getElementsNum() const;
	const ElementInfo* getElements() const;
	u64 getElementConfigId() const;

	IIndexBuffer* getIndexBuffer() { return ib; }
	const std::vector<MaterialGroup>& getMaterialIds() { return mat_ids; }

// internal mechanics
protected:
	// optimize data for gpu drawing
	void optimize(void* vertex_data, u32 num_verts, int vertex_stride,
		u32* index_data, u32 num_indices,
		MaterialGroup* mat_ids, u32 num_mat_ids);

	// validate data for out-of-bound cases
	bool validate(u32 num_verts,
		u32* index_data, u32 num_indices,
		MaterialGroup* mat_ids, u32 num_mat_ids);




	// vertex packing
	void packVertices(ElementDesc* input_format, ElementInfo* output_format, int input_count, int output_count, void* input, void* output, u32 num_verts); // required size assumed

	// helpers with the format
	inline int getFormatStrideInput(ElementDesc* elements, int num_elements) {
		int s = 0;
		for (int i = 0; i < num_elements; i++) {
			s += elements[i].num_components * sizeof(float);
		}
		return s;
	}
	inline int getFormatStrideInternal(ElementInfo* elements, int num_elements) {
		int s = 0;
		for (int i = 0; i < num_elements; i++) {
			s += elements[i].num_components * elements[i].width / 8;
		}
		return s;
	}
	ElementInfo getBaseInfo(ElementDesc desc) {
		ElementSemantic semantic = desc.semantic;

		ElementInfo info;
		info.buffer = nullptr;
		info.offset = 0;
		if (semantic == POSITION) {
			info.num_components = 3;
			info.semantic = POSITION;
			info.type = FLOAT;
			info.width = 32;
		}
		else if (semantic == NORMAL || semantic == TANGENT || semantic == BITANGENT) {
			info.num_components = 3;
			info.semantic = desc.semantic;
			info.type = FLOAT;
			info.width = 32;
		}
		else if (COLOR0 <= semantic && semantic <= COLOR7) {
			info.num_components = desc.num_components;
			info.semantic = desc.semantic;
			info.type = FLOAT;
			info.width = 32;
		}
		else if (TEX0 <= semantic && semantic <= TEX7) {
			info.num_components = desc.num_components;
			info.semantic = desc.semantic;
			info.type = FLOAT;
			info.width = 32;
		}
		else {
			info.num_components = 0;
			info.width = 0;
		}

		return info;
	}

	// mesh gpu resource
	VertexStream streams[20];
	ElementInfo elements[20];
	int num_streams;
	int num_elements;
	IIndexBuffer* ib;
	std::vector<MaterialGroup> mat_ids;

// private vars
private:
	size_t refcount;
	IGapi* gapi;
};