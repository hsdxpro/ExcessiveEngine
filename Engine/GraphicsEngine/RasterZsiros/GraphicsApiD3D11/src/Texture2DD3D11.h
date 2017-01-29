// Texture2DColorD3D11.h By Zsíroskenyér Team 2013.10.20 16:42
#pragma once

#include "../../Core/src/ITexture2D.h"
#include "../../Core/src/common.h"

#include <vector>
#include "GraphicsEngine/ITexture.h"

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
class cGraphicsApiD3D11;

class cTexture2DD3D11 : public ITexture2D {
public:
	cTexture2DD3D11(
		cGraphicsApiD3D11* gApi,
		size_t width,
		size_t height, 
		ID3D11Texture2D* tex,
		ID3D11ShaderResourceView* srv = nullptr,
		ID3D11RenderTargetView* rtv = nullptr,
		ID3D11DepthStencilView* dsv = nullptr,
		ID3D11ShaderResourceView** subsrv = nullptr,
		ID3D11RenderTargetView** subrtv = nullptr,
		ID3D11DepthStencilView** subdsv = nullptr,
		int nSubresource = 0);
	~cTexture2DD3D11();
	void Release() override;

	size_t GetWidth() const override;
	size_t GetHeight() const override;

	cTexture2DD3D11* GetArraySlice(int idx) override;
	const cTexture2DD3D11* GetArraySlice(int idx) const override;

	ID3D11Texture2D* const Get() const;
	ID3D11ShaderResourceView* const GetSRV() const;
	ID3D11RenderTargetView* const GetRTV() const;
	ID3D11DepthStencilView* const GetDSV() const;

	//bool Load(const std::string& file_path) override;

	//void Reset() {}

	//const rTextureGapi& GetDesc() override;

	void GenMipChain() override;

	virtual rTextureGapi GetDesc() const override;

protected:
	size_t width;
	size_t height;

	cGraphicsApiD3D11* gApi;

	ID3D11Texture2D* tex;
	ID3D11ShaderResourceView* srv;
	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsv;

	std::vector<ID3D11ShaderResourceView*> subsrv;
	std::vector<ID3D11RenderTargetView*> subrtv;
	std::vector<ID3D11DepthStencilView*> subdsv;
};