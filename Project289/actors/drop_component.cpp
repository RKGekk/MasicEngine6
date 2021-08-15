#include "drop_component.h"

const std::string DropComponent::g_Name = "DropComponent"s;

const std::string& DropComponent::VGetName() const {
	return DropComponent::g_Name;
}

DropComponent::DropComponent() {
	m_drop_type = "Basic";
}

DropComponent::~DropComponent() {}

bool DropComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pDropType = pData->FirstChildElement("DropType");
	if (pDropType) {
		m_drop_type = pDropType->FirstChild()->Value();
	}

	return true;
}

void DropComponent::VPostInit() {}

void DropComponent::VUpdate(float deltaMs) {}

TiXmlElement* DropComponent::VGenerateXml() {
	return nullptr;
}
