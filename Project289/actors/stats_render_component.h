#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include <assimp/material.h>
#include <assimp/scene.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "actor_component.h"
#include "../tools/mt_random.h"
#include "base_render_component.h"
#include "../graphics/material_texture.h"
#include "../graphics/constant_buffer_types.h"
#include "../graphics/vertex.h"
#include "../graphics/mesh_holder.h"


using namespace std::literals;

class StatsRenderComponent : public BaseRenderComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	StatsRenderComponent();
	virtual ~StatsRenderComponent();

	virtual void VUpdate(float deltaMs) override;
	void UpdateFromScenePrerender(float deltaMs);

	virtual TiXmlElement* VGenerateXml() override;

	const std::string& GetPixelShaderResource();
	const std::string& GetVertexShaderResource();
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetLayout();

	float GetFogStart();
	float GetFogRange();
	DirectX::XMFLOAT4 GetFogColor();

	MeshHolder& GetMesh(int key);
	const DirectX::XMFLOAT4X4& GetTransform4x4f();
	DirectX::XMFLOAT4X4 GetTransform4x4fT();
	DirectX::XMMATRIX GetTransform();
	DirectX::XMFLOAT4X4 GetInvTransform4x4f();
	DirectX::XMMATRIX GetInvTransform();

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual std::shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement) override;

protected:
	static int m_last_mesh_id;

	std::string m_stats_type;
	float m_height;
	float m_gap;
	DirectX::XMFLOAT3 m_scale;
	float m_scale_max;
	DirectX::XMFLOAT4X4 m_transform;
	std::shared_ptr<SceneNode> m_root_node;
	std::vector<std::shared_ptr<SceneNode>> m_full_heart_nodes;
	std::vector<std::shared_ptr<SceneNode>> m_empty_heart_nodes;
	std::shared_ptr<SceneNode> m_danger_node;

	std::string m_full_one_health_mesh_path;
	std::string m_empty_one_health_mesh_path;
	std::string m_danger_mesh_path;
	std::string m_pixel_shader_resource;
	std::string m_vertex_shader_resource;

	Assimp::Importer m_importer1;
	const aiScene* m_pSceneFullHealthScene;

	Assimp::Importer m_importer2;
	const aiScene* m_pSceneEmptyHealthScene;

	Assimp::Importer m_importer3;
	const aiScene* m_pSceneDangerScene;

	static constexpr float m_max = std::numeric_limits<float>::max();
	static constexpr float m_min = std::numeric_limits<float>::min();

	std::unordered_map<int, MeshHolder> m_meshes;

	float m_fog_start;
	float m_fog_range;
	DirectX::XMFLOAT4 m_fog_color;

	std::vector<D3D11_INPUT_ELEMENT_DESC> m_vs_layout;

	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	int GetTextureIndex(aiString* pString);
	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);
	static DirectX::XMMATRIX Inverse(DirectX::CXMMATRIX M);

	std::vector<MaterialTexture> LoadMaterialTexures(ID3D11Device* device, aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);

	void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<SceneNode> parent);
	std::shared_ptr<SceneNode> ProcessMesh(aiMesh* mesh, const aiScene* scene, DirectX::FXMMATRIX nodeMatrix);
};