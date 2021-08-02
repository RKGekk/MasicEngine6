#include "shader.h"

bool VertexShader::Initialize(ID3D11Device* device, const ShaderGonfig& cfg) {
	HRESULT hr = D3DReadFileToBlob(
		cfg.shader_name.c_str(),
		m_shader_buffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to load shader: " + cfg.shader_name;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreateVertexShader(
		m_shader_buffer->GetBufferPointer(),
		m_shader_buffer->GetBufferSize(),
		NULL,
		m_shader.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to create vertex shader: " + cfg.shader_name;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	if (cfg.description.size()) {
		hr = device->CreateInputLayout(
			cfg.description.data(),
			cfg.description.size(),
			m_shader_buffer->GetBufferPointer(),
			m_shader_buffer->GetBufferSize(),
			m_input_layout.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLogger::Log(hr, "Failed to create input layout.");
			return false;
		}
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader() {
	return m_shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer() {
	return m_shader_buffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout() {
	return m_input_layout.Get();
}

bool GeometryShader::Initialize(ID3D11Device* device, const ShaderGonfig& cfg) {
	HRESULT hr = D3DReadFileToBlob(
		cfg.shader_name.c_str(),
		m_shader_buffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to load shader: " + cfg.shader_name;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreateGeometryShader(
		m_shader_buffer->GetBufferPointer(),
		m_shader_buffer->GetBufferSize(),
		NULL,
		m_shader.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to create geometry shader: " + cfg.shader_name;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	if (cfg.description.size()) {
		hr = device->CreateInputLayout(
			cfg.description.data(),
			cfg.description.size(),
			m_shader_buffer->GetBufferPointer(),
			m_shader_buffer->GetBufferSize(),
			m_input_layout.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLogger::Log(hr, "Failed to create input layout.");
			return false;
		}
	}

	return true;
}

ID3D11GeometryShader* GeometryShader::GetShader() {
	return m_shader.Get();
}

ID3D10Blob* GeometryShader::GetBuffer() {
	return m_shader_buffer.Get();
}

ID3D11InputLayout* GeometryShader::GetInputLayout() {
	return m_input_layout.Get();
}

ShaderGonfig& ShaderGonfig::set_shader_name(std::wstring shader_name) {
	this->shader_name = std::move(shader_name);
	return *this;
}

ShaderGonfig& ShaderGonfig::set_description(std::vector<D3D11_INPUT_ELEMENT_DESC> description) {
	this->description = std::move(description);
	return *this;
}

bool PixelShader::Initialize(ID3D11Device* device, const ShaderGonfig& cfg) {
	HRESULT hr = D3DReadFileToBlob(
		cfg.shader_name.c_str(),
		m_shader_buffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to load shader: " + cfg.shader_name;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreatePixelShader(
		m_shader_buffer->GetBufferPointer(),
		m_shader_buffer->GetBufferSize(),
		NULL,
		m_shader.GetAddressOf()
	);
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to create pixel shader: " + cfg.shader_name;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	if (cfg.description.size()) {
		hr = device->CreateInputLayout(
			cfg.description.data(),
			cfg.description.size(),
			m_shader_buffer->GetBufferPointer(),
			m_shader_buffer->GetBufferSize(),
			m_input_layout.GetAddressOf()
		);
		if (FAILED(hr)) {
			ErrorLogger::Log(hr, "Failed to create input layout.");
			return false;
		}
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader() {
	return m_shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer() {
	return m_shader_buffer.Get();
}

ID3D11InputLayout* PixelShader::GetInputLayout() {
	return m_input_layout.Get();
}
