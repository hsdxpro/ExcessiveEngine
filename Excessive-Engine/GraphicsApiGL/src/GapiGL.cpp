#include "GapiGL.h"

#include "custom_assert.h"

#include "ShaderProgramGL.h"
#include "TextureGL.h"
#include "BufferGL.h"
#include "UniformBufferGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"

#include <iostream>
#include <functional>
#include "SFML\Graphics\Image.hpp"
#include "GL\glew.h"

using namespace std;

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C"
EXPORT IGapi* createGraphicsApi()
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

	//use a single global vao
	glGenVertexArrays(1, &global_vao);
	glBindVertexArray(global_vao);
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
IShaderProgram* GapiGL::createShaderProgram(const rShaderPaths& data)
{
	// TODO
	ShaderProgramGL* sp = new ShaderProgramGL();
	return sp;
}

IShaderProgram* GapiGL::createShaderProgram(const rShaderSources& data)
{
	ShaderProgramGL* sp = new ShaderProgramGL();
	sp->id = glCreateProgram();
	if (data.vsSrc != 0)				sp->addShader(data.vsSrc, VERTEX_SHADER);
	if (data.psSrc != 0)				sp->addShader(data.psSrc, PIXEL_SHADER);
	if (data.tessCtrlSrc != 0)			sp->addShader(data.tessCtrlSrc, TESSELLATION_CONTROL_SHADER);
	if (data.tessEvaluationSrc != 0)	sp->addShader(data.tessEvaluationSrc, TESSELLATION_EVALUATION_SHADER);
	if (data.gsSrc != 0)				sp->addShader(data.gsSrc, GEOMETRY_SHADER);
	sp->link();
	return sp;
}
*/



ShaderProgramGL* GapiGL::createShaderSource(
	const char* vertex_shader_source,
	const char* pixel_shader_source,
	const char* geometry_shader_source,
	const char* tess_control_shader_source,
	const char* tess_eval_shader_source)
{
	GLuint program_id;
	GLuint vs=0, ps=0, tcs=0, tes=0, gs=0;
	bool success = false;

	// if success if set to false, frees everything when function returns
	struct CleanupT {
		std::function<void()> func;
		~CleanupT() {
			func();
		};
	} cleanup{[&]{if (!success) { glDeleteProgram(program_id); }}};

	// create program
	program_id = glCreateProgram();
	if (program_id == 0) {
		return nullptr;
	}

	// create, compile, and add shaders one-by-one

	// vertex shader
	if (vertex_shader_source) {
		GLuint id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(id, 1, &vertex_shader_source, 0);
		glCompileShader(id);
		GLint result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			// obtain error message and display it
			/* ... */
			
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
			/* ... */

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
			/* ... */

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
			/* ... */

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
			/* ... */

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

	// finally, create the shader program
	// hope that fucker can allocate...
	success = true;
	ShaderProgramGL* shader_program = new ShaderProgramGL(program_id);
	return shader_program;
}

ShaderProgramGL* GapiGL::createShaderFile(
	const wchar_t* vertex_shader_path,
	const wchar_t* pixel_shader_path,
	const wchar_t* geometry_shader_path,
	const wchar_t* tess_control_shader_path,
	const wchar_t* tess_eval_shader_path)
{
	return nullptr;
}

ShaderProgramGL* GapiGL::createShaderBinary(void* data, size_t size)
{
	return nullptr;
}



UniformBufferGL* GapiGL::createUniformBuffer(const rBuffer& data)
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

VertexBufferGL* GapiGL::createVertexBuffer(const rBuffer& data)
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

TextureGL* GapiGL::createTexture(const rTexture& data)
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
			glTexStorage3D(tex->target, data.num_levels, texture_internal_formats[data.format], data.width, data.height, data.depth);
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
			glTexStorage2D(tex->target, data.num_levels, texture_internal_formats[data.format], data.width, data.height);
		}
	}
	else
	{
		//1D
		tex->dim = 1;

		tex->target = GL_TEXTURE_1D;

		glBindTexture(tex->target, tex->ID);
		glTexStorage1D(tex->target, data.num_levels, texture_internal_formats[data.format], data.width);
	}

	tex->desc = data;

	// Create view for texture
	glGenTextures(1, &tex->viewID);

	glTextureView(tex->viewID,
		tex->target,
		tex->ID,
		texture_internal_formats[data.format],
		0,
		data.num_levels,
		0,
		1);

	return tex;
}

TextureGL* GapiGL::createTexture(const char* path)
{
	sf::Image im;
	im.loadFromFile(path);

	rTexture texdata;
	texdata.width = im.getSize().x;
	texdata.height = im.getSize().y;
	texdata.depth = 1;
	texdata.format = RGBA8;
	texdata.is_cubemap = false;
	texdata.is_layered = false;
	texdata.num_levels = 1;

	TextureGL* tex = createTexture(texdata);

	rTextureUpdate texupdata;
	texupdata.data = (char*)im.getPixelsPtr();
	texupdata.depth = texdata.depth;
	texupdata.format = texdata.format;
	texupdata.width = texdata.width;
	texupdata.height = texdata.height;
	texupdata.level = 0;
	texupdata.x_offset = 0;
	texupdata.y_offset = 0;
	texupdata.z_offset = 0;

	WriteTexture(tex, texupdata);
	return tex;
}

IndexBufferGL* GapiGL::createIndexBuffer(const rBuffer& data)
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

void GapiGL::WriteTexture(ITexture* t, const rTextureUpdate& d)
{
	TextureGL* tex = (TextureGL*)t;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(tex->target, tex->ID);

	if (tex->dim == 1)
	{
		glTextureSubImage1D(tex->ID, d.level, d.x_offset, d.width, texture_formats[d.format], texture_types[d.format], d.data);
	}
	else if (tex->dim == 2)
	{
		//glTextureSubImage2D( id, data->level, data->x_offset, data->y_offset, data->width, data->height, texture_formats[data->format], texture_types[data->format], data->data );
		glTexSubImage2D(tex->target, d.level, d.x_offset, d.y_offset, d.width, d.height, texture_formats[d.format], texture_types[d.format], d.data);

	}
	else //threesome
	{
		glTextureSubImage3D(tex->ID, d.level, d.x_offset, d.y_offset, d.z_offset, d.width, d.height, d.depth, texture_formats[d.format], texture_types[d.format], d.data);
	}
}

void GapiGL::setDepthState(const rDepthState& state)
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

void GapiGL::setStencilState(const rStencilState& state)
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

void GapiGL::setBlendState(const rBlendState& state)
{
	if (state.enable)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	glBlendColor(state.blend_color.x, state.blend_color.y, state.blend_color.z, state.blend_color.w);

	glBlendEquation(blend_eq_data[(u32)state.equation]);

	glBlendFunc(blend_func_data[(u32)state.src_func], blend_func_data[(u32)state.dst_func]);
}

void GapiGL::setSamplerState(const char* slotName, const rSamplerState& smpdata, ITexture* t)
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

void GapiGL::setSRGBWrites(bool val)
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

void GapiGL::setSeamlessCubeMaps(bool val)
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

void GapiGL::setViewport(int x, int y, u32 w, u32 h)
{
	glViewport(x, y, w, h);
}

void GapiGL::setRasterizationState(const rRasterizerState& state)
{
	glPolygonMode(GL_FRONT_AND_BACK, raster_mode_data[(u32)state.mode]);
	glFrontFace(raster_order_data[(u32)state.vertex_order]);
	glCullFace(raster_face_data[(u32)state.face]);
	glColorMask(state.r_mask, state.g_mask, state.b_mask, state.a_mask);
}

bool GapiGL::getError() //true if error
{
	return glGetError != GL_NO_ERROR;
}

void GapiGL::setDebugOutput(bool val)
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

void GapiGL::setSyncDebugOutput(bool val)
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

void GapiGL::setShaderProgram(IShaderProgram* sp)
{
	ASSERT(sp);
	glUseProgram(static_cast<ShaderProgramGL*>(sp)->getProgramId());
}

void GapiGL::setTexture(ITexture* t, u32 idx)
{
	TextureGL* tex = (TextureGL*)t;

	// Activate slot
	glActiveTexture(GL_TEXTURE0 + idx);

	// Bind texture
	glBindTexture(tex->target, tex->viewID);
}

void GapiGL::setRenderTargets(const rRenderTargetInfo* render_targets, u32 size)
{
	//TODO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GapiGL::setUniformBuffer(IUniformBuffer* buf, u32 index)
{
	ASSERT(buf);
	glBindBufferBase(GL_UNIFORM_BUFFER, index, static_cast<UniformBufferGL*>(buf)->id);
}

GLenum attrib_array[] =
{
	GL_FLOAT, GL_INT, GL_UNSIGNED_INT
};

void GapiGL::setVertexBuffers(IVertexBuffer** buffers, const rVertexAttrib* attrib_data, u32 num_buffers)
{
	ASSERT(buffers && attrib_data);

	for (int c = 0; c < num_buffers; ++c)
	{
		GLuint id = static_cast<VertexBufferGL*>(buffers[c])->id;
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glEnableVertexAttribArray(attrib_data[c].index);
		glVertexAttribPointer(attrib_data[c].index, attrib_data[c].nComponent, attrib_array[(u32)attrib_data[c].type], false, attrib_data[c].size, (const void*)attrib_data[c].offset);
		//glVertexAttribDivisor( attrib_data[c].index, attrib_data[c].divisor ); //instancing stuff
	}
}

void GapiGL::setIndexBuffer(IIndexBuffer* ibo)
{
	ASSERT(ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<IndexBufferGL*>(ibo)->id);
}

void GapiGL::draw(u32 num_indices)
{
#ifdef DEBUG_SHADER_ERRORS
	glValidateProgram(static_cast<ShaderProgram*>(s)->id);
	GLchar infolog[INFOLOG_SIZE];
	infolog[0] = '\0';
	glGetProgramInfoLog(static_cast<ShaderProgram*>(s)->id, INFOLOG_SIZE, 0, infolog);
	cerr << infolog << endl;
#endif

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
}

void GapiGL::clearFrameBuffer(eClearFlag f, const mm::vec4& color, float depth /*= 0*/, i32 stencil /*= 0*/)
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