#pragma once

#include <string>
#include <memory>

#include <DirectXMath.h>

#include "actor_component.h"

using namespace std::literals;

class OrientationRelationComponent : public ActorComponent {
public:
    static const std::string g_Name;
    virtual const std::string& VGetName() const override;

public:
    OrientationRelationComponent();
    virtual ~OrientationRelationComponent();

    virtual TiXmlElement* VGenerateXml() override;

    virtual bool VInit(TiXmlElement* pData) override;
    virtual void VPostInit() override;
    virtual void VUpdate(float deltaMs) override;

    virtual const std::string& VGetRelatedToName();
    virtual WeakActorPtr VGetRelatedToActor();
    virtual const DirectX::XMFLOAT4& VGetAt4f();
    virtual const DirectX::XMFLOAT4& VGetUp4f();
    virtual const DirectX::XMFLOAT4& VGetRight4f();
    virtual DirectX::XMVECTOR VGetAt();
    virtual DirectX::XMVECTOR VGetUp();
    virtual DirectX::XMVECTOR VGetRight();

protected:
    std::string m_relate_to;
    DirectX::XMFLOAT4 m_at;
    DirectX::XMFLOAT4 m_right;
    DirectX::XMFLOAT4 m_up;
    bool m_first;
};