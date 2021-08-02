#pragma once

#include <string>
#include <memory>

#include "base_event_data.h"
#include "../actors/actor.h"
#include "../physics/particle_contact_generator.h"

class EvtData_New_Particle_Contact_Generator : public BaseEventData {
    ActorId m_actorId;
    std::shared_ptr<ParticleContactGenerator> m_contact_generator;

public:
    static const EventTypeId sk_EventType = 0xd191e882;
    static const std::string sk_EventName;

    EvtData_New_Particle_Contact_Generator();
    explicit EvtData_New_Particle_Contact_Generator(ActorId actorId, std::shared_ptr<ParticleContactGenerator>);

    virtual void VSerialize(std::ostream& out) const override;
    virtual void VDeserialize(std::istream& in) override;
    virtual EventTypeId VGetEventType() const override;
    virtual IEventDataPtr VCopy() const override;
    virtual const std::string& GetName() const override;

    ActorId GetActorId() const;
    std::shared_ptr<ParticleContactGenerator> GetContactGenerator();

    friend std::ostream& operator<<(std::ostream& os, const EvtData_New_Particle_Contact_Generator& evt);
};