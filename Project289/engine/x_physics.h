#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

#include <DirectXMath.h>

#include "../tools/math_utitity.h"

#include "../actors/actor.h"
#include "i_engine_physics.h"

#include "../physics/particle.h"
#include "../physics/particle_contact.h"
#include "../physics/particle_contact_resolver.h"
#include "../physics/particle_contact_generator.h"
#include "../physics/particle_force_registry.h"
#include "../physics/particle_world.h"
#include "../physics/ground_contacts.h"

#include "../events/i_event_data.h"

class XPhysics : public IEnginePhysics {
	ParticleWorld m_particle_world;
	std::unordered_map<ActorId, Particle*> m_particle_array;
	std::unordered_map<ActorId, std::shared_ptr<ParticleContactGenerator>> m_contact_generators;
	std::unordered_map<ActorId, std::shared_ptr<ParticleForceGenerator>> m_force_generators;

public:
	XPhysics();
	virtual ~XPhysics();

	virtual bool VInitialize() override;
	virtual void VOnUpdate(float deltaSeconds) override;
	virtual void VSyncVisibleScene() override;

	virtual void VAddParticleActor(const ActorId actorId) override;
	virtual std::vector<Particle*>& VGetParticles() override;
	virtual void VRemoveParticle(Particle* p) override;
	virtual void VRemoveActorParticle(ActorId id) override;

	virtual void VAddContactGenerator(ActorId id) override;
	virtual void VRemoveContactGenerator(ActorId id) override;

	virtual void VAddForceGenerator(ActorId id) override;
	virtual void VRemoveForceGenerator(ActorId id) override;

	void NewParticleComponentDelegate(IEventDataPtr pEventData);
	void DestroyActorDelegate(IEventDataPtr pEventData);
	void NewParticleContactGeneratorComponentDelegate(IEventDataPtr pEventData);
	void NewParticleForceGeneratorComponentDelegate(IEventDataPtr pEventData);
};