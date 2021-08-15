#include "actor_factory.h"

#include "transform_component.h"
#include "physics_component.h"
#include "light_render_component.h"
#include "mesh_component.h"
#include "mesh_render_component.h"
#include "particle_component.h"
#include "particle_contact_generator_component.h"
#include "particle_force_generator_component.h"
#include "smoke_component.h"
#include "mesh_render_light_component.h"
#include "orientation_relation_component.h"
#include "pers_texture_anim_state_component.h"
#include "spawn_component.h"
#include "spawn_relation_component.h"
#include "enemy_component.h"
#include "character_stats_component.h"
#include "stats_render_component.h"
#include "sky_render_component.h"
#include "time_out_particle_component.h"
#include "memo_component.h"
#include "drop_component.h"

unsigned int ActorFactory::GetNextActorId() {
    return ++m_lastActorId;
}

ActorFactory::ActorFactory() {
    m_lastActorId = 0;

    m_componentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::g_Name), TransformComponent::g_Name);
    m_componentFactory.Register<PhysicsComponent>(ActorComponent::GetIdFromName(PhysicsComponent::g_Name), PhysicsComponent::g_Name);
    m_componentFactory.Register<LightRenderComponent>(ActorComponent::GetIdFromName(LightRenderComponent::g_Name), LightRenderComponent::g_Name);
    m_componentFactory.Register<MeshComponent>(ActorComponent::GetIdFromName(MeshComponent::g_Name), MeshComponent::g_Name);
    m_componentFactory.Register<MeshRenderComponent>(ActorComponent::GetIdFromName(MeshRenderComponent::g_Name), MeshRenderComponent::g_Name);
    m_componentFactory.Register<ParticleComponent>(ActorComponent::GetIdFromName(ParticleComponent::g_Name), ParticleComponent::g_Name);
    m_componentFactory.Register<ParticleContactGeneratorComponent>(ActorComponent::GetIdFromName(ParticleContactGeneratorComponent::g_Name), ParticleContactGeneratorComponent::g_Name);
    m_componentFactory.Register<ParticleForceGeneratorComponent>(ActorComponent::GetIdFromName(ParticleForceGeneratorComponent::g_Name), ParticleForceGeneratorComponent::g_Name);
    m_componentFactory.Register<SmokeComponent>(ActorComponent::GetIdFromName(SmokeComponent::g_Name), SmokeComponent::g_Name);
    m_componentFactory.Register<MeshRenderLightComponent>(ActorComponent::GetIdFromName(MeshRenderLightComponent::g_Name), MeshRenderLightComponent::g_Name);
    m_componentFactory.Register<OrientationRelationComponent>(ActorComponent::GetIdFromName(OrientationRelationComponent::g_Name), OrientationRelationComponent::g_Name);
    m_componentFactory.Register<PersTextureAnimStateComponent>(ActorComponent::GetIdFromName(PersTextureAnimStateComponent::g_Name), PersTextureAnimStateComponent::g_Name);
    m_componentFactory.Register<SpawnComponent>(ActorComponent::GetIdFromName(SpawnComponent::g_Name), SpawnComponent::g_Name);
    m_componentFactory.Register<SpawnRelationComponent>(ActorComponent::GetIdFromName(SpawnRelationComponent::g_Name), SpawnRelationComponent::g_Name);
    m_componentFactory.Register<EnemyComponent>(ActorComponent::GetIdFromName(EnemyComponent::g_Name), EnemyComponent::g_Name);
    m_componentFactory.Register<CharacterStatsComponent>(ActorComponent::GetIdFromName(CharacterStatsComponent::g_Name), CharacterStatsComponent::g_Name);
    m_componentFactory.Register<StatsRenderComponent>(ActorComponent::GetIdFromName(StatsRenderComponent::g_Name), StatsRenderComponent::g_Name);
    m_componentFactory.Register<SkyRenderComponent>(ActorComponent::GetIdFromName(SkyRenderComponent::g_Name), SkyRenderComponent::g_Name);
    m_componentFactory.Register<TimeOutParticleComponent>(ActorComponent::GetIdFromName(TimeOutParticleComponent::g_Name), TimeOutParticleComponent::g_Name);
    m_componentFactory.Register<MemoComponent>(ActorComponent::GetIdFromName(MemoComponent::g_Name), MemoComponent::g_Name);
    m_componentFactory.Register<DropComponent>(ActorComponent::GetIdFromName(DropComponent::g_Name), DropComponent::g_Name);
}

std::shared_ptr<Actor> ActorFactory::CreateActor(const char* actorResource, TiXmlElement* overrides, const DirectX::XMFLOAT4X4* pinitialTransform, const ActorId serversActorId) {

    TiXmlDocument xml;
    xml.LoadFile(actorResource);

    TiXmlElement* pRoot = xml.RootElement();
    if (!pRoot) {
        return std::shared_ptr<Actor>();
    }

    ActorId nextActorId = serversActorId;
    if (nextActorId == 0) {
        nextActorId = GetNextActorId();
    }
    std::shared_ptr<Actor> pActor(new Actor(nextActorId));
    if (!pActor->Init(pRoot)) {
        return std::shared_ptr<Actor>();
    }

    bool initialTransformSet = false;

    for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
        std::shared_ptr<ActorComponent> pComponent(VCreateComponent(pNode));
        if (pComponent) {
            pActor->AddComponent(pComponent);
            pComponent->SetOwner(pActor);
        }
        else {
            return std::shared_ptr<Actor>();
        }
    }

    if (overrides) {
        ModifyActor(pActor, overrides);
    }

    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pinitialTransform && pTransformComponent) {
        pTransformComponent->SetPosition4x4f(*pinitialTransform);
    }

    pActor->PostInit();

    return pActor;
}

std::shared_ptr<ActorComponent> ActorFactory::VCreateComponent(TiXmlElement* pData) {
    const char* name = pData->Value();
    std::shared_ptr<ActorComponent> pComponent(m_componentFactory.Create(ActorComponent::GetIdFromName(name)));

    if (pComponent) {
        if (!pComponent->VInit(pData)) {
            return std::shared_ptr<ActorComponent>();
        }
    }
    else {
        return std::shared_ptr<ActorComponent>();
    }

    return pComponent;
}

std::shared_ptr<Actor> ActorFactory::CreateActor(const std::string& actorResource, TiXmlElement* overrides, const DirectX::XMFLOAT4X4* initialTransform, const ActorId serversActorId) {
    return CreateActor(actorResource.c_str(), overrides, initialTransform, serversActorId);
}

std::shared_ptr<Actor> ActorFactory::CreateActor(const std::string& actorResource, TiXmlElement* overrides, DirectX::FXMMATRIX initialTransform, const ActorId serversActorId) {
    DirectX::XMFLOAT4X4 T;
    DirectX::XMStoreFloat4x4(&T, initialTransform);
    return CreateActor(actorResource.c_str(), overrides, &T, serversActorId);
}

void ActorFactory::ModifyActor(std::shared_ptr<Actor> pActor, TiXmlElement* overrides) {
    if (overrides->Attribute("name")) {
        pActor->SetName(overrides->Attribute("name"));
    };
    for (TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
        unsigned int componentId = ActorComponent::GetIdFromName(pNode->Value());
        std::shared_ptr<ActorComponent> pComponent = MakeStrongPtr(pActor->GetComponent<ActorComponent>(componentId));
        if (pComponent) {
            pComponent->VInit(pNode);

            pComponent->VOnChanged();
        }
        else {
            pComponent = VCreateComponent(pNode);
            if (pComponent) {
                pActor->AddComponent(pComponent);
                pComponent->SetOwner(pActor);
            }
        }
    }
}