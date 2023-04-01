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
	if (mpVertexBuffer)
	{
		mpVertexBuffer->Release();
		mpIndexBuffer->Release();
	}
}

void ModelDirectX::Bind() const
{
	ID3D11DeviceContext* const context = DX::GetDeviceContext();
	context->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelDirectX::load_gpu_data()
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd{ 0 };
	D3D11_SUBRESOURCE_DATA InitData{ 0 };
	ID3D11Device* dev = DX::GetDevice();

	// Vertex buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexTypes::Vertex) * num_vertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = vertex_data.data();
	hr = dev->CreateBuffer(&bd, &InitData, &mpVertexBuffer);
	assert(SUCCEEDED(hr));

	// Index buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexTypes::VertexTriangle) * num_triangles;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = triangles.data();
	hr = dev->CreateBuffer(&bd, &InitData, &mpIndexBuffer);
	assert(SUCCEEDED(hr));
}

#endif