#ifndef IBuffer_h
#define IBuffer_h

#include <vector>

//TODO
//decide to have separate classes for each buffer type
//or operate with enums
class IBuffer
{
  public:
    virtual void create(bool prefer_gpu_storage = true) = 0; //TODO
    virtual void destroy() = 0;
    virtual void allocate(unsigned size) = 0;
    virtual void update(unsigned offset, unsigned size, void* data) = 0; 
    virtual void getData(std::vector<char>& data) = 0;
    virtual void getSubData(std::vector<char>& data, unsigned offset, unsigned size) = 0;
    virtual unsigned getSize() = 0;
};

#endif