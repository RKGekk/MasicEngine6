#include "particle_sphere_contact.h"

unsigned ParticleSphereContact::addContact(ParticleContact* contact, unsigned limit) const {
    using namespace DirectX;

    unsigned count = 0;
    for (ParticleWorld::Particles::iterator p1 = m_particles->begin(); p1 != m_particles->end(); ++p1) {
        for (ParticleWorld::Particles::iterator p2 = m_particles->begin(); p2 != m_particles->end(); ++p2) {
            if (p2 != p1) {
                XMVECTOR particle1Pos = (*p1)->getPosition();
                XMVECTOR particle2Pos = (*p2)->getPosition();
                float rad1 = (*p1)->getRadius();
                float rad2 = (*p2)->getRadius();

                XMVECTOR contactTrace = particle1Pos - particle2Pos;
                float distance = XMVectorGetX(XMVector3Length(contactTrace));

                if (distance < (rad1 + rad2)) {
                    XMStoreFloat3(&contact->contactNormal, XMVector3Normalize(contactTrace));
                    contact->particle[0] = *p1;
                    contact->particle[1] = *p2;
                    contact->penetration = (rad1 + rad2) - distance;
                    contact->restitution = 1.0f;
                    count++;
                }

                if (count >= limit) { return count; }
            }
        }
    }
    return count;
}
