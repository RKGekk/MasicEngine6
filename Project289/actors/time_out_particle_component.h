#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../graphics/vertex.h"

class TimeOutParticleComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	TimeOutParticleComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VUpdate(float deltaMs) override;

private:
	float m_sleep_until;
	bool m_overdue;

	virtual TiXmlElement* VGenerateXml() override;
};