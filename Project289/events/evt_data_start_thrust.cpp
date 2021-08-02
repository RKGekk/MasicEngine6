#include "evt_data_start_thrust.h"

const std::string EvtData_StartThrust::sk_EventName = "EvtData_StartThrust";

EventTypeId EvtData_StartThrust::VGetEventType() const {
    return sk_EventType;
}

EvtData_StartThrust::EvtData_StartThrust() : m_id(INVALID_ACTOR_ID), m_acceleration(0) {}

EvtData_StartThrust::EvtData_StartThrust(ActorId id, float acceleration) : m_id(id), m_acceleration(acceleration) {}

IEventDataPtr EvtData_StartThrust::VCopy() const {
    return IEventDataPtr(new EvtData_StartThrust(m_id, m_acceleration));
}

void EvtData_StartThrust::VSerialize(std::ostream& out) const {
    out << m_id << " ";
    out << m_acceleration << " ";
}

void EvtData_StartThrust::VDeserialize(std::istream& in) {
    in >> m_id;
    in >> m_acceleration;
}

const std::string& EvtData_StartThrust::GetName() const {
    return sk_EventName;
}

ActorId EvtData_StartThrust::GetActorId() const {
    return m_id;
}

float EvtData_StartThrust::GetAcceleration() const {
    return m_acceleration;
}

void EvtData_StartThrust::Set(ActorId id) {
    m_id = id;
}
