#include "gpch.h"
#include "ShaderDirectX.h"
#include "DX.h"
#include "ConstantBuffer.h"

#if GLACIER_DIRECTX

const D3D11_INPUT_ELEMENT_DESC ShaderDirectX::layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXID", 0, DXGI_FORMAT_R32_UINT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

ShaderDirectX::ShaderDirectX(const std::string& file_name)
	: mpVertexShader(nullptr), mpPixelShader(nullptr), mpVertexLayout(nullptr)
{
	const std::wstring filename(file_name.cbegin(), file_name.cend());
	load_data(filename.c_str(), filename.c_str());
}
ShaderDirectX::ShaderDirectX(const std::string& vertex_shader_file_name, const std::string& pixel_shader_file_name)
{
	const std::wstring vertex_shader_filename(vertex_shader_file_name.cbegin(), vertex_shader_file_name.cend());
	const std::wstring pixel_shader_filename(pixel_shader_file_name.cbegin(), pixel_shader_file_name.cend());
	load_data(vertex_shader_filename.c_str(), pixel_shader_filename.c_str());
}
ShaderDirectX::~ShaderDirectX()
{
	mpVertexShader->Release();
	mpPixelShader->Release();
	mpVertexLayout->Release();
}

void ShaderDirectX::AddConstantBuffer(ConstantBuffer* const buf, const uint32_t index)
{
	assert(buf && index < D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT);
	buffers.emplace_back(buf, index);
}

void ShaderDirectX::Bind() const
{
	auto context = DX::GetDeviceContext();
	context->VSSetShader(mpVertexShader, nullptr, 0);
	context->PSSetShader(mpPixelShader, nullptr, 0);
	context->IASetInputLayout(mpVertexLayout);

	for (const auto& buf : buffers)
		buf.first->Bind(buf.second);
}

void ShaderDirectX::load_data(LPCWSTR const vertex_shader, LPCWSTR const pixel_shader)
{
	ID3D11Device* dev = DX::GetDevice();

	HRESULT hr;
	ID3DBlob* error_blob = nullptr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pVSBlob = nullptr;
	hr = D3DCompileFromFile(vertex_shader, nullptr, nullptr, "VS", vsModel, dwShaderFlags, 0, &pVSBlob, &error_blob);
	if (FAILED(hr))
	{
		PrintShaderError(error_blob);
		assert(false);
	}

	hr = dev->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader);
	assert(SUCCEEDED(hr));

	ID3DBlob* pPSBlob = nullptr;
	hr = D3DCompileFromFile(pixel_shader, nullptr, nullptr, "PS", psModel, dwShaderFlags, 0, &pPSBlob, &error_blob);
	if (FAILED(hr))
	{
		PrintShaderError(error_blob);
		assert(false);
	}

	// Create the pixel shader
	hr = dev->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);
	assert(SUCCEEDED(hr));

	// Create the input layout
	hr = dev->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mpVertexLayout);
	assert(SUCCEEDED(hr));

	pVSBlob->Release();
	pPSBlob->Release();
}

void ShaderDirectX::PrintShaderError(ID3DBlob* const blob)
{
	if (blob)
	{
		OutputDebugStringA(reinterpret_cast<const char*>(blob->GetBufferPointer()));
		blob->Release();
	}
}

#endif