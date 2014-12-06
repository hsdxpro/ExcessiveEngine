#pragma once

#include <cstdint>
#include <mymath/mymath.h>

class ITextureView;

////////////////////////////////////////
// depth stencil
enum class eCompareFunc : uint32_t
{
	SHALL_NOT_PASS = 0,
	EQUAL,
	NOT_EQUAL,
	LESSER,
	LESSER_OR_EQUAL,
	GREATER,
	GREATER_OR_EQUAL,
	ALWAYS,
};

struct rDepthState
{
	bool enable_test;
	bool enable_write;
	float near, far;
	eCompareFunc func;
};

enum class eStencilAction : uint32_t
{
	KEEP = 0,
	ZERO_OUT,
	REPLACE,
	INCREMENT,
	INCREMENT_WRAP,
	DECREMENT,
	DECREMENT_WRAP,
	INVERT,
};

struct rStencilState
{
	bool enable_test;
	bool enable_write;
	unsigned reference;
	unsigned mask;
	unsigned func_mask;
	eCompareFunc func;
	eStencilAction on_stencil_fail, on_stencil_pass_depth_fail, on_stencil_pass_depth_pass;
};


////////////////////////////////////////
// blending
enum class eBlendEquation : uint32_t
{
	SRC_PLUS_DEST = 0,
	SRC_MINUS_DEST,
	B_MINUS_A, MIN, MAX
};

enum class eBlendFunc : uint32_t
{
	ZERO_OUT = 0, ONE_OUT, SRC_COLOR, ONE_MIN_SRC_COLOR, DST_COLOR, ONE_MIN_DST_COLOR,
	SRC_ALPHA, ONE_MINUS_SRC_ALPHA, DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA, SRC_ALPHA_SATURATE,
	SECOND_SRC_COLOR, ONE_MINUS_SECOND_SRC_COLOR, SECOND_SRC_ALPHA, ONE_MINUS_SECOND_SRC_ALPHA
};

struct rBlendState
{
	bool enable;
	mm::vec4 blend_color;
	eBlendEquation equation;
	eBlendFunc src_func, dst_func;
};


////////////////////////////////////////
// rasterization
enum class eRasterizationMode : uint32_t
{
	POINT = 0, WIREFRAME, SOLID
};

enum class eWhichFace : uint32_t//!! redundancy *
{
	FRONT = 0, BACK, FRONT_AND_BACK
};

enum class eVertexOrder : uint32_t //!! redundancy *
{
	CLOCKWISE = 0, COUNTER_CLOCKWISE
};

struct rRasterizerState
{
	eRasterizationMode mode;
	eWhichFace face;
	eVertexOrder vertex_order;
	bool face_culling;
	bool r_mask, g_mask, b_mask, a_mask;
};


////////////////////////////////////////
// render targets

//this specifies how we'd like to use 1 texture
//in this frame buffer object (render target)
struct rRenderTargetInfo
{
	ITextureView* target_texture;
	unsigned target_index;
	unsigned target_level;
	unsigned target_layer;
};

enum class eVertexAttribType : uint32_t
{
	FLOAT = 0, INT, UINT, SHORT, USHORT, BYTE, UBYTE
};

struct rVertexAttrib
{
	unsigned index; //vertex stream index
	unsigned data_components; //4 for vec4
	eVertexAttribType type;
	unsigned offset, size;
	unsigned divisor;
};



