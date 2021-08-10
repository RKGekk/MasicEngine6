#include "character_stats_component.h"

const std::string CharacterStatsComponent::g_Name = "CharacterStatsComponent"s;

const std::string& CharacterStatsComponent::VGetName() const {
	return CharacterStatsComponent::g_Name;
}

CharacterStatsComponent::CharacterStatsComponent() {
	m_current_health = 1.0f;
	m_total_health = 1.0f;
}

CharacterStatsComponent::~CharacterStatsComponent() {}

bool CharacterStatsComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pHealth = pData->FirstChildElement("Health");
	if (pHealth) {
		std::string sHealth = pHealth->FirstChild()->Value();
		m_current_health = std::stoi(sHealth);
	}

	TiXmlElement* pTotalHealth = pData->FirstChildElement("TotalHealth");
	if (pTotalHealth) {
		std::string sTotalHealth = pTotalHealth->FirstChild()->Value();
		m_total_health = std::stoi(sTotalHealth);
	}

	return true;
}

void CharacterStatsComponent::VPostInit() {
}

void CharacterStatsComponent::VUpdate(float deltaMs) {}

float CharacterStatsComponent::GetCurrentHealth() {
	return m_current_health;
}

void CharacterStatsComponent::ApplyHealth(float health) {
	m_current_health += health;
}

float CharacterStatsComponent::GetTotalHealth() {
	return m_total_health;
}

void CharacterStatsComponent::SetHealth(float health) {
	m_total_health = health;
}

TiXmlElement* CharacterStatsComponent::VGenerateXml() {
	return nullptr;
}
