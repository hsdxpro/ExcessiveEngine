#pragma once

#include "../GraphicsApi_Interface/interface/IInputLayout.h"

#include <vector>



class InputLayoutGL : public IInputLayout
{
public:
	InputLayoutGL(rInputElement* elements, size_t num_elements);
	~InputLayoutGL();

	size_t getNumElements() override;
	const rInputElement& getElement(size_t index) override;
	void release();
private:
	std::vector<rInputElement> elements;
};