#include "Mesh.h"

#include <memory>
#include <iostream> // only for debug!!

#include <GraphicsApi>

#ifdef _MSC_VER
#pragma message("Mesh.cpp: Add vertex format & vertex stride")
#endif

////////////////////////////////////////////////////////////////////////////////
// Constructor & destructor

Mesh::Mesh() {
	refcount = 1;
	res_data = nullptr;
	ib = nullptr;
	vb = nullptr;
}

Mesh::~Mesh() {
	reset();
	std::cout << "Mesh @" << this << ": kthxbai";
}


////////////////////////////////////////////////////////////////////////////////
// lifecycle // GOOD

void Mesh::acquire() {
	refcount++;
}

void Mesh::release() {
	refcount--;
	if (refcount == 0) {
		delete this;
	}
}


////////////////////////////////////////////////////////////////////////////////
// load

// GOOD
void Mesh::load(const char* file_path) {
	size_t s = strlen(file_path);
	auto wstr = std::make_unique<wchar_t>(s);
	return load(wstr.get());
}

void Mesh::load(const wchar_t* file_path) {
	// TODO: implement this
}

////////////////////////////////////////////////////////////////////////////////
// modify

// vertex format
void Mesh::setVertexFormat() {
#pragma message("IMPLEMENT A VERTEX FORMAT")
}

// set vertex data to temp cache
// GOOD
void Mesh::setVertexData(const void* data, size_t size) {
	if (!res_data) {
		res_data = new ResourceData;
	}
	else if (res_data->vertex_data != nullptr) {
		operator delete(res_data->vertex_data);
		res_data->vertex_data = nullptr;
	}
	res_data->vertex_data = operator new(size);
	res_data->vertex_bytes = size;
	memcpy(res_data->vertex_data, data, size);
}

// update current vertex buffer
bool Mesh::updateVertexData(const void* data, size_t size, size_t offset) {
	return false; // TODO: implement
}


// set index data to temp cache
// GOOD
void Mesh::setIndexData(const uint32_t* indices, size_t n) {
	if (!res_data) {
		res_data = new ResourceData;
	}
	else if (res_data->index_data != nullptr) {
		delete[] res_data->index_data;
		res_data->index_data = nullptr;
	}
	res_data->index_data = new uint32_t[n];
	res_data->index_num = n;
	memcpy(res_data->index_data, indices, n*sizeof(*indices));
}

// update current index buffer
bool Mesh::updateIndexData(const uint32_t* indices, size_t n, size_t offset) {
	return false; // TODO: implement
}


// set material ids
// GOOD
void Mesh::setMatIds(const size_t* ids, size_t n) {
	if (!res_data) {
		res_data = new ResourceData;
	}
	else if (res_data->mat_ids != nullptr) {
		delete[] res_data->mat_ids;
		res_data->mat_ids = nullptr;
	}
	res_data->mat_ids = new size_t[n];
	res_data->mat_ids_num = n;
	memcpy(res_data->mat_ids, ids, n*sizeof(*ids));
}


// compile geometry:
//	- processes temp cache's data
//	- data is validated (out of bounds or whatever else)
//	- data is cache optimized for the gpu
//	- vertex and index buffers are allocated
//	- cache is deleted

bool Mesh::compile() {
	// validate input

	// optimize input

	// try to allocate new data
	// - succes -> reset & update
	// - failure -> return false

	// free raw input
	delete res_data;
	res_data = nullptr;

	return false;
}


// optimize data for gpu drawing
void Mesh::optimize(void* vertex_data, size_t num_verts, int vertex_stride,
	uint32_t* index_data, size_t num_indices,
	size_t* mat_ids, size_t num_mat_ids)
{
	// TODO: implement
	return;
}

// validate data for out-of-bound cases
// GOOD
bool Mesh::validate(void* vertex_data, size_t num_verts, int vertex_stride,
	uint32_t* index_data, size_t num_indices,
	size_t* mat_ids, size_t num_mat_ids)
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
	// - mat ids must be in order
	size_t prev = 0;
	for (size_t i = 0; i < num_mat_ids; i++) {
		if (mat_ids[i] <= prev) {
			prev = mat_ids[i];
			return false;
		}
		if (mat_ids[i] >= num_indices / 3) {
			return false;
		}
	}

	return true;
}

// cancel compilation
// GOOD
void Mesh::cancel() {
	delete res_data;
	res_data = nullptr;
}

// reset
//	- delete all underlying resources
// GOOD
void Mesh::reset() {
	delete res_data;
	res_data = nullptr;

	vb->destroy();
	ib->destroy();
	mat_ids.clear();
}


////////////////////////////////////////////////////////////////////////////////
// resource data cache

Mesh::ResourceData::ResourceData() :
vertex_data(nullptr), vertex_bytes(0), index_data(nullptr), index_num(0), mat_ids(nullptr), mat_ids_num(0)
{ /* empty on purpose */ }

Mesh::ResourceData::~ResourceData() {
	operator delete(vertex_data);
	delete[] index_data;
	delete[] mat_ids;
}