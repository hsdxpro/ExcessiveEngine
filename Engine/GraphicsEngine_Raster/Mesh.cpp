#include "Mesh.h"
#include "GraphicsApi/IBuffer.h"
#include "GraphicsApi/IGapi.h"

#include <memory>
#include <iostream> // only for debug!!
#include <algorithm>
#include <vector>
#include <mymath/mymath.h>



using namespace mymath;
using namespace std;


inline static int fast_log2(u32 n) {
	int i = -1;
	while (n != 0) {
		n >>= 1;
		i++;
	}
	return i;
}


////////////////////////////////////////////////////////////////////////////////
// Constructor & destructor

Mesh::Mesh(IGapi* gapi) : gapi(gapi) {
	refcount = 1;
	index_buffer = nullptr;

	num_elements = 0;
	num_streams = 0;

	for (int i = 0; i < sizeof(vertex_streams) / sizeof(vertex_streams[0]); i++) {
		vertex_streams[i].vb = nullptr;
		vertex_streams[i].stride = 0;
		vertex_streams[i].offset = 0;
		vertex_stream_content[i] = 0;
	}
	for (auto& element : elements) {
		element.buffer_index = -1;
		element.num_components = 0;
		element.offset = 0;
	}
}

Mesh::~Mesh() {
	Reset();
	std::cout << "Mesh @" << this << ": kthxbai";
}


////////////////////////////////////////////////////////////////////////////////
// lifecycle // GOOD

void Mesh::Acquire() {
	refcount++;
}

void Mesh::Release() {
	refcount--;
	if (refcount == 0) {
		delete this;
	}
}


////////////////////////////////////////////////////////////////////////////////
// load

void Mesh::Load(const wchar_t* file_path) {
	// TODO: implement this
}



////////////////////////////////////////////////////////////////////////////////
// new modify

// Update whole mesh
bool Mesh::Update(MeshData data) {
	struct Deleter {
		void operator()(void* ptr) const {
			operator delete(ptr);
		}
	};
	using RawUniquePtr = std::unique_ptr < void, Deleter > ;

	Reset();

	// a little utility for cleaning up half-baked mesh if an error occurs
	// no way I can track like 10 return statements
	struct ScopeGuardT {
		Mesh* parent;
		bool perform_cleanup;
		~ScopeGuardT() {
			if (perform_cleanup) {
				parent->Reset();
			}
		}
	} scope_guard{this, true};


	// validate input elements, calculate internal elements
	u32 element_flag = 0;
	bool is_position = false;
	bool is_tangent = false;
	bool is_normal = false;
	bool is_bitangent = false;
	num_elements = 0;

	for (u32 i = 0; i < data.vertex_elements_num; i++) {
		// check duplicate semantics
		if ((element_flag & (u32)data.vertex_elements[i].semantic) != 0) {
			return false; // error: duplicate semantic
		}
		element_flag |= data.vertex_elements[i].semantic;

		// check invalid num_components
		if ((data.vertex_elements[i].semantic == POSITION && data.vertex_elements[i].num_components != 3) ||
			(data.vertex_elements[i].semantic == NORMAL && data.vertex_elements[i].num_components != 3) ||
			(data.vertex_elements[i].semantic == TANGENT && data.vertex_elements[i].num_components != 3))
		{
			return false; // invalid component number
		}

		// monitor position and tangent
		if (data.vertex_elements[i].semantic == POSITION) {
			is_position = true;
		}
		else if (data.vertex_elements[i].semantic == TANGENT) {
			is_tangent = true;
		}
		else if (data.vertex_elements[i].semantic == NORMAL) {
			is_normal = true;
		}
		else if (data.vertex_elements[i].semantic == BITANGENT) {
			is_bitangent = true;
		}

		// Add element to internals
		elements[i] = GetBaseInfo(data.vertex_elements[i]);
		num_elements++;
	}

	if (is_bitangent ||
		(is_tangent && !is_normal) ||
		!is_position)
	{
		return false; // invalid input semantic combination
	}

	// sort internal elements
	if (is_tangent) {
		elements[data.vertex_elements_num] = GetBaseInfo(ElementDesc{BITANGENT, 3});
		num_elements++;
	}
	std::sort(elements, elements + num_elements, [](const ElementInfo& o1, const ElementInfo& o2){ return o1.semantic < o2.semantic; });



	// calculate input stride, internal stride, vertex count
	int input_stride = GetFormatStrideInput(data.vertex_elements, data.vertex_elements_num);
	int internal_stride = GetFormatStrideInternal(elements, num_elements);
	u32 num_vertices = data.vertex_bytes / (u32)input_stride;



	// validate data
	MaterialGroup default_mat_id;
	default_mat_id.beginFace = 0;
	default_mat_id.endFace = data.index_num / 3;
	if (!data.mat_ids) {
		data.mat_ids = &default_mat_id;
		data.mat_ids_num = 1;
	}

	bool is_valid = validate(num_vertices, data.index_data, data.index_num, data.mat_ids, data.mat_ids_num);
	if (!is_valid) {
		return false; // out of bounds
	}



	// pack components
	RawUniquePtr packed_vertex_data(operator new(internal_stride * num_vertices));
	packVertices(data.vertex_elements, elements, data.vertex_elements_num,  num_elements, data.vertex_data, packed_vertex_data.get(), num_vertices);



	// compute mat ids
	mat_ids.resize(data.mat_ids_num);
	for (size_t i = 0; i < data.mat_ids_num; i++) {
		mat_ids[i] = data.mat_ids[i];
	}

	// optimize mesh
	optimize(packed_vertex_data.get(), num_vertices, internal_stride, data.index_data, data.index_num, data.mat_ids, data.mat_ids_num);



	// split vertex data:
	// non-interleaved buffers now
	// todo: make grouping policies

	////////////////////////////////////
	// WARNING: 
	//	vertex buffers are subject to change

	// index buffer
	rBuffer ib_desc;
	ib_desc.is_readable = false;
	ib_desc.is_writable = true;
	ib_desc.is_persistent = false;
	ib_desc.prefer_cpu_storage = false;
	ib_desc.size = data.index_num * sizeof(u32);
	ib_desc.initial_data = nullptr;

	IIndexBuffer* _ib = gapi->CreateIndexBuffer(ib_desc);
	gapi->WriteBuffer(_ib, data.index_data, ib_desc.size, 0);

	if (!_ib) {
		return false;
	}

	// vertex buffer(s)
	rBuffer vb_desc;
	vb_desc.is_readable = false;
	vb_desc.is_writable = false;
	vb_desc.is_persistent = true;
	vb_desc.prefer_cpu_storage = false;
	vb_desc.size = internal_stride * num_vertices;

	IVertexBuffer* _vb = nullptr;
	RawUniquePtr vb_data(operator new(4 * sizeof(float) * num_vertices));

	u32 offset = 0;
	for (int i = 0; i < num_elements; i++) {

		// copy relevant stuff from packed_vertex_data
		size_t input_ptr = (size_t)packed_vertex_data.get() + offset;
		size_t output_ptr = (size_t)vb_data.get();
		u32 chunk_size = elements[i].num_components * GetElementTypeSize(elements[i].type);
		offset += chunk_size;
		for (size_t j = 0; j < num_vertices; j++) {
			memcpy((void*)output_ptr, (void*)input_ptr, chunk_size);
			input_ptr += internal_stride;
			output_ptr += chunk_size;
		}
		// Create and fill vertex buffer
		vb_desc.size = chunk_size * num_vertices;  
		_vb = gapi->CreateVertexBuffer(vb_desc);
		if (!_vb) {
			return false;
		}
		gapi->WriteBuffer(_vb, vb_data.get(), vb_desc.size, 0);
		vertex_streams[i].vb = _vb;
		vertex_streams[i].stride = chunk_size;
		vertex_streams[i].offset = 0;
	}

	// END WARNING
	////////////////////////////////////

	num_streams = num_elements;
	index_buffer = _ib;



	// Set elements (vb and offset, as other params are Set)
	// int offset = 0;
	for (int i = 0; i < num_elements; i++) {
		vertex_stream_content[i] = elements[i].semantic;
		elements[i].buffer_index = i;
		elements[i].offset = 0;
	}

	scope_guard.perform_cleanup = false;
	return true;
}

// Update vertex data
bool Mesh::UpdateVertexData(const void* data, u32 offset, u32 size) {
	std::cout << "[WARNING] updating vertex data does not work at the moment" << std::endl;
	return false;
}


// reset
//	- delete all underlying resources
void Mesh::Reset() {
	// delete and nullify buffers
	for (auto& stream : vertex_streams) {
		if (stream.vb) {
			stream.vb->Release();
			stream.vb = nullptr;
		}
		stream.stride = 0;
		stream.offset = 0;
	}
	// nullify buffer content bitmasks
	for (auto& e : vertex_stream_content) {
		e = 0;
	}
	// erase vertex elements
	for (auto& element : elements) {
		element.buffer_index = -1;
		element.num_components = 0;
		element.offset = 0;
	}
	// delete and nullify index buffer
	if (index_buffer) {
		index_buffer->Release();
		index_buffer = nullptr;
	}
	// clear mat ids
	mat_ids.clear();

	num_elements = 0;
	num_streams = 0;
}


////////////////////////////////////////////////////////////////////////////////
// helper

// optimize data for gpu Drawing
void Mesh::optimize(void* vertex_data, u32 num_verts, int vertex_stride,
	u32* index_data, u32 num_indices,
	MaterialGroup* mat_ids, u32 num_mat_ids)
{
	// TODO: implement
	return;
}


// validate data for out-of-bound cases
bool Mesh::validate(u32 num_verts,
	u32* index_data, u32 num_indices,
	MaterialGroup* mat_ids, u32 num_mat_ids)
{
	// criteria:
	// - indices num must be divisible by 3
	if (num_indices % 3 != 0) {
		return false;
	}

	// - indices must not over-index vertices
	for (size_t i = 0; i < num_indices; i++) {
		if (index_data[i] >= num_verts) {
			return false;
		}
	}

	// - mat ids must not over-index indices
	for (size_t i = 0; i < num_mat_ids; i++) {
		if (mat_ids[i].beginFace >= mat_ids[i].endFace ||
			mat_ids[i].endFace > num_indices / 3
			)
		{
			return false;
		}
	}

	return true;
}


// pack vertices to smaller format
void PackPosition(void* input, void* output) {
	float* in = (float*)input;
	float* out = (float*)output;
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}

void PackNormal(void* input, void* output) {
	memcpy(output, input, sizeof(float) * 3); // do nothing

	//float x = ((float*)input)[0],
	//	y = ((float*)input)[1],
	//	z = ((float*)input)[2];
	//u16* out = (u16*)output;
	//out[0] = 65535.f*(0.5f + 0.5f*x);
	//out[1] = 32767.5f*(0.5f + 0.5f*y) + (z > 0.0f) * 32767.5;
}

void PackTangent(void* input, void* output) {
	PackNormal(input, output);
}
void PackBitangent(void* input, void* output) {
	PackNormal(input, output);
}

void PackTexcoord(void* input, void* output, int num_components) {
	memcpy(output, input, num_components * sizeof(float));
}

void PackColor(void* input, void* output, int num_components) { // copies n bytes!
	memcpy(output, input, num_components * sizeof(float));
}

void Mesh::packVertices(ElementDesc* input_format, ElementInfo* output_format, int input_count, int output_count, void* input, void* output, u32 num_verts) {
	// Create a function that can pack 1 vertex

	// compute offsets by semantics, that is, where a semantic is found
	// semantic's index is log2(semantic)
	int offset_input[20];
	int num_components_input[20];
	int offset_output[20];
	for (int i = 0; i < 20; i++) {
		offset_input[i] = -1;
		num_components_input[i] = 0;
		offset_output[i] = -1;
	}
	for (int i = 0, offset = 0; i < input_count; i++) {
		int size = input_format[i].num_components * sizeof(float);
		offset_input[fast_log2(input_format[i].semantic)] = offset;
		num_components_input[fast_log2(input_format[i].semantic)] = input_format[i].num_components;
		offset += size;
	}
	for (int i = 0, offset = 0; i < output_count; i++) {
		int size = output_format[i].num_components * GetElementTypeSize(output_format[i].type);
		offset_output[fast_log2(output_format[i].semantic)] = offset;
		offset += size;
	}

	// enumerate output semantics
	std::vector<ElementSemantic> semantics;
	for (int i = 0; i < output_count; i++) {
		semantics.push_back(output_format[i].semantic);
	}

	// define a pack function that pack a single vertex
	auto PackVertex = [&offset_input, &offset_output, &num_components_input, semantics](void* input, void* output) {
		vec3 normal, tangent;
		bool is_bitangent = false;

		// pack each semantic
		for (auto semantic : semantics) {
			// Get attribute's offset in vertex from LUT
			int offseti = offset_input[fast_log2(semantic)]; 
			int offseto = offset_output[fast_log2(semantic)];
			// calc attribute's absolute Address
			void* off_in = (void*)(size_t(input) + offseti);
			void* off_out = (void*)(size_t(output) + offseto);

			if (semantic == POSITION) {
				PackPosition(off_in, off_out);
			}
			else if (semantic == NORMAL) {
				float* tmp = (float*)off_in;
				normal.x = tmp[0];
				normal.y = tmp[1];
				normal.z = tmp[2];

				PackNormal(off_in, off_out);
			}
			else if (semantic == TANGENT) {
				float* tmp = (float*)off_in;
				tangent.x = tmp[0];
				tangent.y = tmp[1];
				tangent.z = tmp[2];

				PackTangent(off_in, off_out);
			}
			else if (semantic == BITANGENT) {
				is_bitangent = true;
			}
			else if (COLOR0 <= semantic && semantic <= COLOR7) {
				int comp = num_components_input[fast_log2(semantic)];
				PackColor(off_in, off_out, comp);
			}
			else if (TEX0 <= semantic && semantic <= TEX7) {
				int comp = num_components_input[fast_log2(semantic)];
				PackTexcoord(off_in, off_out, comp);
			}
		}

		// compute and pack bitangent
		if (is_bitangent) {
			float tmp[3];
			vec3 bitangent = normalize(cross(normal, tangent));
			tmp[0] = bitangent.x;
			tmp[1] = bitangent.y;
			tmp[2] = bitangent.z;

			int offseto = offset_output[fast_log2(BITANGENT)];
			void* off_out = (void*)(size_t(output) + offseto);
			PackBitangent(tmp, off_out);
		}
	};


	// go through and pack all vertices
	size_t input_raw = (size_t)input;
	size_t output_raw = (size_t)output;
	size_t input_stride = GetFormatStrideInput(input_format, input_count);
	size_t output_stride = GetFormatStrideInternal(output_format, output_count);
	for (size_t vertex = 0; vertex < num_verts; vertex++) {
		PackVertex((void*)(input_raw + input_stride*vertex), (void*)(output_raw + output_stride*vertex));
	}
}


////////////////////////////////////////////////////////////////////////////////
// vertex format

bool Mesh::GetElementBySemantic(ElementInfo& info, ElementSemantic semantic) const {
	for (int i = 0; i < num_elements; i++) {
		if (semantic == elements[i].semantic) {
			info = elements[i];
			return true;
		}
	}
	return false;
}

int Mesh::GetElementsNum() const {
	return num_elements;
}

const Mesh::ElementInfo* Mesh::GetElements() const {
	return elements;
}


// Get a unique identifier corresponding to the mesh's layout
// the ID is 64 bits wide:
//	bits 0-18: streams (a 'composition' of N, where N={number of elements} )
//	bits 18-57: number of components for each element, 2 bit/element


// don't even try to understand it... http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
static inline int NumBitsSet(u32 v) {
	v = v - ((v >> 1) & 0x55555555);
	v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
	return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

u64 Mesh::GetElementConfigId() const {
	u64 id = 0;
	u32 comp = 0;

	// calculate composition
	// see http://en.wikipedia.org/wiki/Composition_(combinatorics) for nice Drawing about binary representation
	for (int i = 0, j = 0; i < num_streams; i++) {
		int weight = NumBitsSet(vertex_stream_content[i]); // how many elements does that stream have?
		for (int k = 1; k < weight; k++) {
			comp |= (1u << j);
			j++;
		}
		j++;
		assert(j <= num_elements); // indicates the streams' elements are not Set correctly
	}

	// calculate num_components
	// so... there are 5 states: 0 to 4 components, 0 means does not exist
	// which, for 20 semantics, yields log2(5^20) = 46.43 bits
	// since position is implicit, log2(5^19) = 44.11 bits -> 45 bits
	// with the 19 bits of composition, it is exactly 64 bits - cool
	// we must write this stuff in base 5 number system
	assert((num_elements == 0) || (num_elements > 0 && elements[0].semantic == POSITION)); // no implicit position if an earlier flaw
	for (int i = 1, j = 0; i < num_elements; i++, j+=2) {
		id += elements[i].num_components;
		id *= 5;
	}
	assert(id >> 45 == 0); // that should not be... see above
	id <<= 19;
	id |= comp;

	return id;
}
