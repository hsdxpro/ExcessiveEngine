#ifndef IShaderProgram_h
#define IShaderProgram_h

#include "mymath/mymath.h"

#include "ITexture.h"
#include "IBuffer.h"
#include <vector>
#include <string>

class IShaderProgram
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void addShader(const char* src) = 0; //TODO
    virtual void link() = 0;
    virtual char* getBinary() = 0;
    virtual void loadFromBinary(char* data, unsigned size) = 0;
    virtual void use() = 0;
};

#endif