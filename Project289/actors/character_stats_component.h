#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../tools/mt_random.h"

using namespace std::literals;

class CharacterStatsComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	CharacterStatsComponent();
	virtual ~CharacterStatsComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(float deltaMs) override;

	float GetCurrentHealth();
	void ApplyHealth(float health);

	float GetTotalHealth();
	void SetHealth(float health);

	virtual TiXmlElement* VGenerateXml() override;

protected:
	int m_current_health;
	int m_total_health;
};