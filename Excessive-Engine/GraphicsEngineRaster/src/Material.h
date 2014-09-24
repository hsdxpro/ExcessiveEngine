#pragma once

#include "../GraphicsEngine_Interface/interface/IMaterial.h"


class Material : public ge::IMaterial
{
public:
	Material();
	~Material();

	void acquire();
	void release() override;

	void load(const char* file_path);
	void load(const wchar_t* file_path);

private:
	size_t refcount;
};