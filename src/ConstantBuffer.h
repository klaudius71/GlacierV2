#ifndef _CONSTANT_BUFFER_DIRECTX
#define _CONSTANT_BUFFER_DIRECTX

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

	void Bind(const uint32_t index);

private:
	ID3D11Buffer* buf;
};

#endif