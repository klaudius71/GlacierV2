#include "gpch.h"
#include "ModelDirectX.h"
#include "DX.h"

#if GLACIER_DIRECTX

ModelDirectX::ModelDirectX(ModelDirectX&& o) noexcept
	: Model(std::move(o)), mpVertexBuffer(o.mpVertexBuffer), mpIndexBuffer(o.mpIndexBuffer)
{
	o.mpVertexBuffer = 0;
	o.mpIndexBuffer = 0;
}
ModelDirectX& ModelDirectX::operator=(ModelDirectX&& o)
{
	Model::operator=(std::move(o));
	mpVertexBuffer = o.mpVertexBuffer;
	mpIndexBuffer = o.mpIndexBuffer;

	o.mpVertexBuffer = nullptr;
	o.mpIndexBuffer = nullptr;

	return *this;
}
ModelDirectX::~ModelDirectX()
{
}

void ModelDirectX::Bind() const
{
}

void ModelDirectX::load_gpu_data()
{
}

#endif