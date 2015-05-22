#include "AnimHand.h"
#include <mymath/mymath.h>
#include <algorithm>
#include "Model.h"
#include <cstdint>
#include <iostream>


using namespace std;


float DistanceFromLine(mm::vec3 l1, mm::vec3 l2, mm::vec3 p) {
	// check weather its in the line segment
	if (mm::dot(l2 - l1, p - l1) > 0 && mm::dot(l1 - l2, p - l2) > 0) {
		/*
		float c = mm::distance(p, l2);
		float cosAlpha = mm::dot(mm::normalize(l1-l2), mm::normalize(p-l2));
		float a = c*cosAlpha;
		float A = mm::distance(l1, l2);
		float t = a / A;
		mm::vec3 pc = t*l2 + (1 - t)*l1;
		//return 0.001;
		return mm::distance(p, pc);
		*/
		float d = 1e10;
		for (float t = 0.0; t <= 1.0; t += 0.01) {
			d = std::min(d, mm::distance(t*l1 + (1-t)*l2, p));
		}
		return d;
	}
	// not in line segment
	else {
		return min(mm::distance(l1, p), mm::distance(l2, p));
	}
}

using Bone = KeyframeAnimation::Bone;
struct Line {
	mm::vec3 start, end;
};

size_t numChildren[] = {1,1,0};
Bone defBones[] = {
	mm::vec3(0,0,0), mm::quat(0, mm::vec3(1,0,0)), mm::vec3(1,1,1), 0.25,
	mm::vec3(0,0,0), mm::quat(0, mm::vec3(1,0,0)), mm::vec3(1,1,1), 0.29,
	mm::vec3(0,0,0), mm::quat(0, mm::vec3(1,0,0)), mm::vec3(1,1,1), 0.20,
};
Line defBoneLines[] = {
	mm::vec3(0,0,0), mm::vec3(0,0.25,0),
	mm::vec3(0,0.25,0), mm::vec3(0,0.54,0),
	mm::vec3(0,0.54,0), mm::vec3(0,0.74,0),
};

Bone kfBones[3][3] = {
	{
	mm::vec3(0,0,0), mm::quat(0, mm::vec3(1,0,0)), mm::vec3(1,1,1), 0.25,
	mm::vec3(0,0,0), mm::quat(0, mm::vec3(1,0,0)), mm::vec3(1,1,1), 0.29,
	mm::vec3(0,0,0), mm::quat(0, mm::vec3(1,0,0)), mm::vec3(1,1,1), 0.20,
	},

	{
	mm::vec3(0,0,0), mm::quat(0, mm::vec3(1,0,0)), mm::vec3(1,1,1), 0.25,
	mm::vec3(0,0.15,0), mm::quat(0.25, mm::vec3(0,0,1)), mm::vec3(1,1,1), 0.29,
	mm::vec3(0,0,0), mm::quat(0.25, mm::vec3(1,0,0)), mm::vec3(1,1,1), 0.20,
	},

	{
	mm::vec3(0,0,0), mm::quat(-0.5, mm::vec3(0,0,1)), mm::vec3(1,1,1), 0.25,
	mm::vec3(0,0.2,0), mm::quat(0.5, mm::vec3(0,0,1)), mm::vec3(1,1,1), 0.29,
	mm::vec3(0,0,0), mm::quat(1, mm::vec3(1,0,0)), mm::vec3(1,2,1), 0.20,
	},
};


struct AnimVertex {
	mm::vec3 pos;
	mm::vec3 normal;
	mm::vec2 tex;
	mm::vec4 boneIndex;
	mm::vec4 boneWeight;
};

bool CreateAnimatedHand(graphics::IEngine* engine, graphics::IMesh*& mesh, KeyframeAnimation& animation) {
	size_t numVertices = sizeof(vertices) / sizeof(vertices[0]);
	AnimVertex animVertices[sizeof(vertices) / sizeof(vertices[0])];

	// copy data
	for (size_t i = 0; i < numVertices; ++i) {
		animVertices[i].pos = vertices[i].pos;
		animVertices[i].normal = vertices[i].normal;
		animVertices[i].tex = vertices[i].tex;
	}

	// compute bone weight and indices
	for (size_t i = 0; i < numVertices; ++i) {
		for (size_t j = 0; j < sizeof(defBoneLines) / sizeof(defBoneLines[0]); j++) {
			float distance = DistanceFromLine(defBoneLines[j].start, defBoneLines[j].end, animVertices[i].pos);
			distance = std::max(0.0f, 0.10f - distance);
			animVertices[i].boneWeight[j] = distance;
			animVertices[i].boneIndex[j] = j;
		}
		animVertices[i].boneWeight[3] = 0.000001f;
		animVertices[i].boneWeight = mm::normalize(animVertices[i].boneWeight);
	}

	// create a mesh
	graphics::IMesh::ElementDesc elements[5] = {
		graphics::IMesh::POSITION, 3,
		graphics::IMesh::NORMAL, 3,
		graphics::IMesh::TEX0, 2,
		graphics::IMesh::BONE_INDICES, 4,
		graphics::IMesh::BONE_WEIGHTS, 4,
	};
	graphics::IMesh::MeshData meshData;
	meshData.index_data = indices;
	meshData.index_num = sizeof(indices) / sizeof(uint32_t);
	meshData.mat_ids = nullptr;
	meshData.mat_ids_num = 0;
	meshData.vertex_bytes = sizeof(animVertices);
	meshData.vertex_data = animVertices;
	meshData.vertex_elements = elements;
	meshData.vertex_elements_num = sizeof(elements) / sizeof(elements[0]);

	if (!mesh->update(meshData)) {
		cout << "could not load mesh" << endl;
		return false;
	}

	// load animation
	animation.setNumKeyframes(3);
	if (!animation.setLayout(numChildren, 3)) {
		cout << "could not set layout of animation" << endl;
		return false;
	}

	for (int i = 0; i < animation.getNumBones(); i++) {
		animation.getDefaultBones()[i] = defBones[i];
	}
	for (int keyframe = 0; keyframe < animation.getNumKeyframes(); keyframe++) {
		for (int i = 0; i < animation.getNumBones(); i++) {
			animation.getBones(keyframe)[i] = kfBones[keyframe][i];
		}
		animation.setKeyframeDuration(keyframe, 0.6f);
	}
	animation.setReferenceBoneDirection({ 0,1,0 });

	return true;
}