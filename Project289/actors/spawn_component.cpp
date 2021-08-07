#include "spawn_component.h"

const std::string SpawnComponent::g_Name = "SpawnComponent"s;

const std::string& SpawnComponent::VGetName() const {
	return SpawnComponent::g_Name;
}

SpawnComponent::SpawnComponent() : m_radius(1.0f) {}

SpawnComponent::~SpawnComponent() {}

bool SpawnComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pSpawnRadius = pData->FirstChildElement("SpawnRadius");
	if (pSpawnRadius) {
		std::string sSpawnRadius = pSpawnRadius->FirstChild()->Value();
		m_radius = std::stof(sSpawnRadius);
	}

	return true;
}

void SpawnComponent::VPostInit() {}

void SpawnComponent::VUpdate(float deltaMs) {}

TiXmlElement* SpawnComponent::VGenerateXml() {
	return nullptr;
}
