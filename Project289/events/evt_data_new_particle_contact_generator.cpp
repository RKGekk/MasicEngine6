#include "evt_data_new_particle_contact_generator.h"

const std::string EvtData_New_Particle_Contact_Generator::sk_EventName = "EvtData_New_Particle_Contact_Generator";

EvtData_New_Particle_Contact_Generator::EvtData_New_Particle_Contact_Generator() {
    m_actorId = 0;
}

EvtData_New_Particle_Contact_Generator::EvtData_New_Particle_Contact_Generator(ActorId actorId, std::shared_ptr<ParticleContactGenerator> contact_generator) {
    m_actorId = actorId;
    m_contact_generator = contact_generator;
}

void EvtData_New_Particle_Contact_Generator::VSerialize(std::ostream& out) const {}
void EvtData_New_Particle_Contact_Generator::VDeserialize(std::istream& in) {}

EventTypeId EvtData_New_Particle_Contact_Generator::VGetEventType() const {
    return sk_EventType;
}

IEventDataPtr EvtData_New_Particle_Contact_Generator::VCopy() const {
    return IEventDataPtr(new EvtData_New_Particle_Contact_Generator(m_actorId, m_contact_generator));
}

const std::string& EvtData_New_Particle_Contact_Generator::GetName() const {
    return sk_EventName;
}

ActorId EvtData_New_Particle_Contact_Generator::GetActorId() const {
    return m_actorId;
}

std::shared_ptr<ParticleContactGenerator> EvtData_New_Particle_Contact_Generator::GetContactGenerator() {
    return m_contact_generator;
}

std::ostream& operator<<(std::ostream& os, const EvtData_New_Particle_Contact_Generator& evt) {
    std::ios::fmtflags oldFlag = os.flags();
    os << "Event type id: " << evt.sk_EventType << std::endl;
    os << "Event name: " << evt.sk_EventName << std::endl;
    os << "Event time stamp: " << evt.GetTimeStamp().time_since_epoch().count() << "ns" << std::endl;
    os << "Event actor id: " << evt.GetActorId() << std::endl;
    os.flags(oldFlag);
    return os;
}