#pragma once

#include "GL/glew.h"

#include "IShaderProgram.h"

class ShaderProgram : public IShaderProgram
{
  public:
    GLuint id;
  
    void destroy();
    void addShader(const char* src, eShaderType type);
    void link();
    void getBinary(char** data, unsigned* size);
    void loadFromBinary(char* data, unsigned size);
};