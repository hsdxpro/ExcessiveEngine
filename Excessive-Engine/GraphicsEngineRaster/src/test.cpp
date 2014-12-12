#include "Mesh.h"
#include <mymath/mymath.h>
#include <iostream>
#include <bitset>

using namespace std;


struct Vertex1 {
	float x, y, z; // pos
	float nx, ny, nz; // normal
	float texcoord[2]; // texcoord
	float color[4]; // color
	float tangent[3]; // tangent
};


ostream& operator<<(ostream& os, const Mesh::ElementInfo& info) {
	os << "{" << info.buffer << ", o="<< info.offset << ", comp=" << info.num_components << ", type=" << (int)info.type << "}";
	return os;
}


extern "C"
__declspec(dllexport) void TestGraphicsEngine() {
	Mesh m(nullptr);
	Mesh::MeshData data;
	
	// vertices
	Vertex1 vertices[30];
	for (int i = 0; i < 30; i++) {
		float* data = (float*)(vertices+i);
		for (int j = 0; j < sizeof(Vertex1) / sizeof(float); j++) {
			data[j] = j;
		}
	}

	// vertex elements
	Mesh::ElementDesc vertex_elements[] = {
		Mesh::POSITION, 3,
		Mesh::NORMAL, 3,
		Mesh::TEX0, 2,
		Mesh::COLOR0, 4,
		Mesh::TANGENT, 3,
	};

	// indices
	u32 indices[30];
	for (int i = 0; i < 30; i++) {
		indices[i] = i;
	}

	// mat ids
	size_t mat_ids[] = {0, 2, 2, 9};
	size_t mat_ids_num = sizeof(mat_ids)/sizeof(mat_ids[0]);


	// setup
	data.vertex_data = vertices;
	data.vertex_bytes = sizeof(vertices);
	data.vertex_elements = vertex_elements;
	data.vertex_elements_num = sizeof(vertex_elements) / sizeof(vertex_elements[0]);

	data.index_data = indices;
	data.index_num = sizeof(indices) / sizeof(indices[0]);

	data.mat_ids = mat_ids;
	data.mat_ids_num = mat_ids_num;

	bool result = m.update(data);
 	cout << (result ? "success" : "invalid data") << endl;
	m.reset();
	m.update(data);

	// get results
	Mesh::ElementInfo info;
	if (m.getElementBySemantic(info, Mesh::NORMAL)) {
		cout << "Normal found!" << endl;
	}

	u64 id = m.getElementConfigId();
	u32 composition = id & 0x7FFFF; // lower 19 bits
	u64 components = id >> 19;
	
	cout << "id = " << bitset<64>(id) << endl;
	cout << "composition = " << bitset<19>(composition) << endl;
	cout << "components = " << bitset<40>(components) << endl;
}