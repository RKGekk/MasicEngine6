#include "spawn_component.h"
#include "transform_component.h"
#include "../tools/mt_random.h"
#include "../tools/memory_utility.h"

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

DirectX::XMVECTOR SpawnComponent::GetSpawnPosition() {
	std::shared_ptr<Actor> act = GetOwner();
	std::shared_ptr<TransformComponent> tc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
	MTRandom rnd;
	float x = rnd.Random(); float y = rnd.Random(); float z = rnd.Random();
	DirectX::XMFLOAT3 pos = tc->GetPosition3f();
	return DirectX::XMVectorSet(x * m_radius + pos.x, y * m_radius + pos.y, z * m_radius + pos.z, 1.0f);
}

DirectX::XMFLOAT4 SpawnComponent::GetSpawnPosition4f() {
	DirectX::XMFLOAT4 res;
	DirectX::XMStoreFloat4(&res, GetSpawnPosition());
	return res;
}

DirectX::XMFLOAT3 SpawnComponent::GetSpawnPosition3f() {
	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(&res, GetSpawnPosition());
	return res;
}

TiXmlElement* SpawnComponent::VGenerateXml() {
	return nullptr;
}
