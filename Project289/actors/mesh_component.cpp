#include "mesh_component.h"

const std::string MeshComponent::g_Name = "MeshComponent";

MeshComponent::MeshComponent() {}

MeshComponent::MeshComponent(TiXmlElement* pData) {
    Init(pData);
}

bool MeshComponent::VInit(TiXmlElement* pData) {
    return Init(pData);
}

const std::string& MeshComponent::VGetName() const {
    return MeshComponent::g_Name;
}

const aiScene* MeshComponent::GetScene() {
	return m_scene;
}

const std::string& MeshComponent::GetResourceName() {
	return m_resource_name;
}

const std::string& MeshComponent::GetResourceDirecory() {
	return m_resource_directory;
}

bool MeshComponent::Init(TiXmlElement* pData) {
	TiXmlElement* pObjElement = pData->FirstChildElement("Obj");
	std::string fileName(pObjElement->GetText());
	if (fileName.empty()) {
		return false;
	}
	m_resource_name = fileName;
	std::filesystem::path p(fileName);
	m_resource_directory = p.parent_path().string();
	if (fileName == "NoObj") {
		return true;
	}
	return LoadModel(p);
}

bool MeshComponent::LoadModel(const std::filesystem::path& fileName) {
    m_scene = m_importer.ReadFile(fileName.string(), aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
	//m_scene = m_importer.ReadFile(fileName.string(), aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_MakeLeftHanded | aiProcess_FlipUVs);
	//m_scene = m_importer.ReadFile(fileName.string(), aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_MakeLeftHanded | aiProcess_FlipUVs);
    if (!m_scene) { return false; }

    return true;
}

TiXmlElement* MeshComponent::VGenerateXml() {
	return nullptr;
}
