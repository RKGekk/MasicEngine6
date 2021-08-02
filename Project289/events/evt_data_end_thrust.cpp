#include "evt_data_end_thrust.h"

const std::string EvtData_EndThrust::sk_EventName = "EvtData_EndThrust";

EventTypeId EvtData_EndThrust::VGetEventType() const {
	return sk_EventType;
}

IEventDataPtr EvtData_EndThrust::VCopy() const {
	return IEventDataPtr(new EvtData_EndThrust(m_id));
}

const std::string& EvtData_EndThrust::GetName() const {
	return sk_EventName;
}

ActorId EvtData_EndThrust::GetActorId() const {
	return m_id;
}

void EvtData_EndThrust::Set(ActorId id) {
	m_id = id;
}

void EvtData_EndThrust::VSerialize(std::ostream& out) const {
	out << m_id << " ";
}

void EvtData_EndThrust::VDeserialize(std::istream& in) {
	in >> m_id;
}

EvtData_EndThrust::EvtData_EndThrust() : m_id(INVALID_ACTOR_ID) {}

EvtData_EndThrust::EvtData_EndThrust(ActorId id) : m_id(id) {}
