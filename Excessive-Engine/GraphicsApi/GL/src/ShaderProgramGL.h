#pragma once

#include "GL/glew.h"
#include <unordered_map>
#include "../../Interfaces/IShaderProgram.h"

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

	/// Provide program id for gapi.
	GLuint getProgramId() { return program; }


	// May only be kept for internal stuff.
	// See interface for more
	int getUniformBlockIndex(const char* str) override;
	int getAttributeIndex(const char* str) override;
	int getSamplerIndex(const char* str) override;
	int getRenderTargetIndex(const char* str) override;

protected:
	GLuint program;
	std::unordered_map<std::string, int> attrib_loc_cache;
};