#ifndef _TEXTURE_DIRECTX
#define _TEXTURE_DIRECTX

#include "Texture.h"

#if GLACIER_DIRECTX

class GLACIER_API TextureDirectX : public Texture
{
public:
	template<typename... Args>
	TextureDirectX(Args&&... args)
		: Texture(std::forward<Args>(args)...)
	{}
	TextureDirectX(const TextureDirectX&) = delete;
	TextureDirectX& operator=(const TextureDirectX&) = delete;
	TextureDirectX(TextureDirectX&& o) noexcept;
	TextureDirectX& operator=(TextureDirectX&& o);
	~TextureDirectX();

	virtual void Bind() const override;

	virtual void SetTextureWrapS(TEXTURE_WRAP wrap) override;
	virtual void SetTextureWrapT(TEXTURE_WRAP wrap) override;
	virtual void SetTextureWrapR(TEXTURE_WRAP wrap) override;
	virtual void SetTextureMinFilter(TEXTURE_MIN_FILTER filter) override;
	virtual void SetTextureMagFilter(TEXTURE_MAG_FILTER filter) override;

private:
	ID3D11Texture2D* mpTexture2D;
	ID3D11ShaderResourceView* mpTextureRV;
	ID3D11SamplerState* mpSampler;

	virtual void load_gpu_data() override;
};

#endif

#endif