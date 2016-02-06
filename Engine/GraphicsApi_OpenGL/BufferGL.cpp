#include "BufferGL.h"

#include <string>

void BufferGL::Release()
{
	glDeleteBuffers(1, &id);
	id = 0;
}

auto BufferGL::GetDesc() -> rBuffer
{
	return adata;
}