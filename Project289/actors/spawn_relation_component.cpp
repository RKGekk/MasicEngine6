#include "spawn_relation_component.h"

const std::string SpawnRelationComponent::g_Name = "SpawnRelationComponent"s;

const std::string& SpawnRelationComponent::VGetName() const {
	return SpawnRelationComponent::g_Name;
}

SpawnRelationComponent::SpawnRelationComponent() {
	m_relate_to_name = "NoRelation";
}

SpawnRelationComponent::~SpawnRelationComponent() {}

bool SpawnRelationComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pRelateTo = pData->FirstChildElement("RelateTo");
	if (pRelateTo) {
		m_relate_to_name = pRelateTo->FirstChild()->Value();
	}

	return true;
}

void SpawnRelationComponent::VPostInit() {}

void SpawnRelationComponent::VUpdate(float deltaMs) {}

TiXmlElement* SpawnRelationComponent::VGenerateXml() {
	return nullptr;
}
