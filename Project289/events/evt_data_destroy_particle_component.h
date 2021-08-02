#pragma once

#include <string>

#include "base_event_data.h"
#include "../actors/actor.h"

class SceneNode;

class EvtData_Destroy_Particle_Component : public BaseEventData {
    ActorId m_actorId;

public:
    static const EventTypeId sk_EventType = 0x3118ee5d;
    static const std::string sk_EventName;

    EvtData_Destroy_Particle_Component();
    explicit EvtData_Destroy_Particle_Component(ActorId actorId);

    virtual void VSerialize(std::ostream& out) const override;
    virtual void VDeserialize(std::istream& in) override;
    virtual EventTypeId VGetEventType() const override;
    virtual IEventDataPtr VCopy() const override;
    virtual const std::string& GetName() const override;

    ActorId GetActorId() const;

    friend std::ostream& operator<<(std::ostream& os, const EvtData_Destroy_Particle_Component& evt);
};