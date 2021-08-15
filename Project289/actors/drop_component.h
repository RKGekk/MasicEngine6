#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../tools/mt_random.h"

using namespace std::literals;

class DropComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	DropComponent();
	~DropComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(float deltaMs) override;

	virtual TiXmlElement* VGenerateXml() override;

protected:
	std::string m_drop_type;
};