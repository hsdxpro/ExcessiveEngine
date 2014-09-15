#ifndef IBuffer_h
#define IBuffer_h

#include <vector>

struct rAllocData
{
  unsigned size;
  bool is_readable;
  bool is_writable;
  bool is_persistent;
  bool prefer_cpu_storage;
};

class IBuffer
{
  public:
    virtual void create() = 0;
    virtual void destroy() = 0;
    virtual void allocate(const rAllocData& data) = 0;
    virtual void update(unsigned offset, unsigned size, void* data) = 0; 
    virtual void getSubData(char* data, unsigned offset, unsigned size) = 0;
    virtual rAllocData getAllocData() = 0;
};

//Implement the ones below

class IVertexBuffer : public IBuffer
{
  public:
    
};

class IIndexBuffer : public IBuffer
{
  public:
    
};

class IUniformBuffer : public IBuffer
{
  public:
    
};

#endif