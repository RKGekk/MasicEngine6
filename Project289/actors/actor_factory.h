#pragma once

#include <map>
#include <string>
#include <memory>
#include <DirectXMath.h>

#include "../tools/tinyxml/tinyxml.h"
#include "Actor.h"
#include "actor_component.h"
#include "../tools/generic_object_factory.h"
#include "../tools/memory_utility.h"

class ActorFactory {
    ActorId m_lastActorId;

protected:
    GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

public:
    ActorFactory();

    std::shared_ptr<Actor> CreateActor(const char* actorResource, TiXmlElement* overrides, const DirectX::XMFLOAT4X4* initialTransform, const ActorId serversActorId);
    std::shared_ptr<Actor> CreateActor(const std::string& actorResource, TiXmlElement* overrides, const DirectX::XMFLOAT4X4* initialTransform, const ActorId serversActorId);
    std::shared_ptr<Actor> CreateActor(const std::string& actorResource, TiXmlElement* overrides, DirectX::FXMMATRIX initialTransform, const ActorId serversActorId);
    void ModifyActor(std::shared_ptr<Actor> pActor, TiXmlElement* overrides);

    virtual std::shared_ptr<ActorComponent> VCreateComponent(TiXmlElement* pData);

private:
    ActorId GetNextActorId();
};