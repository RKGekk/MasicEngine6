#include "material_texture.h"

MaterialTexture::MaterialTexture() : m_type(aiTextureType::aiTextureType_UNKNOWN) {}

MaterialTexture::MaterialTexture(ID3D11Device* device, const std::string& fileName) : MaterialTexture(device, fileName, aiTextureType::aiTextureType_UNKNOWN){}

MaterialTexture::MaterialTexture(ID3D11Device* device, const MaterialColor& color, aiTextureType type) {
	InitializeColorMaterial(device, color, type);
}

MaterialTexture::MaterialTexture(ID3D11Device* device, const MaterialColor* color_data, UINT width, UINT height, aiTextureType type) {
	InitializeTextureMaterial(device, color_data, width, height, type);
}

MaterialTexture::MaterialTexture(ID3D11Device* device, const std::string& fileName, aiTextureType type) : m_type(type) {

	std::filesystem::path p(fileName);
	std::error_code err;
	std::filesystem::file_status status = std::filesystem::status(p, err);
	if (!std::filesystem::is_regular_file(status)) {
		InitializeColorMaterial(device, MaterialColors::UnloadedTextureColor, type);
	}

	if (p.extension().string() == "DDS") {
		HRESULT hr = DirectX::CreateDDSTextureFromFile(device, s2w(fileName).c_str(), reinterpret_cast<ID3D11Resource**>(m_texture.GetAddressOf()), m_texture_view.GetAddressOf());
		if (FAILED(hr)) {
			InitializeColorMaterial(device, MaterialColors::UnloadedTextureColor, type);
		}
	}
	else {
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, s2w(fileName).c_str(), reinterpret_cast<ID3D11Resource**>(m_texture.GetAddressOf()), m_texture_view.GetAddressOf());
		if (FAILED(hr)) {
			InitializeColorMaterial(device, MaterialColors::UnloadedTextureColor, type);
		}
	}
}

MaterialTexture::MaterialTexture(ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type) {
	m_type = type;
	HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, reinterpret_cast<ID3D11Resource**>(m_texture.GetAddressOf()), m_texture_view.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed create texture from memory.");
}

aiTextureType MaterialTexture::GetType() {
	return m_type;
}

ID3D11ShaderResourceView* MaterialTexture::GetTextureResourceView() {
	return m_texture_view.Get();
}

ID3D11ShaderResourceView** MaterialTexture::GetTextureResourceViewAddress() {
	return m_texture_view.GetAddressOf();
}

void MaterialTexture::InitializeColorMaterial(ID3D11Device* device, const MaterialColor& color, aiTextureType type) {
	InitializeTextureMaterial(device, &color, 1, 1, type);
}

void MaterialTexture::InitializeTextureMaterial(ID3D11Device* device, const MaterialColor* color_data, UINT width, UINT height, aiTextureType type) {
	m_type = type;
	if (m_texture != nullptr) { m_texture.Reset(); }
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA init_data;
	ZeroMemory(&init_data, sizeof(init_data));
	init_data.pSysMem = color_data;
	init_data.SysMemPitch = width * sizeof(MaterialColor);
	HRESULT hr = device->CreateTexture2D(&textureDesc, &init_data, m_texture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize texture from color data.");

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8G8B8A8_UNORM);
	hr = device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_texture_view.ReleaseAndGetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from color data.");
}