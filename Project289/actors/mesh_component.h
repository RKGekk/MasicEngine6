#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "actor_component.h"
#include "../graphics/vertex.h"

class MeshComponent : public ActorComponent {
public:
	static const std::string g_Name;

	MeshComponent();
	MeshComponent(TiXmlElement* pData);

	virtual bool VInit(TiXmlElement* pData) override;
	virtual const std::string& VGetName() const override;

	const aiScene* GetScene();

	const std::string& GetResourceName();
	const std::string& GetResourceDirecory();

private:
	std::string m_resource_name;
	std::string m_resource_directory;

	Assimp::Importer m_importer;
	const aiScene* m_scene;

	bool Init(TiXmlElement* pData);
	bool LoadModel(const std::filesystem::path& fileName);

	virtual TiXmlElement* VGenerateXml() override;
};