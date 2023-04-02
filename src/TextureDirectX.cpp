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

	D3D11_TEXTURE2D_DESC texDesc;
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
	samp.Filter = ConvertToDirectXFilter(tex_params.min_filter, tex_params.mag_filter);
	samp.MaxAnisotropy = 16;
	samp.AddressU = ConvertToDirectXWrapMode(tex_params.wrap_s);
	samp.AddressV = ConvertToDirectXWrapMode(tex_params.wrap_t);
	samp.AddressW = ConvertToDirectXWrapMode(tex_params.wrap_r);
	samp.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samp.MinLOD = 0;
	samp.MaxLOD = D3D11_FLOAT32_MAX;
	hr = dev->CreateSamplerState(&samp, &mpSampler);
	assert(SUCCEEDED(hr));
}

const std::map<std::pair<TEXTURE_MIN_FILTER, TEXTURE_MAG_FILTER>, D3D11_FILTER> TextureDirectX::filter_map = {
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_MAG_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_MIPMAP_NEAREST, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_MAG_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_MIPMAP_NEAREST, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR_MIPMAP_NEAREST, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR_MIPMAP_NEAREST, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_MAG_MIP_LINEAR}
};
const std::unordered_map<TEXTURE_WRAP, D3D11_TEXTURE_ADDRESS_MODE> TextureDirectX::wrap_map = {
	{TEXTURE_WRAP::REPEAT, D3D11_TEXTURE_ADDRESS_WRAP},
	{TEXTURE_WRAP::MIRRORED_REPEAT, D3D11_TEXTURE_ADDRESS_MIRROR},
	{TEXTURE_WRAP::CLAMP_TO_EDGE, D3D11_TEXTURE_ADDRESS_CLAMP},
	{TEXTURE_WRAP::CLAMP_TO_BORDER, D3D11_TEXTURE_ADDRESS_BORDER}
};
D3D11_FILTER TextureDirectX::ConvertToDirectXFilter(TEXTURE_MIN_FILTER min, TEXTURE_MAG_FILTER mag) const
{
	auto it = filter_map.find({ min, mag });
	assert(it != filter_map.cend());
	return it->second;
}
D3D11_TEXTURE_ADDRESS_MODE TextureDirectX::ConvertToDirectXWrapMode(TEXTURE_WRAP wrap) const
{
	auto it = wrap_map.find(wrap);
	assert(it != wrap_map.cend());
	return it->second;
}
#endif