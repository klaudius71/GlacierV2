#include "gpch.h"
#include "ConstantBuffer.h"
#include "DX.h"

ConstantBuffer::ConstantBuffer(const uint32_t size)
	: buf(nullptr)
{
	assert(size != 0);
	assert((size & (size - 1)) == 0);

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
	: buf(o.buf)
{
	o.buf = nullptr;
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