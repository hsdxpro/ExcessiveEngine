#pragma once

#include "mymath/mymath.h"

enum eShaderType
{
  VERTEX_SHADER = 0, TESSELLATION_CONTROL_SHADER, TESSELLATION_EVALUATION_SHADER,
  GEOMETRY_SHADER, PIXEL_SHADER, COMPUTE_SHADER
};

class IShaderProgram
{
  public:
    virtual void destroy() = 0;
    //src is the shader source string
    virtual void addShader(const char* src, eShaderType type) = 0; 
    virtual void link() = 0;
    //overwrites pointer at data, size
    virtual void getBinary(char** data, unsigned* size) = 0;
    virtual void loadFromBinary(char* data) = 0;

    virtual int getUniformBlockIndex( const char* str ) = 0;
    virtual int getAttributeIndex( const char* str ) = 0;
    virtual int getSamplerIndex( const char* str ) = 0;
    virtual int getRenderTargetIndex( const char* str ) = 0;
};