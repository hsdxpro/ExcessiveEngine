#include "InputLayoutGL.h"
#include <cassert>


InputLayoutGL::InputLayoutGL(rInputElement* elements, size_t num_elements)
	: elements(elements, elements+num_elements)
{

}

InputLayoutGL::~InputLayoutGL() {
	/* empty */
}


size_t InputLayoutGL::getNumElements() {
	return elements.size();
}

const rInputElement& InputLayoutGL::getElement(size_t index) {
	assert(index < elements.size());
	return elements[index];
}

void InputLayoutGL::release() {
	delete this;
}

