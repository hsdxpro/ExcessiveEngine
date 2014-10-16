#pragma once

#include "mymath/mymath.h"

#include "ITextureView.h"
#include "ITexture.h"
#include "IBuffer.h"
#include "IShaderProgram.h"

//!! PASS_IF is redundant, remove from name; rename SHALL_PASS to ALWAYS
enum eCompareFunc
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
enum eStencilAction
{
	KEEP_VALUE = 0, ZERO_OUT, REPLACE_W_REF, INCREMENT, INCREMENT_WRAP, DECREMENT, 
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
enum eBlendEquation
{
	A_PLUS_B = 0, A_MINUS_B, B_MINUS_A, MIN_A_B, MAX_A_B
};

enum eBlendFunc
{
	ZERO_OUT = 0, ONE_OUT, SRC_COLOR, ONE_MIN_SRC_COLOR, DST_COLOR, ONE_MIN_DST_COLOR,
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

enum eRasterizationMode
{
	POINT = 0, WIREFRAME, SOLID
};

enum eWhichFace //!! redundancy *
{
	FRONT = 0, BACK, FRONT_AND_BACK
};

enum eVertexOrder //!! redundancy *
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
struct rTargetData
{
	ITextureView* target_texture;
	unsigned target_index;
	unsigned target_level;
	unsigned target_layer;
};

enum eVertexAttribType
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
enum eDimensions
{
	ONE = 0, TWO, THREE
};

//specifices what type of texture you'd like to allocate
struct rTextureViewData
{
	unsigned start_level; //mipmap levels
	unsigned num_levels;
	unsigned start_layer;
	unsigned num_layers;
	eFormatType format; //texture format
	ITexture* base_tex;
	//new texture target
	eDimensions dim;
	bool is_layered;
	bool is_cubemap;
};

class IGapi
{
  public:
    virtual IShaderProgram* createShaderProgram() = 0;
	virtual ITexture* createTexture(rTextureData* data) = 0;
	virtual ITextureView* createTextureView(rTextureViewData* data) = 0;
	virtual IVertexBuffer* createVertexBuffer(rAllocData* data) = 0;
	virtual IIndexBuffer* createIndexBuffer(rAllocData* data) = 0;
	virtual IUniformBuffer* createUniformBuffer(rAllocData* data) = 0;
  
    virtual void setDepthState(rDepthState* state) = 0;
    virtual void setStencilState(rStencilState* state) = 0;
    virtual void setBlendState(rBlendState* state) = 0;
	virtual void setRasterizationState(rRasterizerState* state) = 0;

    virtual void setSRGBWrites(bool val) = 0;
    virtual void setSeamlessCubeMaps(bool val) = 0;
    
    virtual void setViewport(int x, int y, unsigned w, unsigned h) = 0;    

    virtual bool getError() = 0;
    virtual void setDebugOutput(bool val) = 0;
    virtual void setSyncDebugOutput(bool val) = 0;

    //pass input/output to shader
    virtual void passTextureView(IShaderProgram* s, ITextureView* tex, unsigned index) = 0;
    virtual void passRenderTargets(IShaderProgram* s, rTargetData* render_targets, unsigned size) = 0;
    virtual void passUniformBuffer(IShaderProgram* s, IUniformBuffer* buf, unsigned index) = 0;
    virtual void passVertexBuffers(IShaderProgram* s, IVertexBuffer** vbos, rVertexAttrib* attrib_data, unsigned num_vbos) = 0;
    virtual void passIndexBuffer(IShaderProgram* s, IIndexBuffer* ibo) = 0;
    
    //draw stuff
    virtual void draw(IShaderProgram* s, unsigned num_indices) = 0;
	//!! Why is shader program handled differently?
	//!! Shader program has the same relation to the pipeline as anything else: it's bound to the pipeline.
	//!! Therefore bind it the same way as a setShaderProgram() call.
	//!! This implies renaming others as setIndexBuffer etc.
};