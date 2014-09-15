#pragma once

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
    virtual void destroy() = 0;
    virtual void update(unsigned offset, const std::vector< char >& data) = 0; 
    virtual void getSubData(std::vector< char >& data, unsigned offset) = 0;
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