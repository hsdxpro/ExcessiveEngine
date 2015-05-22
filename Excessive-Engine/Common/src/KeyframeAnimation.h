#pragma once

#include <mymath/mymath.h>
#include <vector>
#include <stack>


class KeyframeAnimation {
public:
	////////////////////////////////////
	// Helper stuff

	// Bone structure
	struct Bone {
		mm::vec3 position;
		mm::quat rotation;
		mm::vec3 scale;
		float length;
	};

	// Class for storing an entire keyframe
	class Keyframe {
	public:
		void setNumBones(size_t n);
		size_t getNumBones() const;

		Bone& operator[](size_t i);
		const Bone& operator[](size_t i) const;

		void setDuration(float duration);
		float getDuration() const;

		Bone* data();
		const Bone* data() const;
	private:
		std::vector<Bone> bones;
		float duration = 0.0;
	};


	////////////////////////////////////
	// Animation

	KeyframeAnimation();

	// Modify KeyframeAnimation
	bool setLayout(size_t* childCounts, size_t numBones);
	const size_t* getChildCounts() const;
	size_t getNumBones() const;

	void setNumKeyframes(size_t n);
	size_t getNumKeyframes() const;

	void setReferenceBoneDirection(mm::vec3 v) { refBoneDir = mm::normalize(v); }
	mm::vec3 getReferenceBoneDirection() const { return refBoneDir;	}


	// Modify keyframes
	const Keyframe& getDefaultState() const;
	Bone* getDefaultBones();

	const Keyframe& operator[](size_t i) const;

	Bone* getBones(size_t keyframe);
	void setKeyframeDuration(size_t keyframe, float duration);
	float getKeyframeDuration(size_t keyframe);

	// Helper stuff
	size_t getKeyframeAt(float timePoint);
	float getTimeAt(size_t frameIndex) { return keyframeTimes[frameIndex]; }
	
private:
	void initBoneCount();
	void recalcKeyframeTimes();

	std::vector<Keyframe> keyframes;
	std::vector<size_t> layout;
	std::vector<float> keyframeTimes;
	bool keyframeTimesDirty;
	Keyframe defaultBones;

	mm::vec3 refBoneDir;
};




// Function to traverse the nodes of a tree that's organizes linearly
template <class Func>
bool TraverseLinearTree(const size_t* childCounts, size_t numNodes, Func operation) {
	if (numNodes == 0) {
		return true;
	}
	struct StackItem {
		intptr_t parent;
		intptr_t childRemaining;
	};
	std::stack<StackItem> nodeStack;
	nodeStack.push({ -1, std::numeric_limits<intptr_t>::max() }); // root node
	for (size_t i = 0; i < numNodes; i++) {
		// call operation for this and its parent
		operation(nodeStack.top().parent, i);

		// reduce the remaining count of parent
		nodeStack.top().childRemaining--;

		// push this to the stack
		nodeStack.push({ i, childCounts[i] });

		// pop until childRemaining is zero
		while (nodeStack.top().childRemaining == 0) {
			nodeStack.pop();
		}
	}

	// the stack must only contain the root element, otherwise the traversal was unsuccesful
	return nodeStack.size() == 1;
}