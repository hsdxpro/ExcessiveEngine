#include "PipelineNodeFactory.h"


PipelineNode* PipelineNodeFactory::CreateNode(const char* className) {
	auto it = registeredClasses.find(className);
	if (it != registeredClasses.end()) {
		return it->second();
	}
	else {
		return nullptr;
	}
}
