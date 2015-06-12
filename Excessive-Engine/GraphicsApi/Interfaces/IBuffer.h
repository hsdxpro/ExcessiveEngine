#pragma once

#include "../../Common/src/BasicTypes.h"

struct rBuffer {
	u32 size;
	bool is_readable;
	bool is_writable;
	bool is_persistent;
	bool prefer_cpu_storage;
	void* initial_data;
};

class IBuffer
{
public:
	virtual void destroy() = 0;
	virtual rBuffer GetDesc() = 0;
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