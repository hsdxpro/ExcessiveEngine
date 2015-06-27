#include "GapiGL.h"

#include "ShaderProgramGL.h"
#include "TextureGL.h"
#include "BufferGL.h"
#include "UniformBufferGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "InputLayoutGL.h"

#include <iostream>
#include <functional>
#include <GL/glew.h>
#include <SupportLibrary/custom_assert.h>
#include <SupportLibrary/VisualCpuProfiler.h>

using namespace std;

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C"
EXPORT IGapi* CreateGraphicsApi()
{
	return new GapiGL();
}


GapiGL::GapiGL() {
	GLenum glew_error = glewInit();

	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR);

	if (glew_error != GLEW_OK)
	{
		cerr << "Error initializing GLEW: " << glewGetErrorString(glew_error) << endl;
	}

	if (!GLEW_VERSION_4_5)
	{
		cerr << "Error: GL 4.5 is required" << endl;
	}

	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	ResetStatesToDefault();

	// Set initial bindings
	active_shader = nullptr;
	active_layout = nullptr;
}

GLenum func_data[] =
{
	GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS
};

GLenum stencil_op_data[] =
{
	GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT
};

GLenum blend_eq_data[] =
{
	GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX
};

GLenum blend_func_data[] =
{
	GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA
};

GLenum raster_mode_data[] =
{
	GL_POINT, GL_LINE, GL_FILL
};

GLenum raster_face_data[] =
{
	GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
};

GLenum raster_order_data[] =
{
	GL_CW, GL_CCW
};

/* DEPRECATED
IShaderProgram* GapiGL::CreateShaderProgram(const rShaderPaths& data)
{
// TODO
ShaderProgramGL* sp = new ShaderProgramGL();
return sp;
}

IShaderProgram* GapiGL::CreateShaderProgram(const rShaderSources& data)
{
ShaderProgramGL* sp = new ShaderProgramGL();
sp->id = glCreateProgram();
if (data.vsSrc != 0)				sp->AddShader(data.vsSrc, VERTEX_SHADER);
if (data.psSrc != 0)				sp->AddShader(data.psSrc, PIXEL_SHADER);
if (data.tessCtrlSrc != 0)			sp->AddShader(data.tessCtrlSrc, TESSELLATION_CONTROL_SHADER);
if (data.tessEvaluationSrc != 0)	sp->AddShader(data.tessEvaluationSrc, TESSELLATION_EVALUATION_SHADER);
if (data.gsSrc != 0)				sp->AddShader(data.gsSrc, GEOMETRY_SHADER);
sp->link();
return sp;
}
*/



ShaderProgramGL* GapiGL::CreateShaderSource(
	const char* vertex_shader_source,
	const char* pixel_shader_source,
	const char* geometry_shader_source,
	const char* tess_control_shader_source,
	const char* tess_eval_shader_source)
{
	GLuint program_id;
	GLuint vs = 0, ps = 0, tcs = 0, tes = 0, gs = 0;
	bool success = false;

	// if success if Set to false, frees everything when function returns
	struct CleanupT {
		std::function<void()> func;
		~CleanupT() {
			func();
		};
	} cleanup{[&]{if (!success) { glDeleteProgram(program_id); }}};

	// Create program
	program_id = glCreateProgram();
	if (program_id == 0) {
		return nullptr;
	}

	// Create, compile, and Add shaders one-by-one

	// vertex shader
	if (vertex_shader_source) {
		GLuint id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(id, 1, &vertex_shader_source, 0);
		glCompileShader(id);
		GLint result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			// obtain error message and display it
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, infoLog.data());
			cerr << infoLog.data();

			return nullptr;
		}
		glAttachShader(program_id, id);
		glDeleteShader(id);
	}

	// pixel shader
	if (pixel_shader_source) {
		GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(id, 1, &pixel_shader_source, 0);
		glCompileShader(id);
		GLint result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			// obtain error message and display it
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, infoLog.data());
			cerr << infoLog.data();

			return nullptr;
		}
		glAttachShader(program_id, id);
		glDeleteShader(id);
	}

	// geometry shader
	if (geometry_shader_source) {
		GLuint id = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(id, 1, &geometry_shader_source, 0);
		glCompileShader(id);
		GLint result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			// obtain error message and display it
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, infoLog.data());
			cerr << infoLog.data();

			return nullptr;
		}
		glAttachShader(program_id, id);
		glDeleteShader(id);
	}

	// tesselation control shader
	if (tess_control_shader_source) {
		GLuint id = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(id, 1, &tess_control_shader_source, 0);
		glCompileShader(id);
		GLint result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			// obtain error message and display it
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, infoLog.data());
			cerr << infoLog.data();

			return nullptr;
		}
		glAttachShader(program_id, id);
		glDeleteShader(id);
	}

	// tesselation evaluation shader
	if (tess_eval_shader_source) {
		GLuint id = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(id, 1, &tess_eval_shader_source, 0);
		glCompileShader(id);
		GLint result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			// obtain error message and display it
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, infoLog.data());
			cerr << infoLog.data();

			return nullptr;
		}
		glAttachShader(program_id, id);
		glDeleteShader(id);
	}

	// check if the program is linked successfully
	glLinkProgram(program_id);
	GLint is_linked = 0;
	glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);
	if (!is_linked) {
		glDeleteProgram(program_id);
		return nullptr;
	}

	// finally, Create the shader program
	// hope that fucker can allocate...
	success = true;
	ShaderProgramGL* shader_program = new ShaderProgramGL(program_id);
	return shader_program;
}


ShaderProgramGL* GapiGL::CreateShaderBinary(void* data, size_t size)
{
	// Create program
	auto program_id = glCreateProgram();

	ASSERT(data)
	{
		char* ptr = (char*)data;
		GLint size = reinterpret_cast<GLint*>(ptr)[0];
		ptr += sizeof(GLint);
		GLenum format = reinterpret_cast<GLenum*>(ptr)[0];
		ptr += sizeof(GLenum);

		glProgramBinary(program_id, format, ptr, size);
	}

	GLint is_linked = 0;
	glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);
	if (!is_linked) {
		return nullptr;
	}
	else {
		return new ShaderProgramGL(program_id);
	}
}



UniformBufferGL* GapiGL::CreateUniformBuffer(const rBuffer& data)
{
	UniformBufferGL* ubo = new UniformBufferGL();
	glGenBuffers(1, &ubo->id);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo->id);
	/*glNamedBufferStorage( ubo->id, data->size, 0, GL_DYNAMIC_STORAGE_BIT |
	(data->is_readable ? GL_MAP_READ_BIT : 0) |
	(data->is_writable ? GL_MAP_WRITE_BIT : 0) |
	(data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
	(data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );*/

	/*glBufferStorage( GL_UNIFORM_BUFFER, data->size, 0, GL_DYNAMIC_STORAGE_BIT |
	(data->is_readable ? GL_MAP_READ_BIT : 0) |
	(data->is_writable ? GL_MAP_WRITE_BIT : 0) |
	(data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
	(data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );*/

	glBufferData(GL_UNIFORM_BUFFER, data.size, 0, GL_DYNAMIC_DRAW);

	ubo->adata = data;
	return ubo;
}

VertexBufferGL* GapiGL::CreateVertexBuffer(const rBuffer& data)
{
	VertexBufferGL* vbo = new VertexBufferGL();
	glGenBuffers(1, &vbo->id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo->id); //TODO not sure if we need this
	/*glNamedBufferStorage( vbo->id, data->size, 0, GL_DYNAMIC_STORAGE_BIT |
	(data->is_readable ? GL_MAP_READ_BIT : 0) |
	(data->is_writable ? GL_MAP_WRITE_BIT : 0) |
	(data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
	(data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );*/

	/*glBufferStorage( GL_ARRAY_BUFFER, data->size, 0, GL_DYNAMIC_STORAGE_BIT |
	(data->is_readable ? GL_MAP_READ_BIT : 0) |
	(data->is_writable ? GL_MAP_WRITE_BIT : 0) |
	(data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
	(data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );*/

	glBufferData(GL_ARRAY_BUFFER, data.size, 0, GL_DYNAMIC_DRAW);

	vbo->adata = data;
	return vbo;
}

TextureGL* GapiGL::CreateTexture(const rTextureGapi& data)
{
	TextureGL* tex = new TextureGL();
	glGenTextures(1, &tex->ID);
	tex->target = 0;

	if (data.height > 1)
	{
		if (data.depth > 1)
		{
			//3D
			tex->dim = 3;

			if (data.is_layered)
			{
				if (data.is_cubemap)
				{
					tex->target = GL_TEXTURE_CUBE_MAP_ARRAY;
				}
				else
				{
					tex->target = GL_TEXTURE_2D_ARRAY;
				}
			}
			else
			{
				tex->target = GL_TEXTURE_3D;
			}

			glBindTexture(tex->target, tex->ID);
			glTexStorage3D(tex->target, data.num_levels, texture_internal_formats[(int)data.format], data.width, data.height, data.depth);
		}
		else
		{
			//2D
			tex->dim = 2;

			if (data.is_layered)
			{
				tex->target = GL_TEXTURE_1D_ARRAY;
			}
			else if (data.is_cubemap)
			{
				tex->target = GL_TEXTURE_CUBE_MAP;
			}
			else
			{
				tex->target = GL_TEXTURE_2D;
			}

			glBindTexture(tex->target, tex->ID);
			glTexStorage2D(tex->target, data.num_levels, texture_internal_formats[(int)data.format], data.width, data.height);
		}
	}
	else
	{
		//1D
		tex->dim = 1;

		tex->target = GL_TEXTURE_1D;

		glBindTexture(tex->target, tex->ID);
		glTexStorage1D(tex->target, data.num_levels, texture_internal_formats[(int)data.format], data.width);
	}

	tex->desc = data;

	// Create view for texture
	glGenTextures(1, &tex->viewID);

	glTextureView(tex->viewID,
		tex->target,
		tex->ID,
		texture_internal_formats[(int)data.format],
		0,
		data.num_levels,
		0,
		1);

	return tex;
}

IndexBufferGL* GapiGL::CreateIndexBuffer(const rBuffer& data)
{
	IndexBufferGL* ibo = new IndexBufferGL();
	glGenBuffers(1, &ibo->id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
	/*glNamedBufferStorage( ibo->id, data->size, 0, GL_DYNAMIC_STORAGE_BIT |
	(data->is_readable ? GL_MAP_READ_BIT : 0) |
	(data->is_writable ? GL_MAP_WRITE_BIT : 0) |
	(data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
	(data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );*/

	/*glBufferStorage( GL_ELEMENT_ARRAY_BUFFER, data->size, 0, GL_DYNAMIC_STORAGE_BIT |
	(data->is_readable ? GL_MAP_READ_BIT : 0) |
	(data->is_writable ? GL_MAP_WRITE_BIT : 0) |
	(data->is_persistent ? GL_MAP_PERSISTENT_BIT : 0) |
	(data->prefer_cpu_storage ? GL_CLIENT_STORAGE_BIT : 0) );*/

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size, 0, GL_DYNAMIC_DRAW);

	ibo->adata = data;
	return ibo;
}



// textures
void GapiGL::WriteTexture(ITextureGapi* t, const rTextureUpdate& d) {
	TextureGL* tex = (TextureGL*)t;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(tex->target, tex->ID);

	if (tex->dim == 1)
	{
		//glTextureSubImage1D(tex->ID, d.level, d.x_offset, d.width, texture_formats[d.format], texture_types[d.format], d.data);
		glTexSubImage1D(tex->target, d.level, d.x_offset, d.width, texture_formats[(int)d.format], texture_types[(int)d.format], d.data);
	}
	else if (tex->dim == 2)
	{
		//glTextureSubImage2D( id, data->level, data->x_offset, data->y_offset, data->width, data->height, texture_formats[data->format], texture_types[data->format], data->data );
		glTexSubImage2D(tex->target, d.level, d.x_offset, d.y_offset, d.width, d.height, texture_formats[(int)d.format], texture_types[(int)d.format], d.data);

	}
	else //threesome
	{
		glTextureSubImage3D(tex->ID, d.level, d.x_offset, d.y_offset, d.z_offset, d.width, d.height, d.depth, texture_formats[(int)d.format], texture_types[(int)d.format], d.data);
		//glTextureSubImage3D(tex->ID, d.level, d.x_offset, d.y_offset, d.z_offset, d.width, d.height, d.depth, texture_formats[d.format], texture_types[d.format], d.data);
	}
}
void GapiGL::writeTexture(ITextureGapi* t, const rTextureUpdate& d) {
	auto id = ((TextureGL*)t)->ID;
	glGetTextureSubImage(id, d.level, d.x_offset, d.y_offset, d.z_offset, d.width, d.height, d.depth, texture_formats[(int)d.format], texture_types[(int)d.format], ((d.width - d.x_offset) * (d.height - d.y_offset) * (d.depth - d.z_offset) * texture_sizes[(int)d.format]) / 8, d.data);
}

// vertex buffers
void GapiGL::WriteBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) {
	auto id = ((VertexBufferGL*)buffer)->id;
	/*void* ptr = glMapBufferRange( GL_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT );
	memcpy( ptr, data, size );
	glUnmapBuffer( GL_ARRAY_BUFFER );*/
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
void GapiGL::ReadBuffer(IVertexBuffer* buffer, void* data, size_t size, size_t offset) {
	// TODO 4.5
  auto id = ((VertexBufferGL*)buffer)->id;
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferSubData( GL_ARRAY_BUFFER, offset, size, data );
}

// index buffers
void GapiGL::WriteBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) {
	auto id = ((IndexBufferGL*)buffer)->id;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}
void GapiGL::ReadBuffer(IIndexBuffer* buffer, void* data, size_t size, size_t offset) {
	// TODO 4.5
  auto id = ((IndexBufferGL*)buffer)->id;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, offset, size, data );
}

// uniform buffers
void GapiGL::WriteBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) {
	auto id = ((UniformBufferGL*)buffer)->id;
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
void GapiGL::ReadBuffer(IUniformBuffer* buffer, void* data, size_t size, size_t offset) {
	// TODO 4.5
  auto id = ((UniformBufferGL*)buffer)->id;
  glBindBuffer(GL_UNIFORM_BUFFER, id);
  glBufferSubData( GL_UNIFORM_BUFFER, offset, size, data );
}

void GapiGL::ResetStatesToDefault()
{
	//use a single global vao
	glGenVertexArrays(1, &global_vao);
	glBindVertexArray(global_vao);

	rBlendState d;
		d.enable = false;
	SetBlendState(d);
}

void GapiGL::SetDepthState(const rDepthState& state)
{
	if (state.enable_test)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	glDepthMask(state.enable_write);

	glDepthRangef(state.near, state.far);

	glDepthFunc(func_data[(u32)state.func]);
}

void GapiGL::SetStencilState(const rStencilState& state)
{
	if (state.enable_test)
	{
		glEnable(GL_STENCIL_TEST);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}

	glDepthMask(state.enable_write);

	glStencilMask(state.mask);

	glStencilFunc(func_data[(u32)state.func], state.reference, state.func_mask);

	glStencilOp(stencil_op_data[(u32)state.on_stencil_fail], stencil_op_data[(u32)state.on_stencil_pass_depth_fail], stencil_op_data[(u32)state.on_stencil_pass_depth_pass]);
}

void GapiGL::SetBlendState(const rBlendState& state)
{
	if (state.enable)
	{
		glEnable(GL_BLEND);

		glBlendColor(state.blend_color.x, state.blend_color.y, state.blend_color.z, state.blend_color.w);
		glBlendEquation(blend_eq_data[(u32)state.equation]);
		glBlendFunc(blend_func_data[(u32)state.src_func], blend_func_data[(u32)state.dst_func]);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}

void GapiGL::SetSamplerState(const char* slotName, const rSamplerState& smpdata, ITextureGapi* t)
{
	TextureGL* tex = (TextureGL*)t;

	if (smpdata.is_anisotropic)
	{
		glTextureParameterf(tex->ID, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
	}

	if (smpdata.is_clamped)
	{
		// TODO 4.5
		//glTextureParameteri( id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		//glTextureParameteri( id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		//glTextureParameteri( id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

		glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(tex->target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	else
	{
		// TODO 4.5
		//glTextureParameteri( id, GL_TEXTURE_WRAP_S, GL_REPEAT );
		//glTextureParameteri( id, GL_TEXTURE_WRAP_T, GL_REPEAT );
		//glTextureParameteri( id, GL_TEXTURE_WRAP_R, GL_REPEAT );

		glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(tex->target, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}

	if (smpdata.is_bilinear)
	{
		if (smpdata.is_mipmapped)
		{
			// TODO 4.5
			//glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		// TODO 4.5
		//glTextureParameteri( id, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		if (smpdata.is_mipmapped)
		{
			// TODO 4.5
			//glTextureParameteri( id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
			glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
		// TODO 4.5
		//glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void GapiGL::SetSRGBWrites(bool val)
{
	if (val)
	{
		glEnable(GL_FRAMEBUFFER_SRGB);
	}
	else
	{
		glDisable(GL_FRAMEBUFFER_SRGB);
	}
}

void GapiGL::SetSeamlessCubeMaps(bool val)
{
	if (val)
	{
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
	else
	{
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
}

void GapiGL::SetViewport(int x, int y, u32 w, u32 h)
{
	glViewport(x, y, w, h);
}

void GapiGL::SetRasterizationState(const rRasterizerState& state)
{
	glPolygonMode(GL_FRONT_AND_BACK, raster_mode_data[(u32)state.mode]);
	glFrontFace(raster_order_data[(u32)state.vertex_order]);
	glCullFace(raster_face_data[(u32)state.face]);
	glColorMask(state.r_mask, state.g_mask, state.b_mask, state.a_mask);
}

bool GapiGL::GetError() //true if error
{
	return glGetError != GL_NO_ERROR;
}

void GapiGL::SetDebugOutput(bool val)
{
	if (val)
	{
		glEnable(GL_DEBUG_OUTPUT);
	}
	else
	{
		glDisable(GL_DEBUG_OUTPUT);
	}
}

void GapiGL::SetSyncDebugOutput(bool val)
{
	if (val)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
	else
	{
		glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
}

void GapiGL::SetShaderProgram(IShaderProgram* sp)
{
	ASSERT(sp);
	ShaderProgramGL* shader_program = static_cast<ShaderProgramGL*>(sp);
	glUseProgram(shader_program->GetProgramId());

	active_shader = shader_program;
	// Set dirty flag
	is_layout_bound = false;
	//BindInputLayout();
}

void GapiGL::SetTexture(ITextureGapi* t, u32 idx)
{
	TextureGL* tex = (TextureGL*)t;

	// Activate slot
	glActiveTexture(GL_TEXTURE0 + idx);

	// Bind texture
	glBindTexture(tex->target, tex->viewID);
}

void GapiGL::SetRenderTargets(const rRenderTargetInfo* render_targets, u32 size)
{
	//TODO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GapiGL::SetUniformBuffer(IUniformBuffer* buf, u32 index)
{
	ASSERT(buf);
	auto buffer_id = static_cast<UniformBufferGL*>(buf)->id;
	glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer_id);
}

GLenum attrib_array[] =
{
	GL_FLOAT, GL_INT, GL_UNSIGNED_INT
};


// DEPRECATED API
void GapiGL::SetVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers)
{
	ASSERT(buffers && attrib_data);


	for (u32 c = 0; c < num_buffers; ++c)
	{
		u32 type;
		bool normalize = false;
		if (attrib_data[c].type == eVertexAttribType::FLOAT) {
			type = GL_FLOAT;
		}
		else if (attrib_data[c].type == eVertexAttribType::UNORM_16) {
			type = GL_UNSIGNED_SHORT;
			normalize = true;
		}
		else {
			cout << "Type cannot be used..." << endl;
			continue;
		}


		GLuint id = static_cast<const VertexBufferGL*>(buffers[c])->id;
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glEnableVertexAttribArray(attrib_data[c].index);
		glVertexAttribPointer(attrib_data[c].index, attrib_data[c].nComponent, type, normalize, attrib_data[c].size, (const void*)attrib_data[c].offset);
		//glVertexAttribDivisor( attrib_data[c].index, attrib_data[c].divisor ); //instancing stuff
	}
}

void GapiGL::SetIndexBuffer(IIndexBuffer* ibo)
{
	ASSERT(ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<IndexBufferGL*>(ibo)->id);
}

void GapiGL::DrawIndexed(u32 num_indices, u32 index_byte_offset /*= 0*/)
{
#ifdef DEBUG_SHADER_ERRORS
	glValidateProgram(static_cast<ShaderProgram*>(s)->id);
	GLchar infolog[INFOLOG_SIZE];
	infolog[0] = '\0';
	glGetProgramInfoLog(static_cast<ShaderProgram*>(s)->id, INFOLOG_SIZE, 0, infolog);
	cerr << infolog << endl;
#endif
	if (!is_layout_bound) {
		BindInputLayout();
	}

	PROFILE_SCOPE_SUM("glDrawElements");
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void*)index_byte_offset);
}

void GapiGL::Draw(u32 nVertices, u32 offset /*= 0*/)
{
	glDrawArrays(GL_TRIANGLES, offset, nVertices);
}

void GapiGL::ClearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth /*= 0*/, i32 stencil /*= 0*/)
{

	unsigned int clearFlag = 0;
	u32 bits = (u32)f;

	if ((bits & (u32)eClearFlag::COLOR) != 0)
	{
		clearFlag |= GL_COLOR_BUFFER_BIT;
		glClearColor(color.r, color.g, color.b, color.a);
	}

	if ((bits & (u32)eClearFlag::DEPTH) != 0)
	{
		clearFlag |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}

	if ((bits & (u32)eClearFlag::DEPTH) != 0)
	{
		clearFlag |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(stencil);
	}

	glClear(clearFlag);
}



////////////////////////////////////////////////////////////////////////////////
// input layout & vertex streams


// convert attrib type to native
static inline GLenum NativeAttribType(eVertexAttribType type) {
	static const GLenum lut[] = {
		GL_FLOAT,
		GL_HALF_FLOAT,
		GL_INT,
		GL_UNSIGNED_INT,
		GL_SHORT,
		GL_UNSIGNED_SHORT,
		GL_BYTE,
		GL_UNSIGNED_BYTE,
		GL_INT,
		GL_UNSIGNED_INT,
		GL_SHORT,
		GL_UNSIGNED_SHORT,
		GL_BYTE,
		GL_UNSIGNED_BYTE,
	};
	assert((unsigned)type < sizeof(lut) / sizeof(lut[0]));
	return lut[(unsigned)type];
}
static inline GLboolean IsNormalizedType(eVertexAttribType type) {
	return
		type == eVertexAttribType::SNORM_32 ||
		type == eVertexAttribType::SNORM_16 ||
		type == eVertexAttribType::SNORM_8 ||
		type == eVertexAttribType::UNORM_32 ||
		type == eVertexAttribType::UNORM_16 ||
		type == eVertexAttribType::UNORM_8;
}


// Create an input layout
InputLayoutGL* GapiGL::CreateInputLayout(rInputElement* elements, size_t num_elements) {
	// validate here
	for (size_t i = 0; i < num_elements; i++) {
		if (elements[i].num_components == 0 || elements[i].num_components > 4) {
			return nullptr;
		}
	}
	// Create and return a new layout
	InputLayoutGL* layout = new InputLayoutGL(elements, num_elements);
	return layout;
}


// Set an input layout
void GapiGL::SetInputLayout(IInputLayout* layout) {
	active_layout = (InputLayoutGL*)layout;

	// Set dirty flag
	is_layout_bound = false;
	//BindInputLayout();
}

void GapiGL::BindInputLayout() {
	if (!active_layout || !active_shader) {
		return;
	}

	// Set each element (== attribute)
	for (size_t i = 0; i < active_layout->GetNumElements(); i++) {
		auto& element = active_layout->GetElement(i);

		// Get attribute location
		int location = active_shader->GetAttributeIndex(element.name);

		GLenum result;

		// Set attribute
		glVertexAttribFormat(
			location,
			element.num_components,
			NativeAttribType(element.type),
			IsNormalizedType(element.type),
			element.offset
			);
		result = glGetError();

		// Set bindig point
		glVertexAttribBinding(location, element.stream_index);
		result = glGetError();
		// enable this format
		glEnableVertexAttribArray(location);
	}

	// Set dirty flag
	is_layout_bound = true;
}

void GapiGL::SetVertexBuffers(
	const IVertexBuffer* const * buffers,
	u32* strides,
	u32* offsets,
	u32 start_slot,
	u32 num_buffers)
{
	// masse unbindig
	if (buffers == nullptr) {
		glBindVertexBuffers(start_slot, num_buffers, nullptr, nullptr, nullptr);
	}

	// bind buffers one by one
	// TODO: replace by multibind glBindVertexBuffer[s]
	for (size_t i = 0; i < num_buffers; i++) {
		u32 slot = start_slot + i;

		// clear zero buffers
		if (buffers[i] == nullptr) {
			glBindVertexBuffer(slot, 0, 0, 0);
				continue;
		}

		// bind buffers
		glBindVertexBuffer(
			slot,
			((VertexBufferGL*)buffers[i])->id,
			offsets[i],
			strides[i]);
	}
}


u32 GapiGL::GetNumVertexBufferSlots() const {
	return 16;
}
