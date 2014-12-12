#include "ShaderProgramGL.h"

#include <vector>
#include "custom_assert.h"

#define DEBUG_SHADER_ERRORS
#define INFOLOG_SIZE 32768

using namespace std;

//TODO add error checking in debug mode maybe?

//should be indexed by an eShaderType enum
GLenum shader_types[] =
{
	GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER,
	GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER
};

void ShaderProgramGL::destroy()
{
	glDeleteProgram(id);
	id = 0;
}

void ShaderProgramGL::addShader(const char* src, eShaderType type)
{
	GLuint shader_id = glCreateShader(shader_types[type]);
	glShaderSource(shader_id, 1, &src, 0);
	glCompileShader(shader_id);

#ifdef DEBUG_SHADER_ERRORS
	GLchar infolog[INFOLOG_SIZE];
	infolog[0] = '\0';
	glGetShaderInfoLog(shader_id, INFOLOG_SIZE, 0, infolog);
	cerr << infolog << endl;
#endif

	glAttachShader(id, shader_id);
	glDeleteShader(shader_id);
}

void ShaderProgramGL::link()
{
	glLinkProgram(id);

#ifdef DEBUG_SHADER_ERRORS
	GLchar infolog[INFOLOG_SIZE];
	infolog[0] = '\0';
	glGetProgramInfoLog(id, INFOLOG_SIZE, 0, infolog);
	cerr << infolog << endl;
#endif
}

size_t ShaderProgramGL::getBinary(void* data, size_t max_size)
{
	// well, it ain't gonna work out that way 
	/*
	vector<char> buf;
	GLenum format = 0;

	GLint bufsize = 0;
	glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &bufsize);

	if (data == nullptr) {
		return bufsize;
	}

	if (bufsize)
	{
		buf.resize(bufsize);

		glGetProgramBinary(id, bufsize, 0, &format, (void*)buf.data());

		//add space for the enum and the size
		size_t size = bufsize + sizeof(GLint) + sizeof(GLenum);
		data = new char[size];

		char* ptr = *data;
		reinterpret_cast<GLint*>(ptr)[0] = bufsize;
		ptr += sizeof(GLint);
		reinterpret_cast<GLenum*>(ptr)[0] = format;
		ptr += sizeof(GLenum);
		memcpy(ptr, buf.data(), bufsize);
	}
	*/
	return 0;
}

void ShaderProgramGL::loadFromBinary(void* data, size_t size)
{
	// neither will this
	/*
	ASSERT(data)
	{
		char* ptr = data;
		GLint size = reinterpret_cast<GLint*>(ptr)[0];
		ptr += sizeof(GLint);
		GLenum format = reinterpret_cast<GLenum*>(ptr)[0];
		ptr += sizeof(GLenum);

		glProgramBinary(id, format, ptr, size);
	}
	*/
}

int ShaderProgramGL::getUniformBlockIndex(const char* str)
{
	ASSERT(str);
	return glGetUniformBlockIndex(id, str);
}

int ShaderProgramGL::getAttributeIndex(const char* str)
{
	ASSERT(str);
	return glGetAttribLocation(id, str);
}

int ShaderProgramGL::getSamplerIndex(const char* str)
{
	ASSERT(str);
	int loc = glGetUniformLocation(id, str);
	int idx;
	glGetUniformiv(id, loc, &idx);
	return idx;
}

int ShaderProgramGL::getRenderTargetIndex(const char* str)
{
	ASSERT(str);
	return glGetFragDataLocation(id, str);
}