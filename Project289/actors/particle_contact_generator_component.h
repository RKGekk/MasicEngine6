#pragma once

#include <string>
#include <memory>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../engine/i_engine_physics.h"
#include "../physics/particle.h"
#include "../physics/contact_generator.h"
#include "../physics/ground_contacts.h"
#include "../physics/particle_constraint.h"
#include "../physics/particle_link.h"
#include "../physics/particle_rod.h"

using namespace std::literals;

class ParticleContactGeneratorComponent : public ActorComponent {
public:
    static const std::string g_Name;
    virtual const std::string& VGetName() const override;

public:
    ParticleContactGeneratorComponent();
    virtual ~ParticleContactGeneratorComponent();

    virtual TiXmlElement* VGenerateXml() override;

    virtual bool VInit(TiXmlElement* pData) override;
    virtual void VPostInit() override;
    virtual void VUpdate(float deltaMs) override;

    float VGetGroundLevel();
    const std::string& VGetGeneratorTypeName();
    std::shared_ptr<ParticleContactGenerator> VGetContactGenerator();

protected:
    std::shared_ptr<ParticleContactGenerator> m_contact_generator;

    std::string m_contact_generator_type_name;

    float m_ground_level;
    float m_restitution;
    DirectX::XMFLOAT3 m_anchor;
    std::string m_particle_name_to_link;
};