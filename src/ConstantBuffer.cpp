#include "gpch.h"
#include "ConstantBuffer.h"
#include "DX.h"

#if GLACIER_DIRECTX
ConstantBuffer::ConstantBuffer(const uint32_t size)
	: buf(nullptr), size(size)
{
	assert(size != 0);
	assert(((size + (16 - 1)) & ~(16 - 1)) == size);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	HRESULT hr = DX::GetDevice()->CreateBuffer(&bd, nullptr, &buf);
	assert(SUCCEEDED(hr));
}
ConstantBuffer::ConstantBuffer(ConstantBuffer&& o)
	: buf(o.buf), size(o.size)
{
	o.buf = nullptr;
	o.size = 0;
}
ConstantBuffer& ConstantBuffer::operator=(ConstantBuffer&& o)
{
	buf = o.buf;

	o.buf = nullptr;
	return *this;
}
ConstantBuffer::~ConstantBuffer()
{
	buf->Release();
}

void ConstantBuffer::Bind(const uint32_t index) const
{
	auto devcon = DX::GetDeviceContext();
	devcon->VSSetConstantBuffers(index, 1, &buf);
	devcon->PSSetConstantBuffers(index, 1, &buf);
}

void ConstantBuffer::UpdateData(ID3D11DeviceContext* const devcon, const void* data, const uint32_t data_size) const
{
	assert(data_size >= size);
	devcon->UpdateSubresource(buf, 0, nullptr, data, 0, 0);
}
#endif GLACIER_DIRECTX