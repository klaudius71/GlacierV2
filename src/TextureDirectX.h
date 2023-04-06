#ifndef _TEXTURE_DIRECTX
#define _TEXTURE_DIRECTX

#include "Texture.h"

#if GLACIER_DIRECTX

class GLACIER_API TextureDirectX : public Texture
{
public:
	template<typename... Args>
	TextureDirectX(Args&&... args)
		: Texture(std::forward<Args>(args)...), mpTextureRV(nullptr), mpSampler(nullptr)
	{
		load_gpu_data();
	}
	TextureDirectX(const TextureDirectX&) = delete;
	TextureDirectX& operator=(const TextureDirectX&) = delete;
	TextureDirectX(TextureDirectX&& o) noexcept;
	TextureDirectX& operator=(TextureDirectX&& o);
	~TextureDirectX();

	virtual void Bind(const uint32_t index) const override;

	virtual void SetTextureWrapS(TEXTURE_WRAP wrap) override;
	virtual void SetTextureWrapT(TEXTURE_WRAP wrap) override;
	virtual void SetTextureWrapR(TEXTURE_WRAP wrap) override;
	virtual void SetTextureMinFilter(TEXTURE_MIN_FILTER filter) override;
	virtual void SetTextureMagFilter(TEXTURE_MAG_FILTER filter) override;

private:
	ID3D11ShaderResourceView* mpTextureRV;
	ID3D11SamplerState* mpSampler;

	virtual void load_gpu_data() override;

	static const std::map<std::pair<TEXTURE_MIN_FILTER, TEXTURE_MAG_FILTER>, D3D11_FILTER> filter_map;
	static const std::unordered_map<TEXTURE_WRAP, D3D11_TEXTURE_ADDRESS_MODE> wrap_map;
	inline D3D11_FILTER ConvertToDirectXFilter(TEXTURE_MIN_FILTER min, TEXTURE_MAG_FILTER mag) const;
	inline D3D11_TEXTURE_ADDRESS_MODE ConvertToDirectXWrapMode(TEXTURE_WRAP wrap) const;
};

#endif

#endif