#include "gpch.h"
#include "TextureDirectX.h"
#include "DX.h"

#if GLACIER_DIRECTX

TextureDirectX::TextureDirectX(TextureDirectX&& o) noexcept
	: Texture(std::move(o)), 
	mpTextureRV(o.mpTextureRV), mpSampler(o.mpSampler)
{
	o.mpTextureRV = nullptr;
	o.mpSampler = nullptr;
}
TextureDirectX& TextureDirectX::operator=(TextureDirectX&& o)
{
	Texture::operator=(std::move(o));
	mpTextureRV = o.mpTextureRV;
	mpSampler = o.mpSampler;

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
	}
}

void TextureDirectX::Bind(const uint32_t index) const
{
	ID3D11DeviceContext* devcon = DX::GetDeviceContext();
	devcon->PSSetShaderResources(index, 1, &mpTextureRV);
	devcon->PSSetSamplers(index, 1, &mpSampler);
}

void TextureDirectX::SetTextureWrapS(TEXTURE_WRAP wrap)
{
	tex_params.wrap_s = wrap;
	mpSampler->Release();
	load_sampler(DX::GetDevice());
}
void TextureDirectX::SetTextureWrapT(TEXTURE_WRAP wrap)
{
	tex_params.wrap_t = wrap;
	mpSampler->Release();
	load_sampler(DX::GetDevice());
}
void TextureDirectX::SetTextureWrapR(TEXTURE_WRAP wrap)
{
	tex_params.wrap_r = wrap;
	mpSampler->Release();
	load_sampler(DX::GetDevice());
}
void TextureDirectX::SetTextureMinFilter(TEXTURE_MIN_FILTER filter)
{
	tex_params.min_filter = filter;
	mpSampler->Release();
	load_sampler(DX::GetDevice());
}
void TextureDirectX::SetTextureMagFilter(TEXTURE_MAG_FILTER filter)
{
	tex_params.mag_filter = filter;
	mpSampler->Release();
	load_sampler(DX::GetDevice());
}

void TextureDirectX::load_gpu_data()
{
	ID3D11Device* dev = DX::GetDevice();
	HRESULT hr;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = tex_params.type == TEXTURE_TYPE::REGULAR ? 1 : 6;
	texDesc.Format = channels == 4 ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	texDesc.MiscFlags |= tex_params.type == TEXTURE_TYPE::CUBE_MAP ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

	ID3D11Texture2D* tex2D;
	if (texDesc.ArraySize == 6)
	{
		texDesc.MipLevels = 1;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		D3D11_SUBRESOURCE_DATA* subresData = new D3D11_SUBRESOURCE_DATA[texDesc.ArraySize];
		ZeroMemory(subresData, sizeof(D3D11_SUBRESOURCE_DATA) * texDesc.ArraySize);
		for (UINT i = 0; i < texDesc.ArraySize; i++)
		{
			subresData[i].pSysMem = img + i * width * height * channels;
			subresData[i].SysMemPitch = width * channels;
		}
		hr = dev->CreateTexture2D(&texDesc, subresData, &tex2D);
		assert(SUCCEEDED(hr));
		delete[] subresData;
	}
	else
	{
		hr = dev->CreateTexture2D(&texDesc, nullptr, &tex2D);
		assert(SUCCEEDED(hr));
		DX::GetDeviceContext()->UpdateSubresource(tex2D, 0, nullptr, img, width * channels, 0);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shdrResourceDesc;
	ZeroMemory(&shdrResourceDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shdrResourceDesc.Format = texDesc.Format;
	shdrResourceDesc.ViewDimension = tex_params.type == TEXTURE_TYPE::CUBE_MAP ? D3D11_SRV_DIMENSION_TEXTURECUBE : D3D11_SRV_DIMENSION_TEXTURE2D;
	shdrResourceDesc.Texture2D.MostDetailedMip = 0;
	shdrResourceDesc.Texture2D.MipLevels = (UINT)-1;

	hr = dev->CreateShaderResourceView(tex2D, &shdrResourceDesc, &mpTextureRV);
	assert(SUCCEEDED(hr));
	tex2D->Release();

	if(texDesc.ArraySize == 1)
		DX::GetDeviceContext()->GenerateMips(mpTextureRV);
	
	// Create the sampler object
	load_sampler(dev);
}
void TextureDirectX::load_sampler(ID3D11Device* const dev)
{
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
	HRESULT hr = dev->CreateSamplerState(&samp, &mpSampler);
	assert(SUCCEEDED(hr));
}

const std::map<std::pair<TEXTURE_MIN_FILTER, TEXTURE_MAG_FILTER>, D3D11_FILTER> TextureDirectX::filter_map = {
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_MAG_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_MAG_MIP_LINEAR},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_MIPMAP_NEAREST, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_MAG_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_MIPMAP_NEAREST, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::NEAREST_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR_MIPMAP_NEAREST, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR), D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR_MIPMAP_NEAREST, TEXTURE_MAG_FILTER::LINEAR), D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT},
	{std::make_pair(TEXTURE_MIN_FILTER::LINEAR_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::LINEAR), /*D3D11_FILTER_MIN_MAG_MIP_LINEAR*/ D3D11_FILTER_ANISOTROPIC}
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