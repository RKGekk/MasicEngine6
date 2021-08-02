#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <filesystem>

#include <assimp/material.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "material_color.h"
#include "../tools/error_logger.h"
#include "../tools/string_utility.h"

enum class TextureStorageType {
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

class MaterialTexture {
public:
	MaterialTexture();
	MaterialTexture(ID3D11Device* device, const std::string& fileName);
	MaterialTexture(ID3D11Device* device, const MaterialColor& color, aiTextureType type);
	MaterialTexture(ID3D11Device* device, const MaterialColor* color_data, UINT width, UINT height, aiTextureType type);
	MaterialTexture(ID3D11Device* device, const std::string& fileName, aiTextureType type);
	MaterialTexture(ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type);

	aiTextureType GetType();
	ID3D11ShaderResourceView* GetTextureResourceView();
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();

private:
	void InitializeColorMaterial(ID3D11Device* device, const MaterialColor& color, aiTextureType type);
	void InitializeTextureMaterial(ID3D11Device* device, const MaterialColor* color_data, UINT width, UINT height, aiTextureType type);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_view;

	aiTextureType m_type;
};