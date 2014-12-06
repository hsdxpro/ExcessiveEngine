#pragma once

#include "../GraphicsApi_Interface/interface/IInputLayout.h"




class InputLayout : public IInputLayout
{
public:
	size_t getNumComponents();
	InputElement getElement(size_t index);
	void release();

	~InputLayout();
private:

};