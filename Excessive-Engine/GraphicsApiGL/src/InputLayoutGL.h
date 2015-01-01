#pragma once

#include "../Module_Interfaces/GraphicsApi/IInputLayout.h"
#include <vector>
#include <GL/glew.h>



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