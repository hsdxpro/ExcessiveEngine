#pragma once

#include "GL/glew.h"

#include "IShaderProgram.h"

//#define DEBUG_SHADER_ERRORS

#ifdef DEBUG_SHADER_ERRORS
#define INFOLOG_SIZE 4096
#endif

class ShaderProgramGL : public IShaderProgram
{
public:
	GLuint id;

	void destroy() override;
	void addShader(const char* src, eShaderType type);
	void link();
	size_t getBinary(void* data, size_t max_size) override;
	void loadFromBinary(void* data, size_t size) override;

	int getUniformBlockIndex(const char* str);
	int getAttributeIndex(const char* str);
	int getSamplerIndex(const char* str);
	int getRenderTargetIndex(const char* str);
};