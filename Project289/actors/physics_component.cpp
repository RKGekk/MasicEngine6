#include "physics_component.h"
#include "../engine/engine.h"
#include "transform_component.h"

const float DEFAULT_MAX_VELOCITY = 7.5f;
const float DEFAULT_MAX_ANGULAR_VELOCITY = 1.2f;

const std::string PhysicsComponent::g_Name = "PhysicsComponent"s;

const std::string& PhysicsComponent::VGetName() const {
	return PhysicsComponent::g_Name;
}

PhysicsComponent::PhysicsComponent() {
	m_RigidBodyLocation = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	m_RigidBodyOrientation = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	m_RigidBodyScale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

	m_acceleration = 0;
	m_angularAcceleration = 0;
	m_maxVelocity = DEFAULT_MAX_VELOCITY;
	m_maxAngularVelocity = DEFAULT_MAX_ANGULAR_VELOCITY;
}

PhysicsComponent::~PhysicsComponent() {
	m_pGamePhysics->VRemoveActorParticle(m_pOwner->GetId());
}

TiXmlElement* PhysicsComponent::VGenerateXml() {
    TiXmlElement* pBaseElement = new TiXmlElement(VGetName().c_str());

    TiXmlElement* pShape = new TiXmlElement("Shape");
    TiXmlText* pShapeText = new TiXmlText(m_shape.c_str());
    pShape->LinkEndChild(pShapeText);
    pBaseElement->LinkEndChild(pShape);

    TiXmlElement* pDensity = new TiXmlElement("Density");
    TiXmlText* pDensityText = new TiXmlText(m_density.c_str());
    pDensity->LinkEndChild(pDensityText);
    pBaseElement->LinkEndChild(pDensity);

    TiXmlElement* pMaterial = new TiXmlElement("Material");
    TiXmlText* pMaterialText = new TiXmlText(m_material.c_str());
    pMaterial->LinkEndChild(pMaterialText);
    pBaseElement->LinkEndChild(pMaterial);

    TiXmlElement* pInitialTransform = new TiXmlElement("RigidBodyTransform");

    TiXmlElement* pPosition = new TiXmlElement("Position");
    pPosition->SetAttribute("x", std::to_string(m_RigidBodyLocation.x).c_str());
    pPosition->SetAttribute("y", std::to_string(m_RigidBodyLocation.y).c_str());
    pPosition->SetAttribute("z", std::to_string(m_RigidBodyLocation.z).c_str());
    pInitialTransform->LinkEndChild(pPosition);

    TiXmlElement* pOrientation = new TiXmlElement("Orientation");
    pOrientation->SetAttribute("yaw", std::to_string(m_RigidBodyOrientation.x).c_str());
    pOrientation->SetAttribute("pitch", std::to_string(m_RigidBodyOrientation.y).c_str());
    pOrientation->SetAttribute("roll", std::to_string(m_RigidBodyOrientation.z).c_str());
    pInitialTransform->LinkEndChild(pOrientation);

    TiXmlElement* pScale = new TiXmlElement("Scale");
    pScale->SetAttribute("x", std::to_string(m_RigidBodyScale.x).c_str());
    pScale->SetAttribute("y", std::to_string(m_RigidBodyScale.y).c_str());
    pScale->SetAttribute("z", std::to_string(m_RigidBodyScale.z).c_str());
    pInitialTransform->LinkEndChild(pScale);

    pBaseElement->LinkEndChild(pInitialTransform);

    return pBaseElement;
}

bool PhysicsComponent::VInit(TiXmlElement* pData) {
    m_pGamePhysics = g_pApp->GetGameLogic()->VGetGamePhysics();
    if (!m_pGamePhysics) {
        return false;
    }

    TiXmlElement* pShape = pData->FirstChildElement("Shape");
    if (pShape) {
        m_shape = pShape->FirstChild()->Value();
    }

    TiXmlElement* pDensity = pData->FirstChildElement("Density");
    if (pDensity) {
        m_density = pDensity->FirstChild()->Value();
    }

    TiXmlElement* pMaterial = pData->FirstChildElement("PhysicsMaterial");
    if (pMaterial) {
        m_material = pMaterial->FirstChild()->Value();
    }

    TiXmlElement* pRigidBodyTransform = pData->FirstChildElement("RigidBodyTransform");
    if (pRigidBodyTransform) {
        BuildRigidBodyTransform(pRigidBodyTransform);
    }

    return true;
}

void PhysicsComponent::VPostInit() {
    if (m_pOwner)     {
        /*if (m_shape == "Sphere") 		{
            m_pGamePhysics->VAddSphere((float)m_RigidBodyScale.x, m_pOwner, m_density, m_material);
        }
        else if (m_shape == "Box") 		{
            m_pGamePhysics->VAddBox(DirectX::XMLoadFloat3(&m_RigidBodyScale), m_pOwner, m_density, m_material);
        }
        else if (m_shape == "PointCloud") 		{
            
        }*/
    }
}

void PhysicsComponent::VUpdate(float deltaMs) {
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (!pTransformComponent) {
        return;
    }

    DirectX::XMMATRIX transform = pTransformComponent->GetTransform();

    if (m_acceleration != 0)     {

        float accelerationToApplyThisFrame = m_acceleration / 1000.0f * deltaMs;

        //DirectX::XMVECTOR velocity(m_pGamePhysics->VGetVelocity(m_pOwner->GetId()));
        //float velocityScalar = DirectX::XMVectorGetX(DirectX::XMVector3Length(velocity));

        //DirectX::XMMATRIX just_rot = transform;
        //just_rot.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
        //DirectX::XMVECTOR direction = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), just_rot);
        //m_pGamePhysics->VApplyForce(direction, accelerationToApplyThisFrame, m_pOwner->GetId());
    }

    if (m_angularAcceleration != 0)     {
        //float angularAccelerationToApplyThisFrame = m_angularAcceleration / 1000.f * deltaMs;
        //DirectX::XMMATRIX just_rot = transform;
        //just_rot.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
        //DirectX::XMVECTOR up = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), just_rot);
        //m_pGamePhysics->VApplyTorque(up, angularAccelerationToApplyThisFrame, m_pOwner->GetId());
    }
}

void PhysicsComponent::ApplyForce3f(const DirectX::XMFLOAT3& direction, float forceNewtons) {
    //m_pGamePhysics->VApplyForce(DirectX::XMLoadFloat3(&direction), forceNewtons, m_pOwner->GetId());
}

void PhysicsComponent::ApplyForce(DirectX::FXMVECTOR direction, float forceNewtons) {
    //m_pGamePhysics->VApplyForce(direction, forceNewtons, m_pOwner->GetId());
}

void PhysicsComponent::ApplyTorque3f(const DirectX::XMFLOAT3& direction, float forceNewtons) {
    //m_pGamePhysics->VApplyTorque(DirectX::XMLoadFloat3(&direction), forceNewtons, m_pOwner->GetId());
}

void PhysicsComponent::ApplyTorque(DirectX::FXMVECTOR direction, float forceNewtons) {
    //m_pGamePhysics->VApplyTorque(direction, forceNewtons, m_pOwner->GetId());
}

bool PhysicsComponent::KinematicMove4x4f(const DirectX::XMFLOAT4X4& transform) {
    //return m_pGamePhysics->VKinematicMove(DirectX::XMLoadFloat4x4(&transform), m_pOwner->GetId());
    return true;
}

bool PhysicsComponent::KinematicMove(DirectX::FXMMATRIX transform) {
    //return m_pGamePhysics->VKinematicMove(transform, m_pOwner->GetId());
    return true;
}

void PhysicsComponent::ApplyAcceleration(float acceleration) {
    m_acceleration = acceleration;
}

void PhysicsComponent::RemoveAcceleration() {
    m_acceleration = 0.0f;
}

void PhysicsComponent::ApplyAngularAcceleration(float acceleration) {
    m_angularAcceleration = acceleration;
}

void PhysicsComponent::RemoveAngularAcceleration() {
    m_angularAcceleration = 0.0f;
}

DirectX::XMFLOAT3 PhysicsComponent::GetVelocity3f() {
    DirectX::XMFLOAT3 res;
    //DirectX::XMStoreFloat3(&res, m_pGamePhysics->VGetVelocity(m_pOwner->GetId()));
    return res;
}

DirectX::XMVECTOR PhysicsComponent::GetVelocity() {
    //return m_pGamePhysics->VGetVelocity(m_pOwner->GetId());
    return DirectX::XMVectorReplicate(0.0f);
}

void PhysicsComponent::SetVelocity(const DirectX::XMFLOAT3& velocity) {
    //m_pGamePhysics->VSetVelocity(m_pOwner->GetId(), DirectX::XMLoadFloat3(&velocity));
}

void PhysicsComponent::RotateY(float angleRadians) {
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)     {
        DirectX::XMMATRIX transform = pTransformComponent->GetTransform();
        DirectX::XMVECTOR position = transform.r[3];

        DirectX::XMMATRIX rotateY = DirectX::XMMatrixRotationY(angleRadians);
        rotateY.r[3] = position;

        KinematicMove(rotateY);
    }
}

void PhysicsComponent::SetPosition(float x, float y, float z) {
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)     {
        DirectX::XMMATRIX transform = pTransformComponent->GetTransform();
        DirectX::XMVECTOR position = DirectX::XMVectorSet(x, y, z, 1.0f);
        transform.r[3] = position;

        KinematicMove(transform);
    }
}

void PhysicsComponent::Stop() {
    //m_pGamePhysics->VStopActor(m_pOwner->GetId());
}

void PhysicsComponent::CreateShape() {}

void PhysicsComponent::BuildRigidBodyTransform(TiXmlElement* pTransformElement) {
    TiXmlElement* pPositionElement = pTransformElement->FirstChildElement("Position");
    if (pPositionElement) {
        double x = 0;
        double y = 0;
        double z = 0;
        pPositionElement->Attribute("x", &x);
        pPositionElement->Attribute("y", &y);
        pPositionElement->Attribute("z", &z);
        m_RigidBodyLocation = DirectX::XMFLOAT3(x, y, z);
    }

    TiXmlElement* pOrientationElement = pTransformElement->FirstChildElement("Orientation");
    if (pOrientationElement) {
        double yaw = 0;
        double pitch = 0;
        double roll = 0;
        pOrientationElement->Attribute("yaw", &yaw);
        pOrientationElement->Attribute("pitch", &pitch);
        pOrientationElement->Attribute("roll", &roll);
        m_RigidBodyOrientation = DirectX::XMFLOAT3(DirectX::XMConvertToRadians(yaw), DirectX::XMConvertToRadians(pitch), DirectX::XMConvertToRadians(roll));
    }

    TiXmlElement* pScaleElement = pTransformElement->FirstChildElement("Scale");
    if (pScaleElement)     {
        double x = 0;
        double y = 0;
        double z = 0;
        pScaleElement->Attribute("x", &x);
        pScaleElement->Attribute("y", &y);
        pScaleElement->Attribute("z", &z);
        m_RigidBodyScale = DirectX::XMFLOAT3((float)x, (float)y, (float)z);
    }
}
