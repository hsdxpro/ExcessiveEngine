#ifndef IBuffer_h
#define IBuffer_h

#include "basic_types.h"
#include <vector>

//TODO
//decide to have separate classes for each buffer type
//or operate with enums
class IBuffer
{
  public:
    virtual void create(bool prefer_gpu_storage = true) = 0; //TODO
    virtual void destroy() = 0;
    virtual void allocate(u32 size) = 0;
    virtual void update(u32 offset, u32 size, void* data) = 0; 
    virtual void getData(std::vector<i8>& data) = 0;
    virtual void getSubData(std::vector<i8>& data, u32 offset, u32 size) = 0;
    virtual u32 getSize() = 0;
};

#endif