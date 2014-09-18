#pragma once

#include "mymath/mymath.h"

#include "ITexture.h"
#include "IBuffer.h"

enum eShaderType
{
  VERTEX_SHADER, TESSELLATION_CONTROL_SHADER, TESSELLATION_EVALUATION_SHADER,
  GEOMETRY_SHADER, PIXEL_SHADER, COMPUTE_SHADER
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

class IShaderProgram
{
  public:
    virtual void destroy() = 0;
    //src is the shader source string
    virtual void addShader(const char* src, eShaderType type) = 0; 
    virtual void link() = 0;
    virtual void getBinary(char* data, unsigned size) = 0;
    virtual void loadFromBinary(char* data, unsigned size) = 0;
    
    virtual void pass(ITextureView* tex, unsigned index) = 0;
    virtual void pass(rTargetData* render_targets, unsigned size) = 0;
    virtual void pass(IUniformBuffer* buf) = 0;
    virtual void pass(IVertexBuffer* vbos, unsigned num_vbos) = 0;
    
    virtual void draw(unsigned num_indices) = 0;
};