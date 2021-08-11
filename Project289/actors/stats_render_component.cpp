#include "stats_render_component.h"

#include "transform_component.h"
#include "character_stats_component.h"
#include "orientation_relation_component.h"
#include "../tools/memory_utility.h"
#include "../tools/math_utitity.h"
#include "../nodes/d3d_11_stat_pipeline.h"
#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"

const std::string StatsRenderComponent::g_Name = "StatsRenderComponent"s;
int StatsRenderComponent::m_last_mesh_id = 0;

const std::string& StatsRenderComponent::VGetName() const {
	return StatsRenderComponent::g_Name;
}

StatsRenderComponent::StatsRenderComponent() {

	m_stats_type = "BasicHealth";
	m_height = 0.0f;

	m_fog_color.x = 1.0f;
	m_fog_color.y = 1.0f;
	m_fog_color.z = 1.0f;

	m_fog_start = 10.0f;
	m_fog_range = 100.0f;

	m_gap = 0.0f;
	m_pixel_shader_resource = "NoPixelShader";
	m_vertex_shader_resource = "NoVertexShader";
	m_scale.x = 1.0f;
	m_scale.y = 1.0f;
	m_scale.z = 1.0f;
	m_scale_max = 1.0f;
}

StatsRenderComponent::~StatsRenderComponent() {}

bool StatsRenderComponent::VDelegateInit(TiXmlElement* pData) {

	TiXmlElement* pFullOneHealthMeshObj = pData->FirstChildElement("FullOneHealthMeshObj");
	if (pFullOneHealthMeshObj) {
		m_full_one_health_mesh_path = pFullOneHealthMeshObj->FirstChild()->Value();
	}

	TiXmlElement* pEmptyOneHealthMeshObj = pData->FirstChildElement("EmptyOneHealthMeshObj");
	if (pEmptyOneHealthMeshObj) {
		m_empty_one_health_mesh_path = pEmptyOneHealthMeshObj->FirstChild()->Value();
	}

	TiXmlElement* pStatsType = pData->FirstChildElement("StatsType");
	if (pStatsType) {
		m_stats_type = pStatsType->FirstChild()->Value();
	}

	TiXmlElement* pHeight = pData->FirstChildElement("Height");
	if (pHeight) {
		std::string sHeight = pHeight->FirstChild()->Value();
		m_height = std::stof(sHeight);
	}

	TiXmlElement* pGap = pData->FirstChildElement("Gap");
	if (pGap) {
		std::string sGap = pGap->FirstChild()->Value();
		m_gap = std::stof(sGap);
	}

	TiXmlElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement) {
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;
		pScaleElement->Attribute("x", &x);
		pScaleElement->Attribute("y", &y);
		pScaleElement->Attribute("z", &z);
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}
	else {
		m_scale.x = 1.0f;
		m_scale.y = 1.0f;
		m_scale.z = 1.0f;
	}
	float scale = m_scale.x;
	scale = scale > m_scale.y ? scale : m_scale.y;
	scale = scale > m_scale.z ? scale : m_scale.z;
	m_scale_max = scale;

	TiXmlElement* pPixelShader = pData->FirstChildElement("PixelShader");
	if (pPixelShader) {
		m_pixel_shader_resource = pPixelShader->FirstChild()->Value();
	}

	TiXmlElement* pVertexShader = pData->FirstChildElement("VertexShader");
	if (pVertexShader) {
		m_vertex_shader_resource = pVertexShader->FirstChild()->Value();
	}

	m_vs_layout = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_pSceneFullHealthScene = m_importer1.ReadFile(m_full_one_health_mesh_path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
	m_pSceneEmptyHealthScene = m_importer2.ReadFile(m_empty_one_health_mesh_path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);

	return true;
}

void StatsRenderComponent::VUpdate(float deltaMs) {
	using namespace DirectX;

	if (g_pApp->GetGameLogic()->GetState() != BaseEngineState::BGS_Running) { return; }

	std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
	std::shared_ptr<OrientationRelationComponent> pOrientationRelationComponent = MakeStrongPtr(m_pOwner->GetComponent<OrientationRelationComponent>(OrientationRelationComponent::g_Name));
	std::shared_ptr<CharacterStatsComponent> pCharacterStatsComponent = MakeStrongPtr(m_pOwner->GetComponent<CharacterStatsComponent>(CharacterStatsComponent::g_Name));

	std::shared_ptr<CameraNode> camera = g_pApp->GetHumanView()->VGetCamera();
	XMVECTOR cam_pos_xm = camera->GetPosition();
	//XMVECTOR cam_pos_xm = camera->GetViewPos();
	XMVECTOR our_pos_xm = XMVectorSetW(pTransformComponent->GetPosition(), 0.0f);

	XMVECTOR up_xm = pOrientationRelationComponent->VGetUp();

	//XMVECTOR at_xm = XMVector3Normalize(cam_pos_xm - our_pos_xm);
	XMVECTOR at_xm = XMVector3Normalize(our_pos_xm - cam_pos_xm);
	XMVECTOR right_xm;
	float cos_theta = std::fabsf(XMVectorGetX(XMVector3Dot(up_xm, at_xm)));
	if (XMScalarNearEqual(cos_theta, 1.0f, EPSILON * 100.0f)) {
		at_xm = pOrientationRelationComponent->VGetAt();
		right_xm = pOrientationRelationComponent->VGetRight();
	}
	else {
		right_xm = XMVector3Cross(up_xm, at_xm);
		at_xm = XMVector3Cross(right_xm, up_xm);
	}

	XMMATRIX rot_xm{
		XMVectorSetW(right_xm, 0.0f),
		XMVectorSetW(up_xm, 0.0f),
		XMVectorSetW(at_xm, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
	};

	/*XMMATRIX transform = DirectX::XMMatrixMultiply(
		DirectX::XMMatrixMultiply(
			rot_xm,
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z)
		),
		XMMatrixTranslationFromVector(our_pos_xm + up_xm * m_height)
	);*/

	XMMATRIX transform = DirectX::XMMatrixMultiply(
		rot_xm,
		XMMatrixTranslationFromVector(our_pos_xm + up_xm * m_height)
	);

	DirectX::XMStoreFloat4x4(
		&m_transform,
		transform
	);

	m_root_node->VSetTransform(transform, XMMatrixIdentity(), true);

	int hearts_count = pCharacterStatsComponent->GetTotalHealth();
	int hearts_current = pCharacterStatsComponent->GetCurrentHealth();
	for (int i = 0; i < hearts_count; ++i) {
		if (i < hearts_current) {
			m_full_heart_nodes[i]->SetActive(true);
			m_empty_heart_nodes[i]->SetActive(false);
		}
		else {
			m_full_heart_nodes[i]->SetActive(false);
			m_empty_heart_nodes[i]->SetActive(true);
		}
	}
}

void StatsRenderComponent::UpdateFromScenePrerender(float deltaMs) {
	
}


std::shared_ptr<SceneNode> StatsRenderComponent::VCreateSceneNode() {
	using namespace DirectX;

	std::shared_ptr<CharacterStatsComponent> pCharacterStatsComponent = MakeStrongPtr(m_pOwner->GetComponent<CharacterStatsComponent>(CharacterStatsComponent::g_Name));
	if (!pCharacterStatsComponent) { return std::shared_ptr<SceneNode>(); }

	//const aiScene* pSceneFullHealthScene = importer.ReadFile(m_full_one_health_mesh_path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
	//m_pSceneFullHealthScene = m_importer1.ReadFile(m_full_one_health_mesh_path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
	//m_pSceneEmptyHealthScene = m_importer2.ReadFile(m_empty_one_health_mesh_path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
	
	float full_herat_radius = m_scale_max * 2.0f;
	//float empty_herat_radius = empty_heart_node->GetRadius();
	//float radius = full_herat_radius > empty_herat_radius ? full_herat_radius : empty_herat_radius;
	//float radius = 1.0f / full_herat_radius;
	float radius = full_herat_radius;
	//float radius = 10;

	XMFLOAT4X4 transform;
	XMStoreFloat4x4(&transform, XMMatrixIdentity());
	//m_root_node = std::shared_ptr<SceneNode>(new SceneNode(this, RenderPass::RenderPass_Actor, &transform));
	m_root_node = std::shared_ptr<SceneNode>(new SceneNode(this, RenderPass::RenderPass_Actor, &transform));
	m_root_node->SetSelfTransform(true);

	int hearts_count = pCharacterStatsComponent->GetTotalHealth();
	int hearts_current = pCharacterStatsComponent->GetCurrentHealth();
	if (hearts_current > hearts_count) { hearts_count = hearts_current;	}
	float hearts_count_f = (float)hearts_count;
	float start_pos_x = -1.0f * (hearts_count_f * radius + m_gap * (hearts_count_f - 1.0f)) * 0.5f;
	for (int i = 0; i < hearts_count; ++i) {
		float i_f = (float)i;
		float pos = start_pos_x + m_gap * i_f + radius * i_f;
		std::shared_ptr<SceneNode> n = std::make_shared<SceneNode>(nullptr, RenderPass::RenderPass_Actor, XMMatrixMultiply(XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z), XMMatrixTranslation(pos, 0.0f, 0.0f)), XMMatrixIdentity(), true);
		n->SetSelfTransform(true);

		std::shared_ptr<SceneNode> full_heart_node = std::make_shared<SceneNode>(nullptr, RenderPass::RenderPass_Actor, XMMatrixIdentity(), XMMatrixIdentity(), true);
		ProcessNode(m_pSceneFullHealthScene->mRootNode, m_pSceneFullHealthScene, full_heart_node);

		std::shared_ptr<SceneNode> empty_heart_node = std::make_shared<SceneNode>(nullptr, RenderPass::RenderPass_Actor, XMMatrixIdentity(), XMMatrixIdentity(), true);
		ProcessNode(m_pSceneEmptyHealthScene->mRootNode, m_pSceneEmptyHealthScene, empty_heart_node);

		if (i < hearts_current) {
			full_heart_node->SetActive(true);
			empty_heart_node->SetActive(false);
		}
		else {
			full_heart_node->SetActive(false);
			empty_heart_node->SetActive(true);
		}

		m_full_heart_nodes.push_back(full_heart_node);
		m_empty_heart_nodes.push_back(empty_heart_node);

		n->VAddChild(empty_heart_node);
		n->VAddChild(full_heart_node);
		m_root_node->VAddChild(n);
	}

	return m_root_node;
}

void StatsRenderComponent::ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<SceneNode> parent) {
	using namespace DirectX;
	XMMATRIX node_transform_matrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1));

	for (UINT i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		parent->VAddChild(ProcessMesh(mesh, scene, node_transform_matrix));
	}
	for (UINT i = 0; i < node->mNumChildren; ++i) {
		ActorId owner = this->m_pOwner->GetId();
		std::shared_ptr<SceneNode> n = std::make_shared<SceneNode>(nullptr, RenderPass::RenderPass_Actor, node_transform_matrix, DirectX::XMMatrixIdentity(), true);
		n->SetSelfTransform(true);
		parent->VAddChild(n);
		ProcessNode(node->mChildren[i], scene, n);
	}
}

std::shared_ptr<SceneNode> StatsRenderComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene, DirectX::FXMMATRIX nodeMatrix) {
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	float max_x = m_min; float max_y = m_min; float max_z = m_min;
	float min_x = m_max; float min_y = m_max; float min_z = m_max;
	for (UINT i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		vertex.color = { 0.7f, 0.1f, 0.6f, 1.0f };
		vertex.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		vertex.tg = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
		if (mesh->mTextureCoords[0]) {
			vertex.uv = { (float)mesh->mTextureCoords[0][i].x, (float)mesh->mTextureCoords[0][i].y };
		}

		if (vertex.pos.x > max_x) { max_x = vertex.pos.x; }
		if (vertex.pos.y > max_y) { max_y = vertex.pos.y; }
		if (vertex.pos.z > max_z) { max_z = vertex.pos.z; }

		if (vertex.pos.x < min_x) { min_x = vertex.pos.x; }
		if (vertex.pos.y < min_y) { min_y = vertex.pos.y; }
		if (vertex.pos.z < min_z) { min_z = vertex.pos.z; }

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());

	std::vector<MaterialTexture> textures = LoadMaterialTexures(renderer->GetDevice(), material, aiTextureType::aiTextureType_DIFFUSE, scene);
	if (material->GetTextureCount(aiTextureType::aiTextureType_NORMALS)) {
		std::vector<MaterialTexture> normalTextures = LoadMaterialTexures(renderer->GetDevice(), material, aiTextureType::aiTextureType_NORMALS, scene);
		textures.push_back(normalTextures[0]);
	}
	else {
		std::vector<MaterialTexture> heightTextures = LoadMaterialTexures(renderer->GetDevice(), material, aiTextureType::aiTextureType_HEIGHT, scene);
		textures.push_back(heightTextures[0]);
	}

	DirectX::XMFLOAT4X4 nodeTransformMatrix4x4f;
	DirectX::XMStoreFloat4x4(&nodeTransformMatrix4x4f, nodeMatrix);

	MaterialShader mt;
	aiColor3D aiColor(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
	mt.Diffuse = { aiColor.r, aiColor.g, aiColor.b, 1.0f };
	material->Get(AI_MATKEY_COLOR_AMBIENT, aiColor);
	mt.Ambient = { aiColor.r, aiColor.g, aiColor.b, 1.0f };
	material->Get(AI_MATKEY_COLOR_SPECULAR, aiColor);
	mt.Specular = { aiColor.r, aiColor.g, aiColor.b, 1.0f };

	m_meshes[++m_last_mesh_id] = { std::move(vertices), std::move(indices), std::move(textures), mt, nodeTransformMatrix4x4f };

	std::shared_ptr<D3D11StatPipeline> result = std::make_shared<D3D11StatPipeline>(m_last_mesh_id, *this, nodeMatrix, DirectX::XMMatrixIdentity(), true);
	result->SetAlpha(GetColor().w);
	
	float x = std::fabsf(max_x) > std::fabsf(min_x) ? std::fabsf(max_x) : std::fabsf(min_x);
	float y = std::fabsf(max_y) > std::fabsf(min_y) ? std::fabsf(max_y) : std::fabsf(min_y);
	float z = std::fabsf(max_z) > std::fabsf(min_z) ? std::fabsf(max_z) : std::fabsf(min_z);
	float radius = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(x, y, z, 0.0f)));
	//result->SetRadius(radius * m_scale_max);
	result->SetRadius(radius * m_scale_max * 6.0f);
	//result->SetRadius(radius);
	//result->SetRadius(radius * 3.0f);
	
	return result;
}

void StatsRenderComponent::VCreateInheritedXmlElements(TiXmlElement* pBaseElement) {}

TextureStorageType StatsRenderComponent::DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType) {
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

int StatsRenderComponent::GetTextureIndex(aiString* pString) {
	return atoi(&pString->C_Str()[1]);
}

DirectX::XMMATRIX StatsRenderComponent::InverseTranspose(DirectX::CXMMATRIX M) {
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));;
}

DirectX::XMMATRIX StatsRenderComponent::Inverse(DirectX::CXMMATRIX M) {
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixInverse(&det, A);
}

std::vector<MaterialTexture> StatsRenderComponent::LoadMaterialTexures(ID3D11Device* device, aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene) {
	std::vector<MaterialTexture> materialTextures;
	TextureStorageType storeType = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);
	if (textureCount == 0) {
		storeType = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (textureType) {
		case aiTextureType_DIFFUSE: {
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack()) {
				materialTextures.push_back(MaterialTexture(device, MaterialColors::UnloadedTextureColor, textureType));
			}
			else {
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
			case TextureStorageType::EmbeddedIndexCompressed: {
				int index = GetTextureIndex(&path);
				materialTextures.push_back(MaterialTexture(device, reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData), pScene->mTextures[index]->mWidth, textureType));
			}
			break;
			case TextureStorageType::EmbeddedCompressed: {
				const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
				materialTextures.push_back(MaterialTexture(device, reinterpret_cast<uint8_t*>(pTexture->pcData), pTexture->mWidth, textureType));
			}
			break;
			case TextureStorageType::Disk: {
				std::string fileName = path.C_Str();
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

TiXmlElement* StatsRenderComponent::VGenerateXml() {
	return nullptr;
}

const std::string& StatsRenderComponent::GetPixelShaderResource() {
	return m_pixel_shader_resource;
}

const std::string& StatsRenderComponent::GetVertexShaderResource() {
	return m_vertex_shader_resource;
}

const std::vector<D3D11_INPUT_ELEMENT_DESC>& StatsRenderComponent::GetLayout() {
	return m_vs_layout;
}

float StatsRenderComponent::GetFogStart() {
	return m_fog_start;
}

float StatsRenderComponent::GetFogRange() {
	return m_fog_range;
}

DirectX::XMFLOAT4 StatsRenderComponent::GetFogColor() {
	return m_fog_color;
}

MeshHolder& StatsRenderComponent::GetMesh(int key) {
	return m_meshes[key];
}

const DirectX::XMFLOAT4X4& StatsRenderComponent::GetTransform4x4f() {
	return m_transform;
}

DirectX::XMFLOAT4X4 StatsRenderComponent::GetTransform4x4fT() {
	DirectX::XMMATRIX t = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_transform));
	DirectX::XMFLOAT4X4 t4x4;
	DirectX::XMStoreFloat4x4(&t4x4, t);
	return t4x4;
}

DirectX::XMMATRIX StatsRenderComponent::GetTransform() {
	return DirectX::XMLoadFloat4x4(&m_transform);
}

DirectX::XMFLOAT4X4 StatsRenderComponent::GetInvTransform4x4f() {
	DirectX::XMMATRIX t = GetInvTransform();
	DirectX::XMFLOAT4X4 t4x4;
	DirectX::XMStoreFloat4x4(&t4x4, t);
	return t4x4;
}

DirectX::XMMATRIX StatsRenderComponent::GetInvTransform() {
	return DirectX::XMMatrixInverse(nullptr, GetTransform());
}
