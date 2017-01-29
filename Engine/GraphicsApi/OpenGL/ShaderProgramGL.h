#pragma once

#include "GL/glew.h"
#include <unordered_map>
#include "GraphicsApi/IShaderProgram.h"

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

	void Release() override;

	/// Provide program id for gapi.
	GLuint GetProgramId() { return program; }


	// May only be kept for internal stuff.
	// See interface for more
	int GetUniformBlockIndex(const char* str) override;
	int GetAttributeIndex(const char* str) override;
	int GetSamplerIndex(const char* str) override;
	int GetRenderTargetIndex(const char* str) override;

protected:
	GLuint program;
	std::unordered_map<std::string, int> attrib_loc_cache;
};