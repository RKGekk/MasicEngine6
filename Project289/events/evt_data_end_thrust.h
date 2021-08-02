#pragma once

#include "base_event_data.h"
#include "../actors/actor.h"
#include "../tools/string_utility.h"

class EvtData_EndThrust : public BaseEventData {
    ActorId m_id;

public:
    static const EventTypeId sk_EventType = 0xe60f88a4;
    static const std::string sk_EventName;

    EvtData_EndThrust();
    EvtData_EndThrust(ActorId id);

    virtual EventTypeId VGetEventType() const override;
    virtual void VSerialize(std::ostream& out) const override;
    virtual void VDeserialize(std::istream& in) override;
    virtual IEventDataPtr VCopy() const override;
    virtual const std::string& GetName() const override;

    ActorId GetActorId() const;
    void Set(ActorId id);
};