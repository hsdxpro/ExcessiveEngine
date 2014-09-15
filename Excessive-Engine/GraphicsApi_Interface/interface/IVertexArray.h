#ifndef IVertexArray_h
#define IVertexArray_h

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
  rVertexAttrib generic[8];
  rVertexAttrib texcoord[8];
};

class IVertexArray
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void draw(unsigned indices) = 0;
    virtual void use() = 0;
};

#endif