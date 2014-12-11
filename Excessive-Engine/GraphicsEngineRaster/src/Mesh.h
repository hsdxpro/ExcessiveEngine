#pragma once

#include <cstdint>
#include <vector>
#include <limits>

#include "../../GraphicsApi_Interface/interface/IInputLayout.h"
#include "../GraphicsEngine_Interface/interface/IMesh.h"

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
	void load(const char* file_path) override; // it is supposed to return an error code
	void load(const wchar_t* file_path) override;

	// new modify
	bool update(MeshData data) override;
	bool updateVertexData(const void* data, size_t offset, size_t size) override; // both params in bytes, format and total size considered the same

	void reset() override;

	// interface end
	////////////////////////////////////

	// format of the data stored

	enum ElementType : uint32_t {
		FLOAT = 1,
		SINT,
		UINT,
		SNORM,
		UNORM,
	};

	// helps lookup of vb by element semantic
	struct ElementInfo {
		IVertexBuffer* buffer;
		size_t offset; // offset from beginning of vertex
		ElementSemantic semantic;
		ElementType type;
		uint32_t width;
		uint32_t num_components;
	};

	// defines a stream
	struct VertexStream {
		IVertexBuffer* vb;
		unsigned elements;
	};

	// getters
	bool getElementBySemantic(ElementInfo& info, ElementSemantic semantic) const;
	int getElementsNum() const;
	const ElementInfo* getElements() const;
	uint64_t getElementConfigId() const;

	IIndexBuffer* getIndexBuffer() { return ib; }
	const std::vector<size_t>& getMaterialIds() { return mat_ids; }

// internal mechanics
protected:
	// optimize data for gpu drawing
	void optimize(void* vertex_data, size_t num_verts, int vertex_stride,
		uint32_t* index_data, size_t num_indices,
		size_t* mat_ids, size_t num_mat_ids);

	// validate data for out-of-bound cases
	bool validate(size_t num_verts,
		uint32_t* index_data, size_t num_indices,
		size_t* mat_ids, size_t num_mat_ids);




	// vertex packing
	void packVertices(ElementDesc* input_format, ElementInfo* output_format, int input_count, int output_count, void* input, void* output, size_t num_verts); // required size assumed

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
			info.num_components = 2;
			info.semantic = desc.semantic;
			info.type = UNORM;
			info.width = 16;
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
			info.type = UNORM;
			info.width = 16;
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
	std::vector<size_t> mat_ids;

// private vars
private:
	size_t refcount;
	IGapi* gapi;
};