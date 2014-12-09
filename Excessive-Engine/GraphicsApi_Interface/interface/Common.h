#pragma once

#include <cstdint>
#include <mymath/mymath.h>

// TODO
#include <string>

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
	// 32 bit
	FLOAT = 0,
	SINT_32,
	UINT_32,
	SNORM_32,
	UNORM_32,
	// 16 bit
	HALF,
	SINT_16,
	UINT_16,
	SNORM_16,
	UNORM_16,
	// 8 bit
	SINT_8,
	UINT_8,
	SNORM_8,
	UNORM_8,

};

struct rVertexAttrib
{
	unsigned index; //vertex stream index
	unsigned data_components; //4 for vec4
	eVertexAttribType type;
	unsigned offset, size;
	unsigned divisor;
};

struct rShaderProgPaths
{
	rShaderProgPaths() :vsPath(0), tessCtrlPath(0), tessEvaluationPath(0), gsPath(0), psPath(0){}
	char const* vsPath;
	char const* tessCtrlPath;
	char const* tessEvaluationPath;
	char const* gsPath;
	char const* psPath;
};

struct rShaderProgSources
{
	rShaderProgSources():vsSrc(0), tessCtrlSrc(0), tessEvaluationSrc(0), gsSrc(0), psSrc(0){}
	char const* vsSrc;
	char const* tessCtrlSrc;
	char const* tessEvaluationSrc;
	char const* gsSrc;
	char const* psSrc;
};



