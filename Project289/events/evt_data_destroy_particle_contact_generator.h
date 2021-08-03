#pragma once

#include <string>
#include <iostream>
#include <iomanip>

#include "base_event_data.h"
#include "../actors/actor.h"

class SceneNode;

class EvtData_Destroy_Particle_Contact_Generator : public BaseEventData {
    ActorId m_actorId;

public:
    static const EventTypeId sk_EventType = 0x8de6e016;
    static const std::string sk_EventName;

    EvtData_Destroy_Particle_Contact_Generator();
    explicit EvtData_Destroy_Particle_Contact_Generator(ActorId actorId);

    virtual void VSerialize(std::ostream& out) const override;
    virtual void VDeserialize(std::istream& in) override;
    virtual EventTypeId VGetEventType() const override;
    virtual IEventDataPtr VCopy() const override;
    virtual const std::string& GetName() const override;

    ActorId GetActorId() const;

    friend std::ostream& operator<<(std::ostream& os, const EvtData_Destroy_Particle_Contact_Generator& evt);
};
