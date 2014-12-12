#pragma once

#include "mymath/mymath.h"

enum eShaderType
{
	VERTEX_SHADER = 0, TESSELLATION_CONTROL_SHADER, TESSELLATION_EVALUATION_SHADER,
	GEOMETRY_SHADER, PIXEL_SHADER, COMPUTE_SHADER
};

class IShaderProgram
{
public:
	virtual void destroy() = 0;

	///
	virtual size_t getBinary(void* data, size_t max_size) = 0;
	virtual void loadFromBinary(void* data, size_t size) = 0;

	virtual int getUniformBlockIndex(const char* str) = 0;
	virtual int getAttributeIndex(const char* str) = 0;
	virtual int getSamplerIndex(const char* str) = 0;
	virtual int getRenderTargetIndex(const char* str) = 0;
};