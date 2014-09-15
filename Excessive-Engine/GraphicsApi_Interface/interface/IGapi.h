#ifndef IGapi_h
#define IGapi_h

#include "mymath/mymath.h"

enum eFunc
{
  SHALL_NOT_PASS = 0, PASS_IF_LESS, PASS_IF_EQUAL, PASS_IF_LESS_OR_EQUAL,
  PASS_IF_GREATER, PASS_IF_NOT_EQUAL, PASS_IF_GREATER_OR_EQUAL, SHALL_PASS
};

struct rDepthState
{
  bool depth_test;
  bool depth_mask;
  float near, far;
  eFunc func;
};

enum eStencilAction
{
  KEEP_VALUE = 0, ZERO_OUT, REPLACE_W_REF, INCREMENT, INCREMENT_WRAP, DECREMENT, 
  DECREMENT_WRAP, INVERT_BITS
};

struct rStencilState
{
  bool stencil_test;
  bool stencil_mask;
  unsigned reference_stencil_value;
  unsigned stencil_mask;
  eFunc func;
  eStencilAction on_stencil_fail, on_stencil_pass_depth_fail, on_stencil_pass_depth_pass;
};

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
  bool blend_test;
  mm::vec4 blend_color;
  eBlendEquation equation;
  eBlendFunc src_func, dst_func;
};

enum eRasterizationMode
{
  POINT = 0, WIREFRAME, SOLID
};

enum eWhichFace
{
  FRONT = 0, BACK, FRONT_AND_BACK
};

enum eVertexOrder
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

class IGapi
{
  public:
    virtual void setDepthState(const rDepthState& state) = 0;

    virtual void setStencilState(const rStencilState& state) = 0;

    virtual void setBlendState(const rBlendState& state) = 0;

    virtual void setSRGBWrites(bool val) = 0;
    virtual void setSeamlessCubeMaps(bool val) = 0;
    
    virtual void setViewport(int x, int y, unsigned w, unsigned h) = 0;

    virtual void setRasterizationState(const rRasterizerState& state) = 0;

    virtual bool getError() = 0;
    virtual void setDebugOutput(bool val) = 0;
    virtual void setSyncDebugOutput(bool val) = 0;
};



#endif