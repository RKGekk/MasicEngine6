#pragma once

#include <string>
#include <memory>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../engine/i_engine_physics.h"

using namespace std::literals;

class PhysicsComponent : public ActorComponent {
public:
    static const std::string g_Name;
    virtual const std::string& VGetName() const override;

public:
    PhysicsComponent();
    virtual ~PhysicsComponent();

    virtual TiXmlElement* VGenerateXml() override;

    virtual bool VInit(TiXmlElement* pData) override;
    virtual void VPostInit() override;
    virtual void VUpdate(float deltaMs) override;

    void ApplyForce3f(const DirectX::XMFLOAT3& direction, float forceNewtons);
    void ApplyForce(DirectX::FXMVECTOR direction, float forceNewtons);
    void ApplyTorque3f(const DirectX::XMFLOAT3& direction, float forceNewtons);
    void ApplyTorque(DirectX::FXMVECTOR direction, float forceNewtons);
    bool KinematicMove4x4f(const DirectX::XMFLOAT4X4& transform);
    bool KinematicMove(DirectX::FXMMATRIX transform);

    void ApplyAcceleration(float acceleration);
    void RemoveAcceleration();
    void ApplyAngularAcceleration(float acceleration);
    void RemoveAngularAcceleration();

    DirectX::XMFLOAT3 GetVelocity3f();
    DirectX::XMVECTOR GetVelocity();
    void SetVelocity(const DirectX::XMFLOAT3& velocity);
    void RotateY(float angleRadians);
    void SetPosition(float x, float y, float z);
    void Stop();

protected:
    void CreateShape();
    void BuildRigidBodyTransform(TiXmlElement* pTransformElement);

    float m_acceleration;
    float m_angularAcceleration;
    float m_maxVelocity;
    float m_maxAngularVelocity;

    std::string m_shape;
    std::string m_density;
    std::string m_material;

    DirectX::XMFLOAT3 m_RigidBodyLocation;
    DirectX::XMFLOAT3 m_RigidBodyOrientation;
    DirectX::XMFLOAT3 m_RigidBodyScale;

    IEnginePhysics* m_pGamePhysics;
};