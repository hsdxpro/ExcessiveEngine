#pragma once

#include "../GraphicsEngine_Interface/interface/IMesh.h"


class Mesh : public ge::IMesh
{
public:
	Mesh();
	~Mesh();

	void acquire();
	void release() override;

	void load(const char* file_path); // it is supposed to return an error code
	void load(const wchar_t* file_path);
private:
	size_t refcount;
};