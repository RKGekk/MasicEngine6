#include "particle_force_generator_component.h"
#include "../events/evt_data_new_particle_force_generator.h"
#include "../events/i_event_manager.h"
#include "transform_component.h"

#include "../tools/memory_utility.h"

const std::string ParticleForceGeneratorComponent::g_Name = "ParticleForceGeneratorComponent"s;

const std::string& ParticleForceGeneratorComponent::VGetName() const {
    return ParticleForceGeneratorComponent::g_Name;
}

ParticleForceGeneratorComponent::ParticleForceGeneratorComponent() {
    m_gravity = 9.8f;
}

ParticleForceGeneratorComponent::~ParticleForceGeneratorComponent() {}

TiXmlElement* ParticleForceGeneratorComponent::VGenerateXml() {
    return nullptr;
}

bool ParticleForceGeneratorComponent::VInit(TiXmlElement* pData) {

    TiXmlElement* pTypeName = pData->FirstChildElement("ForceGeneratorTypeName");
    if (pTypeName) {
        m_force_generator_type_name = pTypeName->FirstChild()->Value();
    }

    TiXmlElement* pGravity = pData->FirstChildElement("Gravity");
    if (pGravity) {
        const char* gValue = pGravity->FirstChild()->Value();
        float gravity = std::stof(gValue);
        m_gravity = gravity;
    }

    return true;
}

void ParticleForceGeneratorComponent::VPostInit() {
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (m_force_generator_type_name == "Gravity" && pTransformComponent) {
        m_force_generator = std::make_shared<ParticleGravity>(DirectX::XMVectorScale(pTransformComponent->GetPosition(), m_gravity));
    }
    
    std::shared_ptr<EvtData_New_Particle_Force_Generator> pEvent(new EvtData_New_Particle_Force_Generator(m_pOwner->GetId(), m_force_generator));
    IEventManager::Get()->VTriggerEvent(pEvent);
}

void ParticleForceGeneratorComponent::VUpdate(float deltaMs) {}

float ParticleForceGeneratorComponent::VGetGravity() {
    return m_gravity;
}

std::shared_ptr<ParticleForceGenerator> ParticleForceGeneratorComponent::VGetForceGenerator() {
    return m_force_generator;
}


