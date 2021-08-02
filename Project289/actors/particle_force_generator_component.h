#pragma once

#include <string>
#include <memory>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../engine/i_engine_physics.h"
#include "../physics/particle.h"
#include "../physics/particle_gravity.h"

using namespace std::literals;

class ParticleForceGeneratorComponent : public ActorComponent {
public:
    static const std::string g_Name;
    virtual const std::string& VGetName() const override;

public:
    ParticleForceGeneratorComponent();
    virtual ~ParticleForceGeneratorComponent();

    virtual TiXmlElement* VGenerateXml() override;

    virtual bool VInit(TiXmlElement* pData) override;
    virtual void VPostInit() override;
    virtual void VUpdate(float deltaMs) override;

    float VGetGravity();
    std::shared_ptr<ParticleForceGenerator> VGetForceGenerator();

protected:
    std::shared_ptr<ParticleForceGenerator> m_force_generator;

    float m_gravity;
    std::string m_force_generator_type_name;
};