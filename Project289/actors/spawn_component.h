#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../tools/mt_random.h"

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

	DirectX::XMVECTOR GetSpawnPosition();
	DirectX::XMFLOAT4 GetSpawnPosition4f();
	DirectX::XMFLOAT3 GetSpawnPosition3f();

	virtual TiXmlElement* VGenerateXml() override;

protected:
	float m_radius;
	MTRandom m_rnd;
};