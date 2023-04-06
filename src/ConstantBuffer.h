#ifndef _CONSTANT_BUFFER_DIRECTX
#define _CONSTANT_BUFFER_DIRECTX

#if GLACIER_DIRECTX
struct ID3D11Buffer;

class ConstantBuffer
{
public:
	ConstantBuffer(const uint32_t size);
	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;
	ConstantBuffer(ConstantBuffer&& o);
	ConstantBuffer& operator=(ConstantBuffer&& o);
	~ConstantBuffer();

	void Bind(const uint32_t index) const;

	void UpdateData(ID3D11DeviceContext* const devcon, const void* data, const uint32_t data_size) const;

private:
	ID3D11Buffer* buf;
	uint32_t size;
};
#endif

#endif