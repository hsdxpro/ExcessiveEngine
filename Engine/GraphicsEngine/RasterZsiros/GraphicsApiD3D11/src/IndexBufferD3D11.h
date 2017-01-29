////////////////////////////////////////////////////////////////////////////////
//	GraphicsEngine/src/IndexBufferD3D11.h
//	2012.oct.12, zsiroskenyer team, Péter Kardos
////////////////////////////////////////////////////////////////////////////////
//	Direct3D 11 index buffer implementation
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../../Core/src/ResourceProperty.h"
#include "GraphicsApi/IBuffer.h"

struct ID3D11Buffer;

class cIndexBufferD3D11 : public IIndexBuffer {
public:
	size_t GetByteSize() const;
	eUsage GetUsage() const;
	ID3D11Buffer* GetBufferPointer() const;

	rBuffer GetDesc() override { rBuffer tmp; return tmp; }

	cIndexBufferD3D11(ID3D11Buffer* buffer, size_t size, eUsage usage);
	~cIndexBufferD3D11();
	void Release();

public:
	ID3D11Buffer* const buffer;
private:
	size_t byteSize;
	eUsage usage;
};