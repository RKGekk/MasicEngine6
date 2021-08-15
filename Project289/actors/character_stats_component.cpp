#include "character_stats_component.h"
#include "../engine/engine.h"
#include "transform_component.h"
#include "enemy_component.h"
#include "../tools/memory_utility.h"

#include <algorithm>
#include <functional>
#include <cctype>

const std::string CharacterStatsComponent::g_Name = "CharacterStatsComponent"s;

const std::string& CharacterStatsComponent::VGetName() const {
	return CharacterStatsComponent::g_Name;
}

CharacterStatsComponent::CharacterStatsComponent() {
	m_current_health = 1.0f;
	m_total_health = 1.0f;
	m_danger = false;
	m_danger_radius = 1.0f;
	m_danger_is_defined = false;
	m_drops_counter = 0;
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

	TiXmlElement* pDanger = pData->FirstChildElement("Danger");
	if (pDanger) {
		m_danger_is_defined = true;
		std::string sDanger = pDanger->FirstChild()->Value();
		std::for_each(sDanger.begin(), sDanger.end(), [](char& c) { c = ::toupper(c); });
		if (sDanger == "TRUE" || sDanger == "1") {
			m_danger = true;
		}
		else {
			m_danger = false;
		}
	}

	TiXmlElement* pDangerRadius = pData->FirstChildElement("DangerRadius");
	if (pDangerRadius) {
		std::string sDangerRadius = pDangerRadius->FirstChild()->Value();
		m_danger_radius = std::stof(sDangerRadius);
	}

	return true;
}

void CharacterStatsComponent::VPostInit() {
}

void CharacterStatsComponent::VUpdate(float deltaMs) {
	using namespace DirectX;
	if (!m_danger_is_defined) { return; }
	//std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
	unsigned int enemyComponentComponentId = EnemyComponent::GetIdFromName("EnemyComponent");
	BaseEngineLogic* el = g_pApp->GetGameLogic();
	if (el->GetState() != BaseEngineState::BGS_Running) { return; }
	if (el->VCheckActorsExistByComponent(enemyComponentComponentId)) {
		//std::shared_ptr<TransformComponent> pSelfTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
		const std::unordered_set<ActorId>& enemy_actids = el->VGetActorsByComponent(enemyComponentComponentId);
		bool ex = false;
		for (ActorId enemy_actid : enemy_actids) {
			StrongActorPtr pEnemy_Act = MakeStrongPtr(el->VGetActor(enemy_actid));
			if (!pEnemy_Act) { continue; }
			std::shared_ptr<EnemyComponent> pEnemyComponent = MakeStrongPtr(pEnemy_Act->GetComponent<EnemyComponent>(EnemyComponent::g_Name));
			StrongActorPtr pTarget_Act = MakeStrongPtr(el->VGetActorByName(pEnemyComponent->GetTargetName()));
			if (!pTarget_Act) { continue; }
			std::shared_ptr<TransformComponent> pTargetTransformComponent = MakeStrongPtr(pTarget_Act->GetComponent<TransformComponent>(TransformComponent::g_Name));
			std::shared_ptr<TransformComponent> pEnemyTransformComponent = MakeStrongPtr(pEnemy_Act->GetComponent<TransformComponent>(TransformComponent::g_Name));
			//XMVECTOR self_pos = pSelfTransformComponent->GetPosition();
			XMVECTOR enemy_pos = pEnemyTransformComponent->GetPosition();
			XMVECTOR target_pos = pTargetTransformComponent->GetPosition();
			//float distance = XMVectorGetX(XMVector3Length(self_pos - enemy_pos));
			float distance = XMVectorGetX(XMVector3Length(target_pos - enemy_pos));
			if (distance <= m_danger_radius) {
				ex = true;
				break;
			}
		}
		m_danger = ex;
	}
	else {
		m_danger = false;
	}
}

int CharacterStatsComponent::GetCurrentHealth() {
	return m_current_health;
}

void CharacterStatsComponent::ApplyHealth(int health) {
	m_current_health += health;
}

int CharacterStatsComponent::GetTotalHealth() {
	return m_total_health;
}

void CharacterStatsComponent::SetHealth(int health) {
	m_total_health = health;
}

bool CharacterStatsComponent::GetDanger() {
	return m_danger;
}

int CharacterStatsComponent::GetDrops() {
	return m_drops_counter;
}

void CharacterStatsComponent::ApplyDrops(int drops) {
	m_drops_counter += drops;
}

TiXmlElement* CharacterStatsComponent::VGenerateXml() {
	return nullptr;
}
