#pragma once

#include <mymath/mymath.h>

// Remove this unless really necessary
// Reason: no longer needed
/*
enum eShaderType
{
	VERTEX_SHADER = 0, TESSELLATION_CONTROL_SHADER, TESSELLATION_EVALUATION_SHADER,
	GEOMETRY_SHADER, PIXEL_SHADER, COMPUTE_SHADER
};
*/

/// Whole shader program for rendering.
class IShaderProgram
{
public:
	virtual void destroy() = 0;

	/// Get compiled binary shader code.
	/// \param max_size Write no more bytes to data.
	/// \return Number of bytes written. If data is null, then the binary's size.
	//virtual size_t GetBinary(void* data, size_t max_size) = 0;

	/// Load this shader from binary code.
	/// \param data The shader's binary code.
	/// \param size Number of bytes in data.
	/// \return Indicates success.
	//virtual bool loadBinary(void* data, size_t size) = 0;


	// Remove this unless really necessary.
	// Reason: OpenGL specific.
	// Functionality to be replaced by string based calls elsewhere.
	virtual int GetUniformBlockIndex(const char* str) = 0;
	virtual int GetAttributeIndex(const char* str) = 0;
	virtual int GetSamplerIndex(const char* str) = 0;
	virtual int GetRenderTargetIndex(const char* str) = 0;
};