////////////////////////////////////////////////////////////////////////////////
//	GraphicsD3D11/src/Shader.h
//	2013.oct.14, Zsiroskenyer Team, Péter Kardos
////////////////////////////////////////////////////////////////////////////////
//	IShaderProgram implementation
#pragma once

#include "../../Core/src/VertexFormat.h"
#include "../../Core/src/common.h"
#include <unordered_map>
#include "GraphicsApi/IShaderProgram.h"

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11SamplerState;
class cGraphicsApiD3D11;

class cShaderProgramD3D11 : public IShaderProgram {
	friend class cGraphicsApiD3D11;
public:
	// sampler info
	struct tSamplerState {
		ID3D11SamplerState* state;
		uint16_t slot;
		
		tSamplerState(ID3D11SamplerState* s, uint16_t slot) :state(s), slot(slot){}
	};

	// ctor/dtor/release
	cShaderProgramD3D11(
		void* vsByteCode, size_t vsByteCodeSize, cVertexFormat vsInputFormat,
		ID3D11VertexShader* vs = nullptr, ID3D11HullShader* hs = nullptr,
		ID3D11DomainShader* ds = nullptr, ID3D11GeometryShader* gs = nullptr, 
		ID3D11PixelShader* ps = nullptr);
	~cShaderProgramD3D11();
	void Release();

	void SetTextureSlotsVS(const std::unordered_map<zsString, uint16_t>& texSlotsVS);
	void SetTextureSlotsPS(const std::unordered_map<zsString, uint16_t>& texSlotsPS);

	void SetSamplerStatesVS(const std::vector<tSamplerState>& states);
	void SetSamplerStatesPS(const std::vector<tSamplerState>& states);

	int GetTextureSlotVS(const zsString& varName) const;
	int GetTextureSlotPS(const zsString& varName) const;

	const std::vector<tSamplerState>& GetSamplerStatesVS() const;
	const std::vector<tSamplerState>& GetSamplerStatesPS() const;

	const ID3D11VertexShader* GetVS() const;
	const ID3D11HullShader* GetHS() const;
	const ID3D11DomainShader* GetDS() const;
	const ID3D11GeometryShader* GetGS() const;
	const ID3D11PixelShader* GetPS() const;

	// byte codes
	size_t GetVSByteCodeSize() const;
	const void* const GetVSByteCode() const;

	// format
	cVertexFormat GetVSInputFormat() const;

	int GetUniformBlockIndex(const char* str) override { return 0; }
	int GetAttributeIndex(const char* str) override { return 0; }
	int GetSamplerIndex(const char* str) override { return 0; }
	int GetRenderTargetIndex(const char* str) override { return 0; }

protected:
	// In Sequence of the Dx pipeline :) 
	ID3D11VertexShader*		vs;
	void*					vsByteCode;
	size_t					vsByteCodeSize;
	cVertexFormat			vsInputFormat;

	ID3D11HullShader*		hs;
	ID3D11DomainShader*		ds;
	ID3D11GeometryShader*	gs;
	ID3D11PixelShader*		ps;


	// Texture name, slot idx
	std::unordered_map<zsString, uint16_t> textureSlotsVS;
	std::unordered_map<zsString, uint16_t> textureSlotsPS;

	// Sampler slot look up
	std::vector<tSamplerState> samplerStatesVS;
	std::vector<tSamplerState> samplerStatesPS;

	// vertex format
	cVertexFormat format;
};