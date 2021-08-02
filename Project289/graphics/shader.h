#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>

#include "../tools/error_logger.h"

class ShaderGonfig {
public:
	ShaderGonfig& set_shader_name(std::wstring shader_name);
	ShaderGonfig& set_description(std::vector<D3D11_INPUT_ELEMENT_DESC> description);
	
	std::wstring shader_name;
	std::vector<D3D11_INPUT_ELEMENT_DESC> description;
};

class VertexShader {
public:
	bool Initialize(ID3D11Device* device, const ShaderGonfig& cfg);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shader_buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_input_layout;
};

class GeometryShader {
public:
	bool Initialize(ID3D11Device* device, const ShaderGonfig& cfg);
	ID3D11GeometryShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shader_buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_input_layout;
};

class PixelShader {
public:
	bool Initialize(ID3D11Device* device, const ShaderGonfig& cfg);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shader_buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_input_layout;
};