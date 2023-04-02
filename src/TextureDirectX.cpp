#include "gpch.h"
#include "TextureDirectX.h"
#include "DX.h"

#if GLACIER_DIRECTX
TextureDirectX::TextureDirectX(TextureDirectX&& o) noexcept
	: Texture(std::move(o)), 
	mpTexture2D(o.mpTexture2D), mpTextureRV(o.mpTextureRV), mpSampler(o.mpSampler)
{
	o.mpTexture2D = nullptr;
	o.mpTextureRV = nullptr;
	o.mpSampler = nullptr;
}
TextureDirectX& TextureDirectX::operator=(TextureDirectX&& o)
{
	Texture::operator=(std::move(o));
	mpTexture2D = o.mpTexture2D;
	mpTextureRV = o.mpTextureRV;
	mpSampler = o.mpSampler;

	o.mpTexture2D = nullptr;
	o.mpTextureRV = nullptr;
	o.mpSampler = nullptr;

	return *this;
}
TextureDirectX::~TextureDirectX()
{
	if (mpTextureRV)
	{
		mpSampler->Release();
		mpTextureRV->Release();
		mpTexture2D->Release();
	}
}

void TextureDirectX::Bind() const
{
	ID3D11DeviceContext* devcon = DX::GetDeviceContext();
	devcon->PSSetShaderResources(0, 1, &mpTextureRV);
	devcon->PSSetSamplers(0, 1, &mpSampler);
}

void TextureDirectX::SetTextureWrapS(TEXTURE_WRAP wrap)
{
}
void TextureDirectX::SetTextureWrapT(TEXTURE_WRAP wrap)
{
}
void TextureDirectX::SetTextureWrapR(TEXTURE_WRAP wrap)
{
}
void TextureDirectX::SetTextureMinFilter(TEXTURE_MIN_FILTER filter)
{
}
void TextureDirectX::SetTextureMagFilter(TEXTURE_MAG_FILTER filter)
{
}

void TextureDirectX::load_gpu_data()
{
	ID3D11Device* dev = DX::GetDevice();
	HRESULT hr;

	D3D11_TEXTURE2D_DESC texDesc{ 0 };
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = channels == 4 ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	D3D11_SUBRESOURCE_DATA subresData;
	ZeroMemory(&subresData, sizeof(D3D11_SUBRESOURCE_DATA));
	subresData.pSysMem = img;
	subresData.SysMemPitch = width * channels;

	hr = dev->CreateTexture2D(&texDesc, &subresData, &mpTexture2D);
	assert(SUCCEEDED(hr));	

	hr = dev->CreateShaderResourceView(mpTexture2D, nullptr, &mpTextureRV);
	assert(SUCCEEDED(hr));

	//DX::GetDeviceContext()->GenerateMips(mpTextureRV);
	
	// Temp settings
	D3D11_SAMPLER_DESC samp;
	ZeroMemory(&samp, sizeof(D3D11_SAMPLER_DESC));
	samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samp.MaxAnisotropy = 1;
	samp.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samp.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samp.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samp.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samp.MinLOD = 0;
	samp.MaxLOD = D3D11_FLOAT32_MAX;
	hr = dev->CreateSamplerState(&samp, &mpSampler);
	assert(SUCCEEDED(hr));
}
#endif