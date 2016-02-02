#include "InputLayoutGL.h"
#include <cassert>


InputLayoutGL::InputLayoutGL(rInputElement* elements, size_t num_elements)
	: elements(elements, elements+num_elements)
{

}

InputLayoutGL::~InputLayoutGL() {
	/* empty */
}


size_t InputLayoutGL::GetNumElements() {
	return elements.size();
}

const rInputElement& InputLayoutGL::GetElement(size_t index) {
	assert(index < elements.size());
	return elements[index];
}

void InputLayoutGL::Release() {
	delete this;
}

