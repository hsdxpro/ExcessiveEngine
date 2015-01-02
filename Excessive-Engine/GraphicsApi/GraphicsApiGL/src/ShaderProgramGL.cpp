#include "ShaderProgramGL.h"

#include <vector>
#include "../Common/src/custom_assert.h"

using namespace std;


ShaderProgramGL::ShaderProgramGL(GLuint program_id) 
	: program(program_id)
{
	// get all attributes
	GLint count = 0;
	GLint attrib_name_len = 512;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count); 
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attrib_name_len);
	vector<GLchar> attrib_name(attrib_name_len+1);
	for (GLint i = 0; i < count; i++) {
		// get name
		GLint array_size = 0;
		GLenum type = 0;
		GLsizei name_len = 0;
		glGetActiveAttrib(program, i, attrib_name.size(), &name_len, &array_size, &type, attrib_name.data());
		string name(attrib_name.data(), attrib_name.data() + name_len);

		// get location
		auto location = glGetAttribLocation(program, name.c_str());

		assert(location >= 0);

		// add to cache
		attrib_loc_cache.insert(pair<string, int>(name, location));
	}
}


ShaderProgramGL::~ShaderProgramGL() 
{
	glDeleteProgram(program);
}





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
	auto index = glGetUniformBlockIndex(program, str);
	// ugly hack to get uniform blocks working...
	// delete this as soon as you can
	glUniformBlockBinding(program, index, index);
	return index;
}

int ShaderProgramGL::getAttributeIndex(const char* str)
{
	ASSERT(str);
	auto it = attrib_loc_cache.find(str);
	if (it != attrib_loc_cache.end()) {
		return it->second;
	}
	else {
		return -1;
	}
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
