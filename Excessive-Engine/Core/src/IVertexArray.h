#ifndef IVertexArray_h
#define IVertexArray_h

class IVertexArray
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void draw() = 0; //TODO
    virtual void use() = 0;
};

#endif