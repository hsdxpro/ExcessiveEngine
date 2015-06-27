#pragma once
#include "..\ILight.h"

class Light : public graphics::ILight
{
public:
	void SetColor() override;
	void SetPosition() override;
	void SetDirection() override;
};