#include "evt_data_destroy_particle_component.h"

const std::string EvtData_Destroy_Particle_Component::sk_EventName = "EvtData_Destroy_Particle_Component";

EvtData_Destroy_Particle_Component::EvtData_Destroy_Particle_Component() {
    m_actorId = 0;
}

EvtData_Destroy_Particle_Component::EvtData_Destroy_Particle_Component(ActorId actorId) {
    m_actorId = actorId;
}

void EvtData_Destroy_Particle_Component::VSerialize(std::ostream& out) const {}
void EvtData_Destroy_Particle_Component::VDeserialize(std::istream& in) {}

EventTypeId EvtData_Destroy_Particle_Component::VGetEventType() const {
    return sk_EventType;
}

IEventDataPtr EvtData_Destroy_Particle_Component::VCopy() const {
    return IEventDataPtr(new EvtData_Destroy_Particle_Component(m_actorId));
}

const std::string& EvtData_Destroy_Particle_Component::GetName() const {
    return sk_EventName;
}

ActorId EvtData_Destroy_Particle_Component::GetActorId() const {
    return m_actorId;
}

std::ostream& operator<<(std::ostream& os, const EvtData_Destroy_Particle_Component& evt) {
    std::ios::fmtflags oldFlag = os.flags();
    os << "Event type id: " << evt.sk_EventType << std::endl;
    os << "Event name: " << evt.sk_EventName << std::endl;
    os << "Event time stamp: " << evt.GetTimeStamp().time_since_epoch().count() << "ns" << std::endl;
    os << "Event actor id: " << evt.GetActorId() << std::endl;
    os.flags(oldFlag);
    return os;
}