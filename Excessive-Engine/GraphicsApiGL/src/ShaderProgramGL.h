#pragma once

#include "GL/glew.h"

#include "IShaderProgram.h"

//#define DEBUG_SHADER_ERRORS

#ifdef DEBUG_SHADER_ERRORS
#define INFOLOG_SIZE 4096
#endif

/// Implementation of the IShaderProgram interface.
/// \see IShaderProgram.
class ShaderProgramGL : public IShaderProgram
{
public:
	ShaderProgramGL(GLuint program_id);
	~ShaderProgramGL();

	void destroy() override;

	bool compile(
		const char* vertex_shader,
		const char* pixel_shader,
		const char* geometry_shader = nullptr,
		const char* tesselation_control_shader = nullptr,
		const char* tesselation_evaluation_shader = nullptr) override;

	size_t getBinary(void* data, size_t max_size) override;
	bool loadBinary(void* data, size_t size) override;

	/// Provide program id for gapi.
	GLuint getProgramId() { return program; }


	// May only be kept for internal stuff.
	// See interface for more
	int getUniformBlockIndex(const char* str);
	int getAttributeIndex(const char* str);
	int getSamplerIndex(const char* str);
	int getRenderTargetIndex(const char* str);

protected:
	GLuint program;
};