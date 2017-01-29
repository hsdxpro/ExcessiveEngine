////////////////////////////////////////////////////////////////////////////////
//	GraphicsEngine/src/IndexBufferD3D11.cpp
//	2012.oct.12, zsiroskenyer team, Péter Kardos
////////////////////////////////////////////////////////////////////////////////
//	Direct3D 11 index buffer implementation
////////////////////////////////////////////////////////////////////////////////

#include "IndexBufferD3D11.h"

#include <d3d11.h>
cIndexBufferD3D11::cIndexBufferD3D11(ID3D11Buffer* buffer, size_t byteSize, eUsage usage)
:buffer(buffer), usage(usage), byteSize(byteSize) {
}

cIndexBufferD3D11::~cIndexBufferD3D11() {
	buffer->Release();
}

void cIndexBufferD3D11::Release() {
	delete this;
}

size_t cIndexBufferD3D11::GetByteSize() const {
	return byteSize;
}

eUsage cIndexBufferD3D11::GetUsage() const {
	return usage;
}

ID3D11Buffer* cIndexBufferD3D11::GetBufferPointer() const {
	return buffer;
}