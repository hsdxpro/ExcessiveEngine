#pragma once

#include "mymath/mymath.h"

#include "ITexture.h"
#include "IBuffer.h"
#include <string>
#include <vector>

enum eShaderType
{
  VERTEX_SHADER, TESSELLATION_CONTROL_SHADER, TESSELLATION_EVALUATION_SHADER,
  GEOMETRY_SHADER, PIXEL_SHADER, COMPUTE_SHADER
};

struct rShaderBinaryData
{
  std::vector< char > data;
};

class IShaderProgram
{
  public:
    virtual void destroy() = 0;
    virtual void addShader(const std::string& src, eShaderType type) = 0; //src is the shader source string
    virtual void link() = 0;
    virtual void getBinary(rShaderBinaryData& data) = 0;
    virtual void loadFromBinary(const rShaderBinaryData& data) = 0;
    
    virtual void pass(ITextureView* tex, unsigned index) = 0;
    virtual void pass(IFrameBuffer* buf) = 0;
    virtual void pass(IUniformBuffer* buf) = 0;
    virtual void pass(IVertexArray* array) = 0;
    
    virtual void draw(unsigned num_indices) = 0;
};