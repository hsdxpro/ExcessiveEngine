#pragma once

#include "GL/glew.h"

#include "IShaderProgram.h"

#define DEBUG_SHADER_ERRORS

#ifdef DEBUG_SHADER_ERRORS
#define INFOLOG_SIZE 4096
#endif

class ShaderProgram : public IShaderProgram
{
  public:
    GLuint id;
  
    void destroy();
    void addShader(const char* src, eShaderType type);
    void link();
    void getBinary(char** data, unsigned* size);
    void loadFromBinary(char* data);
};