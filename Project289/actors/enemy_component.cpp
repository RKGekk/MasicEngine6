#include "enemy_component.h"
#include "transform_component.h"
#include "particle_component.h"
#include "../tools/memory_utility.h"
#include "../engine/engine.h"

const std::string EnemyComponent::g_Name = "EnemyComponent"s;

const std::string& EnemyComponent::VGetName() const {
	return EnemyComponent::g_Name;
}

EnemyComponent::EnemyComponent() {
	m_target_name = "NoTarget";
	m_force = 100.0f;
}

EnemyComponent::~EnemyComponent() {}

bool EnemyComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pEnemyTarget = pData->FirstChildElement("EnemyTarget");
	if (pEnemyTarget) {
		m_target_name = pEnemyTarget->FirstChild()->Value();
	}

	TiXmlElement* pEnemyForce = pData->FirstChildElement("EnemyForce");
	if (pEnemyForce) {
		std::string sEnemyForce = pEnemyForce->FirstChild()->Value();
		m_force = std::stof(sEnemyForce);
	}

	return true;
}

void EnemyComponent::VPostInit() {}

void EnemyComponent::VUpdate(float deltaMs) {
	using namespace DirectX;
	if (g_pApp->GetGameLogic()->GetState() != BaseEngineState::BGS_Running) { return; }
	StrongActorPtr pTargetActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName(m_target_name));
	if (!pTargetActor) { return; }

	std::shared_ptr<Actor> act = GetOwner();
	std::shared_ptr<TransformComponent> tc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
	std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
	std::shared_ptr<TransformComponent> target_tc = MakeStrongPtr(pTargetActor->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
	XMVECTOR direction = XMVector3Normalize(target_tc->GetPosition() - tc->GetPosition());
	//XMVECTOR direction = XMVector3Normalize(tc->GetPosition() - target_tc->GetPosition());
	if (pc) {
		Particle& particle = pc->VGetParticle();
		//particle.setAwake(true);
		particle.clearAccumulator();
		particle.addForce(direction * m_force);
		particle.integrate(deltaMs);
	}
	else {
		tc->SetPosition3(tc->GetPosition() + direction * deltaMs);
	}
}

const std::string& EnemyComponent::GetTargetName() {
	return m_target_name;
}

TiXmlElement* EnemyComponent::VGenerateXml() {
	return nullptr;
}
