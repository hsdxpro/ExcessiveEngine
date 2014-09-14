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

    virtual void pass(unsigned location, bool data) = 0;
    virtual void pass(unsigned location, int data) = 0;
    virtual void pass(unsigned location, unsigned data) = 0;
    virtual void pass(unsigned location, mm::vec2 data) = 0;
    virtual void pass(unsigned location, mm::vec3 data) = 0;
    virtual void pass(unsigned location, mm::vec4 data) = 0;
    virtual void pass(unsigned location, mm::mat2 data) = 0;
    virtual void pass(unsigned location, mm::mat3 data) = 0;
    virtual void pass(unsigned location, mm::mat4 data) = 0;
    virtual void pass(unsigned location, ITexture* data) = 0;
    virtual void pass(unsigned location, IBuffer* data) = 0;
};

#endif