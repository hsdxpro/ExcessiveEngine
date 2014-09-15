#pragma once

enum eVertexAttribType
{
  ATTRIB_FLOAT = 0, ATTRIB_INT, ATTRIB_UNSIGNED_INT
};

struct rVertexAttrib
{
  unsigned index; //vertex stream index
  unsigned data_components; //4 for vec4
  eVertexAttribType type;
  unsigned offset, size;
  unsigned divisor;
};

struct rVertexFormat
{
  rVertexAttrib attribs[16];
};

class IVertexArray
{
  public:
    virtual void destroy() = 0;
};