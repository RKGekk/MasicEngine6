#pragma once

#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"

using namespace std::literals;

class SpawnComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	SpawnComponent();
	~SpawnComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(float deltaMs) override;

	virtual TiXmlElement* VGenerateXml() override;

protected:
	float m_radius;
};