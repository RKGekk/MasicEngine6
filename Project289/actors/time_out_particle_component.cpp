#include "time_out_particle_component.h"
#include "../engine/engine.h"
#include "../tools/memory_utility.h"
#include "particle_component.h"
#include "transform_component.h"

const std::string TimeOutParticleComponent::g_Name = "TimeOutParticleComponent";

TimeOutParticleComponent::TimeOutParticleComponent() {
    m_sleep_until = 0.0f;
    m_overdue = false;
}

bool TimeOutParticleComponent::VInit(TiXmlElement* pData) {
    TiXmlElement* pSleepUntil = pData->FirstChildElement("SleepUntil");
    if (pSleepUntil) {
        float sleepUntil = std::stof(pSleepUntil->FirstChild()->Value());
        m_sleep_until = sleepUntil;
    }

    return true;
}

void TimeOutParticleComponent::VUpdate(float deltaMs) {
    if (m_overdue) {
        std::shared_ptr<Actor> act = GetOwner();
        std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
        if (pc) {
            pc->VGetParticle().setAwake(true);
        }
        return;
    }

    float current_time = g_pApp->GetTimer().TotalTime();
    if (current_time < m_sleep_until) {
        /*std::shared_ptr<Actor> act = GetOwner();
        std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
        if (pc) {
            Particle& particle = pc->VGetParticle();
            particle.setAwake(false);
            particle.clearAccumulator();
        }*/
        return;
    }
    else {
        m_overdue = true;
    }
}

TiXmlElement* TimeOutParticleComponent::VGenerateXml() {
    return nullptr;
}

const std::string& TimeOutParticleComponent::VGetName() const {
    return TimeOutParticleComponent::g_Name;
}