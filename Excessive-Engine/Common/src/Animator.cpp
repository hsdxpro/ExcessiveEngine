#include "Animator.h"
#include "../GraphicsEngine/Interfaces/IMesh.h"
#include <algorithm>

using namespace std;


void Animator::setEntity(graphics::IEntity* entity) {
	this->entity = entity;
}
graphics::IEntity* Animator::getEntity() const {
	return entity;
}

void Animator::setAnimation(KeyframeAnimation* animation) {
	this->animation = animation;
}
KeyframeAnimation* Animator::getAnimation() const {
	return animation;
}

bool Animator::isCompatible() const {
	return entity->getMesh() && entity->getMesh()->getHighestBoneIndex() < animation->getNumBones();
}

// Set animation state
void Animator::setPlayTime(float time) {
	playTime = time;
	// TODO: do interpolation, update entities matrices

	// get keyframes to interpolate between
	size_t idxKf1 = animation->getKeyframeAt(playTime);
	size_t idxKf2 = min(animation->getNumBones() - 1, idxKf1 + 1);

	// generate interpolated keyframe
	double t1 = animation->getTimeAt(idxKf1);
	double t2 = animation->getTimeAt(idxKf2);
	float t;
	double diff = t2 - t1;
	if (diff == 0) {
		t = 0.0f;
	}
	else {
		t = (playTime - t1) / diff;
	}
	Keyframe kfInterpol = lerp((*animation)[idxKf1], (*animation)[idxKf2], t);

	// absolutify keyframe
	auto matrices = absolute(kfInterpol);

	// multiply matrix by default matrices' inverse
	auto defMatrices = absolute(animation->getDefaultState());
	for (auto& v : defMatrices) {
		v = mm::inverse(v);
	}
	assert(defMatrices.size() == matrices.size());

	// copy matrices into entity 
	for (size_t i = 0; i < entity->getNumBones(); i++) {
		entity->getBoneMatrices()[i] = matrices[i] * defMatrices[i];
	}
}

void Animator::update(float elapsed) {
	setPlayTime(playTime + playSpeed * elapsed);
}

void Animator::setPlaySpeed(float speed) {
	playSpeed = speed;
}



auto Animator::lerp(const Keyframe& kf1, const Keyframe& kf2, float t) const -> Keyframe {
	Keyframe result;
	result.setNumBones(std::min(kf1.getNumBones(), kf2.getNumBones()));
	for (size_t bone = 0; bone < result.getNumBones(); bone++) {
		// slerp the quats, lerp the others
		float u = 1 - t;
		result[bone].position = u*kf1[bone].position + t*kf2[bone].position;
		result[bone].scale = u*kf1[bone].scale + t*kf2[bone].scale;
		result[bone].rotation = mm::slerp(kf1[bone].rotation, kf2[bone].rotation, t);
		result[bone].length = u*kf1[bone].length + t*kf2[bone].length;
	}
	return result;
}

auto Animator::cubic(size_t index1, size_t index2, size_t index3, size_t index4, float t) -> Keyframe {
	// TODO: implement
	return Keyframe();
}

std::vector<mm::mat4> Animator::absolute(const Keyframe& keyframe) {
	if (keyframe.getNumBones() <= 0) {
		return std::vector<mm::mat4>();
	}
	assert(keyframe.getNumBones() == animation->getNumBones());

	std::vector<mm::mat4> matrices(keyframe.getNumBones());

	TraverseLinearTree(
		animation->getChildCounts(),
		animation->getNumBones(), 
		[&](intptr_t idxParent, intptr_t idxNode) {
			const Bone& node = keyframe[idxNode];

			// treat root nodes specially
			if (idxParent < 0) {
				// simply compute its matrix
				matrices[idxNode] = matrixify(node.position, node.rotation, node.scale);
				return;
			}

			const Bone& parent = keyframe[idxParent];
			
			// note: since a parent preceedes all its children, parent's transform must be ready
			mm::mat4& parentMat = matrices[idxParent];
			// matrixify local transform
			mm::mat4 nodeMat = matrixify(node.position, node.rotation, node.scale);
			// create parent length translation
			mm::mat4 lengthTransMat = mm::create_translation(animation->getReferenceBoneDirection()*parent.length);
			
			// multiply matrices
			matrices[idxNode] = parentMat * lengthTransMat * nodeMat;
		}
	);

	return matrices;
}