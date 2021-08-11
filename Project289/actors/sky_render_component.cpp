#include "sky_render_component.h"
#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"
#include "../tools/geometry_generator.h"
#include "../nodes/d3d_11_sky_pipeline.h"

const std::string SkyRenderComponent::g_Name = "SkyRenderComponent";

const std::string& SkyRenderComponent::VGetName() const {
	return g_Name;
}

SkyRenderComponent::SkyRenderComponent() {
    m_pixelShaderResource = "NoPShader";
    m_vertexShaderResource = "NoVShader";
    m_resource_directory = "NoDirectory";
    m_sky_cube_map_path = "NoSkyCubePath";

    m_sky_sphere_radius = 1000.0f;
    m_sky_sphere_slice_count = 30;
    m_sky_sphere_stack_count = 30;
}

const std::string& SkyRenderComponent::GetPixelShaderResource() {
	return m_pixelShaderResource;
}

const std::string& SkyRenderComponent::GetVertexShaderResource() {
	return m_vertexShaderResource;
}

const std::vector<D3D11_INPUT_ELEMENT_DESC>& SkyRenderComponent::GetLayout() {
	return m_vs_layout;
}

MeshHolder& SkyRenderComponent::GetMesh() {
	return m_mesh;
}

bool SkyRenderComponent::VDelegateInit(TiXmlElement* pData) {
	TiXmlElement* pPixelShader = pData->FirstChildElement("PixelShader");
	if (pPixelShader) {
		m_pixelShaderResource = pPixelShader->FirstChild()->Value();
	}

	TiXmlElement* pVertexShader = pData->FirstChildElement("VertexShader");
	if (pVertexShader) {
		m_vertexShaderResource = pVertexShader->FirstChild()->Value();
	}

	TiXmlElement* pSkySphereRadius = pData->FirstChildElement("SkySphereRadius");
	if (pSkySphereRadius) {
		std::string sSkySphereRadius = pSkySphereRadius->FirstChild()->Value();
		m_sky_sphere_radius = std::stof(sSkySphereRadius);
	}

	TiXmlElement* pSkySphereSliceCount = pData->FirstChildElement("SkySphereSliceCount");
	if (pSkySphereSliceCount) {
		std::string sSkySphereSliceCount = pSkySphereSliceCount->FirstChild()->Value();
		m_sky_sphere_slice_count = std::stoi(sSkySphereSliceCount);
	}

	TiXmlElement* pSkySphereStackCount = pData->FirstChildElement("SkySphereStackCount");
	if (pSkySphereStackCount) {
		std::string sSkySphereStackCount = pSkySphereStackCount->FirstChild()->Value();
		m_sky_sphere_stack_count = std::stoi(sSkySphereStackCount);
	}

	TiXmlElement* pSkyCubeMap = pData->FirstChildElement("SkyCubeMap");
	if (pSkyCubeMap) {
		m_sky_cube_map_path = pSkyCubeMap->FirstChild()->Value();
	}

	m_vs_layout = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

    return true;
}

std::shared_ptr<SceneNode> SkyRenderComponent::VCreateSceneNode() {
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());

	MaterialTexture texture(renderer->GetDevice(), m_sky_cube_map_path, aiTextureType::aiTextureType_BASE_COLOR);
	GeometryGenerator geoGen;
	geoGen.CreateSphere(m_sky_sphere_radius, m_sky_sphere_slice_count, m_sky_sphere_stack_count, m_mesh);
	m_mesh.textures.push_back(std::move(texture));

	//DirectX::XMFLOAT4X4 nodeTransformMatrix4x4f;
	//DirectX::XMStoreFloat4x4(&nodeTransformMatrix4x4f, DirectX::XMMatrixIdentity());

	std::shared_ptr<D3D11SkyPipeline> result = std::make_shared<D3D11SkyPipeline>(*this, DirectX::XMMatrixIdentity(), DirectX::XMMatrixIdentity(), false);
	result->SetAlpha(GetColor().w);
	result->SetRadius(m_sky_sphere_radius);

	return result;
}

void SkyRenderComponent::VCreateInheritedXmlElements(TiXmlElement* pBaseElement) {}
