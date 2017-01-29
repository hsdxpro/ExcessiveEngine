////////////////////////////////////////////////////////////////////////////////
//	GraphicsD3D11/src/Shader.cpp
//	2013.oct.14, Zsiroskenyer Team, Péter Kardos
////////////////////////////////////////////////////////////////////////////////
//	Shader related stuff - not yet known

#include "ShaderProgramD3D11.h"
#include <d3d11.h>
#include "..\..\Core\src\VertexFormat.h"


cShaderProgramD3D11::cShaderProgramD3D11(
		void* vsByteCode,
		size_t vsByteCodeSize,
		cVertexFormat vsInputFormat,
		ID3D11VertexShader*		vs /*= nullptr*/,
		ID3D11HullShader*		hs /*= nullptr*/, 
		ID3D11DomainShader*		ds /*= nullptr*/,
		ID3D11GeometryShader*	gs /*= nullptr*/,
		ID3D11PixelShader*		ps /*= nullptr*/)
:vsByteCodeSize(vsByteCodeSize), vsInputFormat(vsInputFormat),
 vs(vs), hs(hs), ds(ds), gs(gs), ps(ps)
{
	assert( vsByteCode );
	this->vsByteCode = new unsigned char[vsByteCodeSize];
	memcpy(this->vsByteCode, vsByteCode, vsByteCodeSize );
}

cShaderProgramD3D11::~cShaderProgramD3D11() {
	if (vs) vs->Release();
	if (hs) hs->Release();
	if (ds) ds->Release();
	if (gs) gs->Release();
	if (ps) ps->Release();

	delete[] vsByteCode;
}

void cShaderProgramD3D11::Release() {
	delete this;
}

void cShaderProgramD3D11::SetTextureSlotsVS(const std::unordered_map<zsString, uint16_t>& texSlotsVS) {
	textureSlotsVS = texSlotsVS;
}

void cShaderProgramD3D11::SetTextureSlotsPS(const std::unordered_map<zsString, uint16_t>& texSlotsPS) {
	textureSlotsPS = texSlotsPS;
}

void cShaderProgramD3D11::SetSamplerStatesVS(const std::vector<cShaderProgramD3D11::tSamplerState>& states) {
	samplerStatesVS = states;
}

void cShaderProgramD3D11::SetSamplerStatesPS(const std::vector<cShaderProgramD3D11::tSamplerState>& states) {
	samplerStatesPS = states;
}

int cShaderProgramD3D11::GetTextureSlotVS(const zsString& varName) const {
	auto i = textureSlotsVS.find(varName);
	return (i != textureSlotsVS.end()) ? i->second : -1;
}

int cShaderProgramD3D11::GetTextureSlotPS(const zsString& varName) const {
	auto i = textureSlotsPS.find(varName);
	return (i != textureSlotsPS.end()) ? i->second : -1;
}

const std::vector<cShaderProgramD3D11::tSamplerState>& cShaderProgramD3D11::GetSamplerStatesVS() const {
	return samplerStatesVS;
}

const std::vector<cShaderProgramD3D11::tSamplerState>& cShaderProgramD3D11::GetSamplerStatesPS() const {
	return samplerStatesPS;
}

const ID3D11VertexShader* cShaderProgramD3D11::GetVS() const {
	return vs;
}

const ID3D11HullShader* cShaderProgramD3D11::GetHS() const {
	return hs;
}

const ID3D11DomainShader* cShaderProgramD3D11::GetDS() const {
	return ds;
}

const ID3D11GeometryShader* cShaderProgramD3D11::GetGS() const {
	return gs;
}

const ID3D11PixelShader* cShaderProgramD3D11::GetPS() const {
	return ps;
}

size_t cShaderProgramD3D11::GetVSByteCodeSize() const {
	return vsByteCodeSize;
}

const void* const cShaderProgramD3D11::GetVSByteCode() const {
	return vsByteCode;
}

cVertexFormat cShaderProgramD3D11::GetVSInputFormat() const {
	return vsInputFormat;
}