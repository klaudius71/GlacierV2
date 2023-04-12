#ifndef _SHADER_DIRECTX
#define _SHADER_DIRECTX

#include "Shader.h"

#define vsModel (LPCSTR)"vs_4_0"
#define psModel (LPCSTR)"ps_4_0"

#if GLACIER_DIRECTX

class ConstantBuffer;

class ShaderDirectX : public Shader
{
public:
	ShaderDirectX(const std::string& file_name);
	ShaderDirectX(const std::string& vertex_shader_file_name, const std::string& pixel_shader_file_name);
	ShaderDirectX(const ShaderDirectX&) = delete;
	ShaderDirectX& operator=(const ShaderDirectX&) = delete;
	ShaderDirectX(ShaderDirectX&&) = delete;
	ShaderDirectX& operator=(ShaderDirectX&&) = delete;
	~ShaderDirectX();

	void AddConstantBuffer(ConstantBuffer* const buf, const uint32_t index);

	void Bind() const;

private:
	void load_data(LPCWSTR const vertex_shader, LPCWSTR const pixel_shader);

	static const D3D11_INPUT_ELEMENT_DESC layout[];

	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpVertexLayout;
	std::vector<std::pair<ConstantBuffer*, uint32_t>> buffers;

	void PrintShaderError(ID3DBlob* const blob);
};

#endif

#endif