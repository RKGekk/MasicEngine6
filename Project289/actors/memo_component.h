#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../tools/mt_random.h"

using namespace std::literals;

class MemoComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	MemoComponent();
	~MemoComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(float deltaMs) override;

	virtual TiXmlElement* VGenerateXml() override;

protected:
	float m_start_time;
	float m_duration;
	float m_distance;
	bool m_condition_present;
	std::string m_condition;
	std::string m_target;
};