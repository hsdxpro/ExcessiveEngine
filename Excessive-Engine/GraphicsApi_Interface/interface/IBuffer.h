#pragma once

#include "../../Common/src/BasicTypes.h"


class IBuffer
{
public:
	struct rDesc {
		u32 size;
		bool is_readable;
		bool is_writable;
		bool is_persistent;
		bool prefer_cpu_storage;
	};
public:
	virtual void destroy() = 0;
	virtual void update(char* data, unsigned size, unsigned offset) = 0; 
	virtual void getSubData(char* data, unsigned size, unsigned offset) = 0;
	virtual rDesc getDesc() = 0;
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