#include "BufferGL.h"

#include <string>

void BufferGL::destroy()
{
	glDeleteBuffers(1, &id);
	id = 0;
}

auto BufferGL::getDesc() -> rBuffer
{
	return adata;
}