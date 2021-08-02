#pragma once

#include "base_event_data.h"
#include "../actors/actor.h"
#include "../tools/string_utility.h"

class EvtData_StartThrust : public BaseEventData {
    ActorId m_id;
    float m_acceleration;

public:
    static const EventTypeId sk_EventType = 0x1d62d48c;
    static const std::string sk_EventName;

    EvtData_StartThrust();
    EvtData_StartThrust(ActorId id, float acceleration);

    virtual EventTypeId VGetEventType() const override;
    virtual void VSerialize(std::ostream& out) const override;
    virtual void VDeserialize(std::istream& in) override;
    virtual IEventDataPtr VCopy() const override;
    virtual const std::string& GetName() const override;

    ActorId GetActorId() const;
    float GetAcceleration() const;
    void Set(ActorId id);
};