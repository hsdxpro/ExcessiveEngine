#ifndef IShaderSource_h
#define IShaderSource_h

#include "basic_types.h"
#include <string>

//TODO
//decide to have separate classes for vertex/pixel/geometry shaders etc.
//or operate with enums
class IShaderSource
{
  public:
    virtual void create() = 0; //TODO
    virtual void destroy() = 0;
    virtual void setSource(const std::string& src) = 0;
    virtual void compileShader() = 0;
};

#endif