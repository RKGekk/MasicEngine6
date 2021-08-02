#include "x_physics.h"
#include "../tools/memory_utility.h"
#include "engine.h"
#include "../actors/particle_component.h"
#include "../actors/particle_contact_generator_component.h"
#include "../actors/particle_force_generator_component.h"
#include "../events/evt_data_destroy_particle_component.h"
#include "../events/evt_data_destroy_particle_contact_generator.h"
#include "../events/evt_data_destroy_particle_force_generator.h"
#include "../events/evt_data_new_particle_component.h"
#include "../events/evt_data_new_particle_contact_generator.h"
#include "../events/evt_data_new_particle_force_generator.h"
#include "../events/evt_data_move_actor.h"
#include "../events/evt_data_destroy_actor.h"
#include "../events/i_event_manager.h"
#include "../actors/transform_component.h"

#include <utility>

XPhysics::XPhysics() : m_particle_world(1024, 16) {
	m_particle_array.reserve(128);
	m_contact_generators.reserve(16);

	IEventManager* pEventMgr = IEventManager::Get();
	pEventMgr->VAddListener({ connect_arg<&XPhysics::NewParticleComponentDelegate>, this }, EvtData_New_Particle_Component::sk_EventType);
	pEventMgr->VAddListener({ connect_arg<&XPhysics::DestroyActorDelegate>, this }, EvtData_Destroy_Actor::sk_EventType);
	pEventMgr->VAddListener({ connect_arg<&XPhysics::NewParticleContactGeneratorComponentDelegate>, this }, EvtData_New_Particle_Contact_Generator::sk_EventType);
	pEventMgr->VAddListener({ connect_arg<&XPhysics::NewParticleForceGeneratorComponentDelegate>, this }, EvtData_New_Particle_Force_Generator::sk_EventType);
}

XPhysics::~XPhysics() {
	IEventManager* pEventMgr = IEventManager::Get();
	pEventMgr->VRemoveListener({ connect_arg<&XPhysics::NewParticleComponentDelegate>, this }, EvtData_New_Particle_Component::sk_EventType);
	pEventMgr->VRemoveListener({ connect_arg<&XPhysics::DestroyActorDelegate>, this }, EvtData_Destroy_Actor::sk_EventType);
	pEventMgr->VRemoveListener({ connect_arg<&XPhysics::NewParticleContactGeneratorComponentDelegate>, this }, EvtData_New_Particle_Contact_Generator::sk_EventType);
	pEventMgr->VRemoveListener({ connect_arg<&XPhysics::NewParticleForceGeneratorComponentDelegate>, this }, EvtData_New_Particle_Force_Generator::sk_EventType);
}

bool XPhysics::VInitialize() {
	/*size_t psz = m_particle_array.size();
	for (unsigned i = 0; i < psz; i++) {
		m_particle_world.getParticles().push_back(m_particle_array[i]);
	}
	
	for (auto &[key, val] : m_contact_generators) {
		m_particle_world.getContactGenerators().push_back(val.get());
	}*/
	return true;
}

void XPhysics::VOnUpdate(float deltaSeconds) {
	m_particle_world.startFrame();
	m_particle_world.runPhysics(deltaSeconds);
}

void XPhysics::VSyncVisibleScene() {
	using namespace DirectX;
	for (const auto& [key, val] : m_particle_array) {
		StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(key));
		std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
		XMVECTOR newPos = val->getPosition();
		XMVECTOR oldPos = pTransformComponent->GetPosition();
		if (!XMVector3NearEqual(newPos, oldPos, XMVectorReplicate(EPSILON))) {
			pTransformComponent->SetPosition3(newPos);

			std::shared_ptr<EvtData_Move_Actor> pEvent(new EvtData_Move_Actor(key, pTransformComponent->GetTransform4x4f()));
			IEventManager::Get()->VQueueEvent(pEvent);
		}
	}
}

void XPhysics::VAddParticleActor(const ActorId actorId) {
	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(actorId));
	std::shared_ptr<ParticleComponent> pParticleComponent = MakeStrongPtr(pActor->GetComponent<ParticleComponent>(ParticleComponent::g_Name));

	Particle* pParticle = pParticleComponent->VGetParticlePtr();
	m_particle_world.getParticles().push_back(pParticle);
	m_particle_array.emplace(std::make_pair(actorId, pParticle));
}

std::vector<Particle*>& XPhysics::VGetParticles() {
	return m_particle_world.getParticles();
}

void XPhysics::VRemoveParticle(Particle* p) {
	auto it = std::find_if(m_particle_array.begin(), m_particle_array.end(), [p](const std::pair<ActorId, Particle*>& t) -> bool { return t.second == p; });
	ActorId act = (*it).first;
	m_particle_array.erase(act);
	ParticleWorld::Particles& particles = m_particle_world.getParticles();
	particles.erase(std::find(particles.begin(), particles.end(), p));
}

void XPhysics::VRemoveActorParticle(ActorId id) {
	Particle* pParticle = m_particle_array[id];
	ParticleWorld::Particles& particles = m_particle_world.getParticles();
	particles.erase(std::find(particles.begin(), particles.end(), pParticle));

	m_particle_array.erase(id);
}

void XPhysics::VAddContactGenerator(ActorId id) {
	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(id));
	std::shared_ptr<ParticleContactGeneratorComponent> pParticleContactGenegator = MakeStrongPtr(pActor->GetComponent<ParticleContactGeneratorComponent>(ParticleContactGeneratorComponent::g_Name));
	
	std::shared_ptr<ParticleContactGenerator> pCg = pParticleContactGenegator->VGetContactGenerator();
	m_particle_world.getContactGenerators().push_back(pCg.get());
	m_contact_generators.emplace(std::make_pair(id, pCg));
}

void XPhysics::VRemoveContactGenerator(ActorId id) {
	std::shared_ptr<ParticleContactGenerator>& cg = m_contact_generators[id];
	ParticleContactGenerator* pCg = cg.get();
	ParticleWorld::ContactGenerators& cg_array = m_particle_world.getContactGenerators();
	cg_array.erase(std::find(cg_array.begin(), cg_array.end(), pCg));
	m_contact_generators.erase(id);
}

void XPhysics::VAddForceGenerator(ActorId id) {
	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(id));
	std::shared_ptr<ParticleForceGeneratorComponent> pParticleForceGenegator = MakeStrongPtr(pActor->GetComponent<ParticleForceGeneratorComponent>(ParticleForceGeneratorComponent::g_Name));

	std::shared_ptr<ParticleForceGenerator> pFg = pParticleForceGenegator->VGetForceGenerator();
	for (Particle* pParticle : m_particle_world.getParticles()) {
		m_particle_world.getForceRegistry().add(pParticle, pFg.get());
	}
	m_force_generators.emplace(std::make_pair(id, pFg));
}

void XPhysics::VRemoveForceGenerator(ActorId id) {
	std::shared_ptr<ParticleForceGenerator>& fg = m_force_generators[id];
	ParticleForceGenerator* pFg = fg.get();
	for (Particle* pParticle : m_particle_world.getParticles()) {
		m_particle_world.getForceRegistry().remove(pParticle, pFg);
	}
	m_force_generators.erase(id);
}

void XPhysics::NewParticleComponentDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_New_Particle_Component> pCastEventData = std::static_pointer_cast<EvtData_New_Particle_Component>(pEventData);
	Particle* pParticle = pCastEventData->GetParticlePtr();
	ActorId act = pCastEventData->GetActorId();
	m_particle_world.getParticles().push_back(pParticle);
	m_particle_array.emplace(std::make_pair(act, pParticle));
}

void XPhysics::DestroyActorDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_Destroy_Actor> pCastEventData = std::static_pointer_cast<EvtData_Destroy_Actor>(pEventData);
	ActorId act = pCastEventData->GetId();
	if (m_particle_array.count(act)) {
		VRemoveActorParticle(act);
	}
	if (m_contact_generators.count(act)) {
		VRemoveContactGenerator(act);
	}
}

void XPhysics::NewParticleContactGeneratorComponentDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_New_Particle_Contact_Generator> pCastEventData = std::static_pointer_cast<EvtData_New_Particle_Contact_Generator>(pEventData);
	ActorId act = pCastEventData->GetActorId();
	std::shared_ptr<ParticleContactGenerator> pCg = pCastEventData->GetContactGenerator();
	m_particle_world.getContactGenerators().push_back(pCg.get());
	m_contact_generators.emplace(std::make_pair(act, pCg));
}

void XPhysics::NewParticleForceGeneratorComponentDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_New_Particle_Force_Generator> pCastEventData = std::static_pointer_cast<EvtData_New_Particle_Force_Generator>(pEventData);
	ActorId act = pCastEventData->GetActorId();
	std::shared_ptr<ParticleForceGenerator> pFg = pCastEventData->GetForceGenerator();
	for (Particle* pParticle : m_particle_world.getParticles()) {
		m_particle_world.getForceRegistry().add(pParticle, pFg.get());
	}
	m_force_generators.emplace(std::make_pair(act, pFg));
}
