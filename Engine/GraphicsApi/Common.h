#pragma once

#include <mymath/mymath.h>
#include <SupportLibrary/BasicTypes.h>

// TODO
#include <string>

class ITextureView;

////////////////////////////////////////
// depth stencil
enum class eCompareFunc : u32
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
	rDepthState(): enable_test(true), enable_write(true), near(0), far(0), func(eCompareFunc::ALWAYS) {}

	bool enable_test;
	bool enable_write;
	float near, far;
	eCompareFunc func;
};

enum class eStencilAction : u32
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
	u32 reference;
	u32 mask;
	u32 func_mask;
	eCompareFunc func;
	eStencilAction on_stencil_fail, on_stencil_pass_depth_fail, on_stencil_pass_depth_pass;
};


////////////////////////////////////////
// blending
enum class eBlendEquation : u32
{
	SRC_PLUS_DEST = 0,
	SRC_MINUS_DEST,
	B_MINUS_A, MIN, MAX
};

enum class eBlendFunc : u32
{
	ZERO_OUT = 0, ONE_OUT, SRC_COLOR, ONE_MIN_SRC_COLOR, DST_COLOR, ONE_MIN_DST_COLOR,
	SRC_ALPHA, ONE_MINUS_SRC_ALPHA, DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA, SRC_ALPHA_SATURATE,
	SECOND_SRC_COLOR, ONE_MINUS_SECOND_SRC_COLOR, SECOND_SRC_ALPHA, ONE_MINUS_SECOND_SRC_ALPHA
};

struct rBlendState
{
	rBlendState():enable(false), blend_color(1,1,1,1), equation(eBlendEquation::SRC_PLUS_DEST), src_func(eBlendFunc::ONE_MINUS_DST_ALPHA), dst_func(eBlendFunc::ONE_MINUS_SRC_ALPHA){}

	bool enable;
	mm::vec4 blend_color;
	eBlendEquation equation;
	eBlendFunc src_func, dst_func;
};


////////////////////////////////////////
// rasterization
enum class eRasterizationMode : u32
{
	POINT = 0, WIREFRAME, SOLID
};

enum class eWhichFace : u32//!! redundancy *
{
	FRONT = 0, BACK, FRONT_AND_BACK
};

enum class eVertexOrder : u32 //!! redundancy *
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
	u32 target_index;
	u32 target_level;
	u32 target_layer;
};

enum class eVertexAttribType : u32
{
	// floating point types
	FLOAT = 0,
	HALF,
	// integer types
	SINT_32,
	UINT_32,
	SINT_16,
	UINT_16,
	SINT_8,
	UINT_8,
	// normalized integer types
	// signed: [-1.0f, 1.0f]
	// unsigned: [0.0f, 1.0f]
	SNORM_32,
	UNORM_32,
	SNORM_16,
	UNORM_16,
	SNORM_8,
	UNORM_8,
};

struct rVertexAttrib
{
	u32 index; //vertex stream index
	u32 nComponent; //4 for vec4
	eVertexAttribType type;
	u32 offset, size;
	u32 divisor;
};

struct rShaderPaths
{
	rShaderPaths() :vsPath(0), tessCtrlPath(0), tessEvaluationPath(0), gsPath(0), psPath(0){}
	char const* vsPath;
	char const* tessCtrlPath;
	char const* tessEvaluationPath;
	char const* gsPath;
	char const* psPath;
};

struct rShaderSources
{
	rShaderSources() :vsSrc(0), tessCtrlSrc(0), tessEvaluationSrc(0), gsSrc(0), psSrc(0){}
	char const* vsSrc;
	char const* tessCtrlSrc;
	char const* tessEvaluationSrc;
	char const* gsSrc;
	char const* psSrc;
};

enum class eClearFlag : u32 {
	COLOR				= 1,
	DEPTH				= 2,
	STENCIL				= 4,

	COLOR_DEPTH_STENCIL	= COLOR | DEPTH | STENCIL,
	COLOR_DEPTH			= COLOR | DEPTH,
	COLOR_STENCIL		= COLOR | STENCIL,
	DEPTH_STENCIL		= DEPTH | STENCIL,
};

