#include "spawn_relation_component.h"
#include "spawn_component.h"
#include "transform_component.h"
#include "particle_component.h"
#include "../tools/memory_utility.h"
#include "../processes/exec_process.h"
#include "../engine/engine.h"

const std::string SpawnRelationComponent::g_Name = "SpawnRelationComponent"s;

const std::string& SpawnRelationComponent::VGetName() const {
	return SpawnRelationComponent::g_Name;
}

SpawnRelationComponent::SpawnRelationComponent() {
	m_relate_to_name = "NoRelation";
}

SpawnRelationComponent::~SpawnRelationComponent() {}

bool SpawnRelationComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pRelateTo = pData->FirstChildElement("RelateTo");
	if (pRelateTo) {
		m_relate_to_name = pRelateTo->FirstChild()->Value();
	}

	return true;
}

void SpawnRelationComponent::VPostInit() {
	std::shared_ptr<ExecProcess> execOne = std::make_shared<ExecProcess>([this]() {
		std::shared_ptr<Actor> act = GetOwner();
		std::shared_ptr<TransformComponent> tc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
		std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
		StrongActorPtr pRelationActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName(m_relate_to_name));
		std::shared_ptr<SpawnComponent> related_sc = MakeStrongPtr(pRelationActor->GetComponent<SpawnComponent>(ActorComponent::GetIdFromName("SpawnComponent")));
		if (pc) {
			pc->VGetParticle().setPosition3f(related_sc->GetSpawnPosition3f());
		}
		else {
			tc->SetPosition3f(related_sc->GetSpawnPosition3f());
		}

		/*StrongActorPtr pActorLoading = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("loading"));
		std::shared_ptr<MeshRenderComponent> pMeshComponentLoading = MakeStrongPtr(pActorLoading->GetComponent<MeshRenderComponent>(MeshRenderComponent::g_Name));
		pMeshComponentLoading->SetColorA(0.0f);*/
		return true;
	});
	g_pApp->GetGameLogic()->AttachProcess(execOne);
}

void SpawnRelationComponent::VUpdate(float deltaMs) {}

const std::string& SpawnRelationComponent::GetRelatedName() {
	return m_relate_to_name;
}

void SpawnRelationComponent::Respawn() {
	std::shared_ptr<Actor> act = GetOwner();
	std::shared_ptr<TransformComponent> tc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
	std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
	StrongActorPtr pRelationActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName(m_relate_to_name));
	std::shared_ptr<SpawnComponent> related_sc;
	if (pRelationActor) {
		related_sc = MakeStrongPtr(pRelationActor->GetComponent<SpawnComponent>(ActorComponent::GetIdFromName("SpawnComponent")));
	}
	else { return; }
	if (pc) {
		Particle& particle = pc->VGetParticle();
		particle.setAwake(true);
		particle.setPosition3f(related_sc->GetSpawnPosition3f());
	}
	else {
		tc->SetPosition3f(related_sc->GetSpawnPosition3f());
	}
}

TiXmlElement* SpawnRelationComponent::VGenerateXml() {
	return nullptr;
}
