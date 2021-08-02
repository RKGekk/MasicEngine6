#include "model.h"

Model::Model() {}

bool Model::Initialize(ID3D11Device* device, const std::string& filePath, ConstantBuffer<CB_VS_VertexShader>& cbvs) {
	m_cbvs = &cbvs;
	
	if (!LoadModel(device, filePath)) { return false; }

	return true;
}

void Model::Draw(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX worldMatrix, DirectX::FXMMATRIX viewProjectionMatrix) {
	for (size_t i = 0; i < m_meshes.size(); ++i) {
		DirectX::XMStoreFloat4x4(&m_cbvs->data.lwvpMatrix, DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(m_meshes[i].GetTransformMatrix(), worldMatrix), viewProjectionMatrix)));
		DirectX::XMStoreFloat4x4(&m_cbvs->data.invWorldMatrix, Inverse(DirectX::XMMatrixMultiply(m_meshes[i].GetTransformMatrix(), worldMatrix)));
		DirectX::XMStoreFloat4x4(&m_cbvs->data.worldMatrix, XMMatrixTranspose(DirectX::XMMatrixMultiply(m_meshes[i].GetTransformMatrix(), worldMatrix)));
		if (!m_cbvs->ApplyChanges(deviceContext)) { return; }
		deviceContext->VSSetConstantBuffers(0u, 1u, m_cbvs->GetAddressOf());
		m_meshes[i].Draw(deviceContext);
	}
}

bool Model::LoadModel(ID3D11Device* device, const std::string& fileName) {
	std::filesystem::path p(fileName);
	m_directory = p.parent_path().string();

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (!pScene) { return false; }

	ProcessNode(device, pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());

	return true;
}

TextureStorageType Model::DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType) {
	if (pMat->GetTextureCount(textureType) == 0) {
		return TextureStorageType::None;
	}

	aiString path;
	pMat->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();
	if (texturePath[0] == '*') {
		if (pScene->mTextures[0]->mHeight == 0) {
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else {
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}

	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str())) {
		if (pTex->mHeight == 0) {
			return TextureStorageType::EmbeddedCompressed;
		}
		else {
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}

	return TextureStorageType::Disk;
}

int Model::GetTextureIndex(aiString* pString) {
	return atoi(&pString->C_Str()[1]);
}

DirectX::XMMATRIX Model::InverseTranspose(DirectX::CXMMATRIX M) {
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));;
}

DirectX::XMMATRIX Model::Inverse(DirectX::CXMMATRIX M) {
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixInverse(&det, A);
}

void Model::ProcessNode(ID3D11Device* device, aiNode* node, const aiScene* scene, DirectX::FXMMATRIX parentTransformMatrix) {
	DirectX::XMMATRIX node_transform_matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixTranspose(DirectX::XMMATRIX(&node->mTransformation.a1)), parentTransformMatrix);

	for (UINT i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(device, mesh, scene, node_transform_matrix));
	}
	for (UINT i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(device, node->mChildren[i], scene, node_transform_matrix);
	}
}

Mesh Model::ProcessMesh(ID3D11Device* device, aiMesh* mesh, const aiScene* scene, DirectX::FXMMATRIX parentTransformMatrix) {
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (UINT i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		vertex.color = {0.7f, 0.1f, 0.6f, 1.0f};
		vertex.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		if (mesh->mTextureCoords[0]) {
			vertex.uv = { (float)mesh->mTextureCoords[0][i].x, (float)mesh->mTextureCoords[0][i].y };
		}
		vertices.push_back(vertex);
	}
	for (UINT i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<MaterialTexture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<MaterialTexture> diffuseTextures = LoadMaterialTexures(device, material, aiTextureType::aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
	//textures.push_back(MaterialTexture(device, MaterialColors::UnloadedTextureColor, aiTextureType::aiTextureType_DIFFUSE));

	return Mesh(device, vertices, indices, textures, parentTransformMatrix);
}

std::vector<MaterialTexture> Model::LoadMaterialTexures(ID3D11Device* device, aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene) {
	std::vector<MaterialTexture> materialTextures;
	TextureStorageType storeType = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);
	if (textureCount == 0) {
		storeType = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (textureType) 	{
			case aiTextureType_DIFFUSE: {
				pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
				if (aiColor.IsBlack()) {
					materialTextures.push_back(MaterialTexture(device, MaterialColors::UnloadedTextureColor, textureType));
				}
				else 	{
					materialTextures.push_back(MaterialTexture(device, MaterialColor(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
				}
			}
			break;
		}
	}
	else {
		for (UINT i = 0u; i < textureCount; ++i) {
			aiString path;
			pMaterial->GetTexture(textureType, i, &path);
			TextureStorageType store_type = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
			switch (store_type) {
				case TextureStorageType::EmbeddedIndexCompressed : {
					int index = GetTextureIndex(&path);
					materialTextures.push_back(MaterialTexture(device, reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData), pScene->mTextures[index]->mWidth, textureType));
				}
				break;
				case TextureStorageType::EmbeddedCompressed : {
					const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
					materialTextures.push_back(MaterialTexture(device, reinterpret_cast<uint8_t*>(pTexture->pcData), pTexture->mWidth, textureType));
				}
				break;
				case TextureStorageType::Disk: {
					std::string fileName = m_directory + "\\" + path.C_Str();
					materialTextures.push_back(MaterialTexture(device, fileName, textureType));
				}
				break;
			}
		}
	}
	
	if (materialTextures.size() == 0u) {
		materialTextures.push_back(MaterialTexture(device, MaterialColors::UnhandeledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
	}

	return materialTextures;
}
