#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"

using namespace std::literals;

class SpawnRelationComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	SpawnRelationComponent();
	~SpawnRelationComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(float deltaMs) override;

	const std::string& GetRelatedName();
	void Respawn();

	virtual TiXmlElement* VGenerateXml() override;

protected:
	std::string m_relate_to_name;
};