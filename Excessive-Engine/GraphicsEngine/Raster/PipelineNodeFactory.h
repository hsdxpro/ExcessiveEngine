#pragma once

#include "PipelineNode.h"

#include <unordered_map>
#include <functional>
#include <type_traits>


class PipelineNodeFactory {
public:
	PipelineNodeFactory() = delete;
	PipelineNodeFactory(const PipelineNodeFactory&) = delete;

	/// Register a node class for further instantiation.
	template <class T>
	static bool RegisterNodeClass(const char* className);

	/// Instantiate node class by name.
	static PipelineNode* CreateNode(const char* className);

private:
	static std::unordered_map<std::string, std::function<PipelineNode*()>> registeredClasses;
};



template <class T>
bool PipelineNodeFactory::RegisterNodeClass(const char* className) {
	static_assert(std::is_base_of<NodeBase, T>::value, "Registered class does not inherit from NodeBase.");
	auto it = registeredClasses.find(className);

	// check already registered
	if (it != registeredClasses.end()) {
		return false; 
	}

	registeredClasses.insert({ className, [] { return new T(); } });
	return true;
}



#define REGISTER_NODE_CLASS(NAME, CLASS)				\
template <int dummy = 0>								\
class NodeFactory_RegisterDummy_##CLASS {				\
	struct RegCtor {									\
		RegCtor() {										\
			if (!PipelineNodeFactory::RegisterNodeClass<CLASS>(NAME)) {	\
				"Could not register class!";					\
			}											\
		}												\
	};													\
	static RegCtor regCtor;								\
};														\
template <int dummy>									\
NodeFactory_RegisterDummy_##CLASS::RegCtor NodeFactory_RegisterDummy_##CLASS::regCtor;