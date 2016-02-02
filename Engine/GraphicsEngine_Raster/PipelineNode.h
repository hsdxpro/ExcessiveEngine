#pragma once

#include <cstddef>

class InputPortBase;
class OutputPortBase;


class PipelineNode {
public:
	virtual size_t GetNumInputs() = 0;
	virtual size_t GetNumOutputs() = 0;
	virtual InputPortBase* GetInput(size_t index) = 0;
	virtual OutputPortBase* GetOutput(size_t index) = 0;
	virtual void update() = 0;
private:

};