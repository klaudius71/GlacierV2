#ifndef _MODEL_DIRECTX
#define _MODEL_DIRECTX

#include "Model.h"

#if GLACIER_DIRECTX

class GLACIER_API ModelDirectX : public Model
{
public:
	template<typename... Args>
	ModelDirectX(Args&&... args)
		: Model(std::forward<Args>(args)...)
	{
	}
	ModelDirectX(const ModelDirectX&) = delete;
	ModelDirectX& operator=(const ModelDirectX&) = delete;
	ModelDirectX(ModelDirectX&& o) noexcept;
	ModelDirectX& operator=(ModelDirectX&& o);
	~ModelDirectX();

	virtual void Bind() const override;

private:
	const UINT stride = sizeof(VertexTypes::Vertex);
	const UINT offset = 0;

	ID3D11Buffer* mpVertexBuffer;
	ID3D11Buffer* mpIndexBuffer;

	virtual void load_gpu_data() override;
};

#endif

#endif