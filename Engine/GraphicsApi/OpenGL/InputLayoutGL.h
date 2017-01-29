#pragma once

#include "GraphicsApi/IInputLayout.h"
#include <vector>

class InputLayoutGL : public IInputLayout
{
public:
	InputLayoutGL(rInputElement* elements, size_t num_elements);
	~InputLayoutGL();

	size_t GetNumElements() override;
	const rInputElement& GetElement(size_t index) override;
	void Release();
private:
	std::vector<rInputElement> elements;
};