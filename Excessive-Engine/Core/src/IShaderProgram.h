#ifndef IShaderProgram_h
#define IShaderProgram_h

#include "mymath/mymath.h"

#include "IShaderSource.h"
#include "ITexture.h"
#include "IBuffer.h"
#include <vector>

class IShaderProgram
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void addCompiledShaderSource(IShaderSource* source) = 0;
    virtual void link() = 0;
    virtual void getBinary(std::vector<i8>& data) = 0;
    virtual void loadFromBinary(const std::vector<i8>& data) = 0;
    virtual void use() = 0;

    virtual void pass(u32 location, i8 data) = 0;
    virtual void pass(u32 location, u8 data) = 0;
    virtual void pass(u32 location, i16 data) = 0;
    virtual void pass(u32 location, u16 data) = 0;
    virtual void pass(u32 location, i32 data) = 0;
    virtual void pass(u32 location, u32 data) = 0;
    virtual void pass(u32 location, mm::vec2 data) = 0;
    virtual void pass(u32 location, mm::vec3 data) = 0;
    virtual void pass(u32 location, mm::vec4 data) = 0;
    virtual void pass(u32 location, mm::mat2 data) = 0;
    virtual void pass(u32 location, mm::mat3 data) = 0;
    virtual void pass(u32 location, mm::mat4 data) = 0;
    virtual void pass(u32 location, ITexture* data) = 0;
    virtual void pass(u32 location, IBuffer* data) = 0;
};

#endif