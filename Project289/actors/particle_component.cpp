#include "particle_component.h"
#include "../engine/engine.h"
#include "transform_component.h"
#include "../events/evt_data_new_particle_component.h"
#include "../events/i_event_manager.h"
#include "../tools/memory_utility.h"

const std::string ParticleComponent::g_Name = "ParticleComponent"s;

const std::string& ParticleComponent::VGetName() const {
	return ParticleComponent::g_Name;
}

ParticleComponent::ParticleComponent() {
    m_particle.setPosition(0.0f, 0.0f, 0.0f);
    m_particle.setVelocity(0.0f, 0.0f, 0.0f);
    m_particle.setAcceleration(0.0f, 0.0f, 0.0f);
    m_particle.setMass(1.0f);
    m_particle.setDamping(0.9f);
    m_particle.setRadius(1.0f);
    m_pGamePhysics = nullptr;
}

ParticleComponent::~ParticleComponent() {
    m_pGamePhysics->VRemoveParticle(&m_particle);
}

TiXmlElement* ParticleComponent::VGenerateXml() {
    return nullptr;
}

bool ParticleComponent::VInit(TiXmlElement* pData) {
    m_pGamePhysics = g_pApp->GetGameLogic()->VGetGamePhysics();
    if (!m_pGamePhysics) {
        return false;
    }

    TiXmlElement* pVelocityElement = pData->FirstChildElement("Velocity");
    if (pVelocityElement) {
        double x = 0;
        double y = 0;
        double z = 0;
        pVelocityElement->Attribute("x", &x);
        pVelocityElement->Attribute("y", &y);
        pVelocityElement->Attribute("z", &z);
        m_particle.setVelocity(x, y, z);
    }
    TiXmlElement* pAccelerationElement = pData->FirstChildElement("Acceleration");
    if (pAccelerationElement) {
        double x = 0;
        double y = 0;
        double z = 0;
        pAccelerationElement->Attribute("x", &x);
        pAccelerationElement->Attribute("y", &y);
        pAccelerationElement->Attribute("z", &z);
        m_particle.setAcceleration(x, y, z);
    }

    TiXmlElement* pMass = pData->FirstChildElement("Mass");
    if (pMass) {
        float mass = std::stof(pMass->FirstChild()->Value());
        m_particle.setMass(mass);
    }

    TiXmlElement* pDamping = pData->FirstChildElement("Damping");
    if (pDamping) {
        float damping = std::stof(pDamping->FirstChild()->Value());
        m_particle.setDamping(damping);
    }

    TiXmlElement* pRadius = pData->FirstChildElement("Radius");
    if (pRadius) {
        float radius = std::stof(pRadius->FirstChild()->Value());
        m_particle.setRadius(radius);
    }

    return true;
}

void ParticleComponent::VPostInit() {
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent) {
        m_particle.setPosition3f(pTransformComponent->GetPosition3f());
    }

    std::shared_ptr<EvtData_New_Particle_Component> pEvent(new EvtData_New_Particle_Component(m_pOwner->GetId(), &m_particle));
    IEventManager::Get()->VTriggerEvent(pEvent);
}

void ParticleComponent::VUpdate(float deltaMs) {}

Particle& ParticleComponent::VGetParticle() {
    return m_particle;
}

Particle* ParticleComponent::VGetParticlePtr() {
    return &m_particle;
}
