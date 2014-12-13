#include "ShaderProgramGL.h"

#include <vector>
#include "custom_assert.h"

using namespace std;


// Remove this unless necessary.
//should be indexed by an eShaderType enum
GLenum shader_types[] =
{
	GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER,
	GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER
};


void ShaderProgramGL::destroy()
{
	delete this;
}

/*
// TODO: I could not get it working with the new signature (Peti)
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
	//* /
	return 0;
}
//*/

/*
// TODO: I could not get it working with the new signature (Peti)
bool ShaderProgramGL::loadBinary(void* data, size_t size)
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
	//* /
}
//*/


////////////////////////////////////////////////////////////////////////////////
// REVIEW CODE BELOW


// May only be kept for internal stuff.
// Depracated API
int ShaderProgramGL::getUniformBlockIndex(const char* str)
{
	ASSERT(str);
	return glGetUniformBlockIndex(program, str);
}

int ShaderProgramGL::getAttributeIndex(const char* str)
{
	ASSERT(str);
	return glGetAttribLocation(program, str);
}

int ShaderProgramGL::getSamplerIndex(const char* str)
{
	ASSERT(str);
	int loc = glGetUniformLocation(program, str);
	int idx;
	glGetUniformiv(program, loc, &idx);
	return idx;
}

int ShaderProgramGL::getRenderTargetIndex(const char* str)
{
	ASSERT(str);
	return glGetFragDataLocation(program, str);
}


// Deprecated API
/*
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
*/


// Deprecated API
/*
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
*/