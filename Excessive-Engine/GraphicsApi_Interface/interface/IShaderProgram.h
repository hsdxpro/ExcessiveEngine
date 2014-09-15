#ifndef IShaderProgram_h
#define IShaderProgram_h

#include "mymath/mymath.h"

#include "ITexture.h"
#include "IBuffer.h"
#include <vector>
#include <string>

struct rShaderBinaryData
{
  char* data;
  unsigned size;
};

class IShaderProgram
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void addShader(const char* src) = 0; //src is the shader source string
    virtual void link() = 0;
    virtual void getBinary(rShaderBinaryData& data) = 0;
    virtual void loadFromBinary(const rShaderBinaryData& data) = 0;
    virtual void use() = 0;
};

#endif