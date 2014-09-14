#ifndef ITexture_h
#define ITexture_h

#include <vector>

//TODO
//decide whether or not to have separate classes for 2d/3d/cube etc.
//or operate with enums
class ITexture
{
  public:
    virtual void create() = 0; //TODO
    virtual void destroy() = 0;
    virtual void allocate() = 0; //TODO
    virtual void setWrapping() = 0; //TODO
    virtual void setFiltering() = 0; //TODO
    virtual void update() = 0; //TODO
    virtual void getData(std::vector<char>& data) = 0;
    virtual void getSubData(std::vector<char>& data) = 0; //TODO
    virtual void getSize() = 0; //TODO
    virtual void genMipChain() = 0; //TODO
};

#endif