#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>

#include "vertex.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "constant_buffer.h"
#include "constant_buffer_types.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model {
public:
	Model();

	bool Initialize(ID3D11Device* device, const std::string& filePath, ConstantBuffer<CB_VS_VertexShader>& cbvs);
	void Draw(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX worldMatrix, DirectX::FXMMATRIX viewProjectionMatrix);

	bool LoadModel(ID3D11Device* device, const std::string& fileName);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	int GetTextureIndex(aiString* pString);
	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);
	static DirectX::XMMATRIX Inverse(DirectX::CXMMATRIX M);

private:
	void ProcessNode(ID3D11Device* device, aiNode* node, const aiScene* scene, DirectX::FXMMATRIX parentTransformMatrix);
	Mesh ProcessMesh(ID3D11Device* device, aiMesh* mesh, const aiScene* scene, DirectX::FXMMATRIX parentTransformMatrix);
	std::vector<MaterialTexture> LoadMaterialTexures(ID3D11Device* device, aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);

	std::vector<Mesh> m_meshes;

	ConstantBuffer<CB_VS_VertexShader>* m_cbvs = nullptr;

	std::string m_directory = "";
};