#include "particle_contact_generator_component.h"
#include "../events/evt_data_new_particle_contact_generator.h"
#include "../events/i_event_manager.h"
#include "../physics/ground_contacts.h"
#include "../physics/particle_constraint.h"
#include "../physics/particle_link.h"
#include "../physics/particle_rod.h"
#include "../physics/particle_sphere_contact.h"
#include "../physics/particle_sphere_contact_evet_gen.h"
#include "../physics/geo_ground_contacts.h"
#include "transform_component.h"
#include "../tools/memory_utility.h"
#include "../engine/engine.h"

const std::string ParticleContactGeneratorComponent::g_Name = "ParticleContactGeneratorComponent"s;

const std::string& ParticleContactGeneratorComponent::VGetName() const {
	return ParticleContactGeneratorComponent::g_Name;
}

ParticleContactGeneratorComponent::ParticleContactGeneratorComponent() {
	m_contact_generator_type_name = "NoName";
	m_ground_level = 0.0f;
    m_restitution = 0.2f;
	m_anchor = { 0.0f, 0.0f, 0.0f };
	m_particle_name_to_link = "NoName";
}

ParticleContactGeneratorComponent::~ParticleContactGeneratorComponent() {}

TiXmlElement* ParticleContactGeneratorComponent::VGenerateXml() {
	return nullptr;
}

bool ParticleContactGeneratorComponent::VInit(TiXmlElement* pData) {

    TiXmlElement* pGroundLevel = pData->FirstChildElement("GroundLevel");
    if (pGroundLevel) {
        const char* gValue = pGroundLevel->FirstChild()->Value();
        float groundLevel = std::stof(gValue);
        m_ground_level = groundLevel;
    }

    TiXmlElement* pRestitution = pData->FirstChildElement("Restitution");
    if (pRestitution) {
        const char* rValue = pRestitution->FirstChild()->Value();
        float restitution = std::stof(rValue);
        m_restitution = restitution;
    }

    TiXmlElement* pTypeName = pData->FirstChildElement("ContactGeneratorTypeName");
    if (pTypeName) {
        m_contact_generator_type_name = pTypeName->FirstChild()->Value();
    }

    return true;
}

void ParticleContactGeneratorComponent::VPostInit() {
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (m_contact_generator_type_name == "GroundContact") {
        m_contact_generator = std::make_shared<GroundContacts>(m_ground_level, m_restitution);
    }
    if (m_contact_generator_type_name == "GeoGroundContact") {
        m_contact_generator = std::make_shared<GeoGroundContacts>(pTransformComponent->GetPosition(), m_ground_level, m_restitution);
    }
    if (m_contact_generator_type_name == "ParticleSphereContact") {
        ParticleWorld::Particles* particles = &g_pApp->GetGameLogic()->VGetGamePhysics()->VGetParticles();
        std::shared_ptr<ParticleSphereContact> cg = std::make_shared<ParticleSphereContact>();
        cg->init(particles);
        m_contact_generator = cg;
    }
    if (m_contact_generator_type_name == "ParticleSphereContactEventGen") {
        ParticleWorld::Particles* particles = &g_pApp->GetGameLogic()->VGetGamePhysics()->VGetParticles();
        std::shared_ptr<ParticleSphereContactEventGen> cg = std::make_shared<ParticleSphereContactEventGen>();
        cg->init(particles);
        m_contact_generator = cg;
    }

    std::shared_ptr<EvtData_New_Particle_Contact_Generator> pEvent(new EvtData_New_Particle_Contact_Generator(m_pOwner->GetId(), m_contact_generator));
    IEventManager::Get()->VTriggerEvent(pEvent);
}

void ParticleContactGeneratorComponent::VUpdate(float deltaMs) {}

float ParticleContactGeneratorComponent::VGetGroundLevel() {
	return m_ground_level;
}

const std::string& ParticleContactGeneratorComponent::VGetGeneratorTypeName() {
	return m_contact_generator_type_name;
}

std::shared_ptr<ParticleContactGenerator> ParticleContactGeneratorComponent::VGetContactGenerator() {
    return m_contact_generator;
}