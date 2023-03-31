#ifndef _SHADER_DIRECTX
#define _SHADER_DIRECTX

#include "Shader.h"

#define vsModel (LPCSTR)"vs_4_0"
#define psModel (LPCSTR)"ps_4_0"

#if GLACIER_DIRECTX

class ShaderDirectX : public Shader
{
public:
	ShaderDirectX(const std::string& file_name);
	ShaderDirectX(const ShaderDirectX&) = delete;
	ShaderDirectX& operator=(const ShaderDirectX&) = delete;
	ShaderDirectX(ShaderDirectX&&) = delete;
	ShaderDirectX& operator=(ShaderDirectX&&) = delete;
	~ShaderDirectX();

	void Bind() const;

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpVertexLayout;
	ID3DBlob* pVSBlob;
	ID3DBlob* pPSBlob;

	void PrintShaderError(ID3DBlob* const blob);
};

#endif

#endif