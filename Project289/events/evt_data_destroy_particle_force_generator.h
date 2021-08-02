#pragma once

#include <string>

#include "base_event_data.h"
#include "../actors/actor.h"

class SceneNode;

class EvtData_Destroy_Particle_Force_Generator : public BaseEventData {
    ActorId m_actorId;

public:
    static const EventTypeId sk_EventType = 0xa2d9b463;
    static const std::string sk_EventName;

    EvtData_Destroy_Particle_Force_Generator();
    explicit EvtData_Destroy_Particle_Force_Generator(ActorId actorId);

    virtual void VSerialize(std::ostream& out) const override;
    virtual void VDeserialize(std::istream& in) override;
    virtual EventTypeId VGetEventType() const override;
    virtual IEventDataPtr VCopy() const override;
    virtual const std::string& GetName() const override;

    ActorId GetActorId() const;

    friend std::ostream& operator<<(std::ostream& os, const EvtData_Destroy_Particle_Force_Generator& evt);
};
