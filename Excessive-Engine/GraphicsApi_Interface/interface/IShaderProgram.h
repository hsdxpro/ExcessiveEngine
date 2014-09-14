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
    virtual void addShader(const std::string& src) = 0; //TODO
    virtual void link() = 0;
    virtual void getBinary(std::vector<char>& data) = 0;
    virtual void loadFromBinary(const std::vector<char>& data) = 0;
    virtual void use() = 0;
};

#endif