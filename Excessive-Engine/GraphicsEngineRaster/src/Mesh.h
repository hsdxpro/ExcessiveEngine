#pragma once

#include <cstdint>
#include "../GraphicsEngine_Interface/interface/IMesh.h"


class Mesh : public ge::IMesh
{
// public interface
public:
	// ctor, dtor
	Mesh();
	~Mesh();

	// refcount / release
	void acquire();
	void release() override;

	// load
	void load(const char* file_path) override; // it is supposed to return an error code
	void load(const wchar_t* file_path) override;

	// modify
	void setVertexFormat() override;

	void setVertexData(const void* data, size_t size) override; // replace all verts
	bool updateVertexData(const void* data, size_t size, size_t offset) override; // update existing vertex buffer

	void setIndexData(const uint32_t* indices, size_t n) override; // replace all indices
	bool updateIndexData(const uint32_t* indices, size_t n, size_t offset) override; // update existing index buffer

	void setMatIds(const size_t* ids, size_t n) override; // set per-triangle mat id

	void cancel() override;
	bool compile() override; // needs to be called after setWhatever to compile new mesh
	void reset() override;

// internal mechanics
private:
	// stores raw mesh data for further processing
	struct ResourceData {
		ResourceData();
		~ResourceData();
		void* vertex_data;
		size_t vertex_bytes;
		uint32_t* index_data;
		size_t index_num;
		size_t* mat_ids;
		size_t mat_ids_num;
	};

	// optimize data for gpu drawing
	void optimize(void* vertex_data, size_t num_verts, int vertex_stride,
		uint32_t* index_data, size_t num_indices,
		size_t* mat_ids, size_t num_mat_ids);

	// validate data for out-of-bound cases
	bool validate(void* vertex_data, size_t num_verts, int vertex_stride,
		uint32_t* index_data, size_t num_indices,
		size_t* mat_ids, size_t num_mat_ids);

// private vars
private:
	size_t refcount;
	ResourceData* res_data; // temporary cache for setWhatever
};