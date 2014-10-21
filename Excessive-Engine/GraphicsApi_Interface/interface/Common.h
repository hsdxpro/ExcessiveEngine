#pragma once

#include <cstdint>
#include <mymath/mymath.h>

class ITextureView;

//!! PASS_IF is redundant, remove from name; rename SHALL_PASS to ALWAYS
enum class eCompareFunc : uint32_t
{
	SHALL_NOT_PASS = 0, PASS_IF_LESS, PASS_IF_EQUAL, PASS_IF_LESS_OR_EQUAL,
	PASS_IF_GREATER, PASS_IF_NOT_EQUAL, PASS_IF_GREATER_OR_EQUAL, SHALL_PASS
};

struct rDepthState
{
	bool depth_test; //!! rename to 'enable_test'
	bool depth_mask; //!! rename to 'enable_write'
	float near, far;
	eCompareFunc func;
};
//!! rename (redundancy): INVERT_BITS->INVERT, KEEP_VALUE->KEEP, REPLACE_W_REF->REPLACE
enum class eStencilAction : uint32_t
{
	KEEP_VALUE = 0, stencilZERO_OUT, REPLACE_W_REF, INCREMENT, INCREMENT_WRAP, DECREMENT,
	DECREMENT_WRAP, INVERT_BITS
};

struct rStencilState
{
	bool stencil_test; //!! rename to enable_test
	bool stencil_mask; //!! rename to enable_write (name collision)
	unsigned reference_stencil_value; //!! rename to reference
	unsigned mask;
	unsigned func_mask;
	eCompareFunc func;
	eStencilAction on_stencil_fail, on_stencil_pass_depth_fail, on_stencil_pass_depth_pass;
};
//!! ambiguous: which is A and which is B? may use SRC_MINUS_DEST
enum class eBlendEquation : uint32_t
{
	A_PLUS_B = 0, A_MINUS_B, B_MINUS_A, MIN_A_B, MAX_A_B
};

enum class eBlendFunc : uint32_t
{
	blendZERO_OUT = 0, ONE_OUT, SRC_COLOR, ONE_MIN_SRC_COLOR, DST_COLOR, ONE_MIN_DST_COLOR,
	SRC_ALPHA, ONE_MINUS_SRC_ALPHA, DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA, SRC_ALPHA_SATURATE,
	SECOND_SRC_COLOR, ONE_MINUS_SECOND_SRC_COLOR, SECOND_SRC_ALPHA, ONE_MINUS_SECOND_SRC_ALPHA
};

struct rBlendState
{
	bool blend_test; //!! rename to 'enable'
	mm::vec4 blend_color;
	eBlendEquation equation;
	eBlendFunc src_func, dst_func;
};

enum class eRasterizationMode : uint32_t
{
	rastPOINT = 0, WIREFRAME, SOLID
};

enum class eWhichFace : uint32_t//!! redundancy *
{
	FRONT = 0, BACK, FRONT_AND_BACK
};

enum class eVertexOrder : uint32_t //!! redundancy *
{
	CLOCKWISE = 0, COUNTER_CLOCKWISE
};
//!! * merge the two concepts above
//!! every mesh must be stored by convention
/*
enum eCullMode {
CLOCKWISE, COUNTER_CLOCKWISE
}
*/

struct rRasterizerState
{
	eRasterizationMode mode;
	eWhichFace face;
	eVertexOrder vertex_order;
	bool face_culling;
	bool r_mask, g_mask, b_mask, a_mask;
};

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
	ATTRIB_FLOAT = 0, ATTRIB_INT, ATTRIB_UNSIGNED_INT
};

struct rVertexAttrib
{
	unsigned index; //vertex stream index
	unsigned data_components; //4 for vec4
	eVertexAttribType type;
	unsigned offset, size;
	unsigned divisor;
};
//!! unnecessary enum: replace with an integer, enums can be made just as wrong as an int, since they are an int...
enum class eDimensions : uint32_t
{
	ONE = 0, TWO, THREE
};
