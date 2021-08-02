#include "transform_component.h"

#include "../tools/string_utility.h"

const std::string TransformComponent::g_Name = "TransformComponent";

TransformComponent::TransformComponent() {

    DirectX::XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity());

    m_forward = DEFAULT_FORWARD_VECTOR;
    m_up = DEFAULT_UP_VECTOR;
    m_right = DEFAULT_RIGHT_VECTOR;

    m_scale = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

TransformComponent::TransformComponent(TiXmlElement* pData) {
    Init(pData);
}

const std::string& TransformComponent::VGetName() const {
    return g_Name;
}

TiXmlElement* TransformComponent::VGenerateXml() {
    TiXmlElement* pBaseElement = new TiXmlElement(VGetName().c_str());

    TiXmlElement* pPosition = new TiXmlElement("Position");
    DirectX::XMFLOAT3 pos(m_transform._41, m_transform._42, m_transform._43);
    pPosition->SetAttribute("x", std::to_string(pos.x).c_str());
    pPosition->SetAttribute("y", std::to_string(pos.y).c_str());
    pPosition->SetAttribute("z", std::to_string(pos.z).c_str());
    pBaseElement->LinkEndChild(pPosition);

    TiXmlElement* pDirection = new TiXmlElement("YawPitchRoll");
    DirectX::XMMATRIX tr = DirectX::XMLoadFloat4x4(&m_transform);
    DirectX::XMFLOAT3 orient = GetYawPitchRoll3f();
    orient.x = DirectX::XMConvertToDegrees(orient.x);
    orient.y = DirectX::XMConvertToDegrees(orient.y);
    orient.z = DirectX::XMConvertToDegrees(orient.z);
    pDirection->SetAttribute("x", std::to_string(orient.x).c_str());
    pDirection->SetAttribute("y", std::to_string(orient.y).c_str());
    pDirection->SetAttribute("z", std::to_string(orient.z).c_str());
    pBaseElement->LinkEndChild(pDirection);

    return pBaseElement;
}

const DirectX::XMFLOAT4X4& TransformComponent::GetTransform4x4f() const {
    return m_transform;
}

DirectX::XMMATRIX TransformComponent::GetTransform() const {
    return DirectX::XMLoadFloat4x4(&m_transform);
}

void TransformComponent::SetTransform(const DirectX::XMFLOAT4X4& newTransform) {
    m_transform = newTransform;
}

void TransformComponent::SetTransform(const DirectX::FXMMATRIX& newTransform) {
    DirectX::XMStoreFloat4x4(&m_transform, newTransform);
}

DirectX::XMFLOAT3 TransformComponent::GetPosition3f() const {
    return DirectX::XMFLOAT3(m_transform._41, m_transform._42, m_transform._43);
}

DirectX::XMFLOAT4 TransformComponent::GetPosition4f() const {
    return DirectX::XMFLOAT4(m_transform._41, m_transform._42, m_transform._43, m_transform._44);
}

DirectX::XMVECTOR TransformComponent::GetPosition() const {
    return DirectX::XMVectorSet(m_transform._41, m_transform._42, m_transform._43, 1.0f);
}

DirectX::XMFLOAT3 TransformComponent::GetScale3f() const {
    return DirectX::XMFLOAT3(m_scale.x, m_scale.y, m_scale.z);
}

DirectX::XMVECTOR TransformComponent::GetScale() const {
    return DirectX::XMVectorSet(m_scale.x, m_scale.y, m_scale.z, 1.0f);
}

void TransformComponent::SetPosition3f(const DirectX::XMFLOAT3& pos) {
    m_transform._41 = pos.x;
    m_transform._42 = pos.y;
    m_transform._43 = pos.z;
    m_transform.m[3][3] = 1.0f;
}

void TransformComponent::SetPosition4f(const DirectX::XMFLOAT4& pos) {
    m_transform._41 = pos.x;
    m_transform._42 = pos.y;
    m_transform._43 = pos.z;
    m_transform._44 = pos.w;
}

void TransformComponent::SetPosition4x4f(const DirectX::XMFLOAT4X4& pos) {
    m_transform._41 = pos._41;
    m_transform._42 = pos._42;
    m_transform._43 = pos._43;
    m_transform._44 = pos._44;
}

void TransformComponent::SetPosition3(const DirectX::FXMVECTOR& pos) {
    DirectX::XMFLOAT3 temp;
    DirectX::XMStoreFloat3(&temp, pos);
    m_transform._41 = temp.x;
    m_transform._42 = temp.y;
    m_transform._43 = temp.z;
    m_transform._44 = 1.0f;
}

void TransformComponent::SetPosition4(const DirectX::FXMVECTOR& pos) {
    DirectX::XMFLOAT4 temp;
    DirectX::XMStoreFloat4(&temp, pos);
    m_transform._41 = temp.x;
    m_transform._42 = temp.y;
    m_transform._43 = temp.z;
    m_transform._44 = temp.w;
}

void TransformComponent::SetScale3f(const DirectX::XMFLOAT3& sclae) {
    m_scale.x = sclae.x;
    m_scale.y = sclae.y;
    m_scale.z = sclae.z;
    m_scale.w = 1.0f;
}

void TransformComponent::SetScale4f(const DirectX::XMFLOAT4& sclae) {
    m_scale = sclae;
}

void TransformComponent::SetScale(const DirectX::FXMVECTOR& scale) {
    DirectX::XMFLOAT3 temp;
    DirectX::XMStoreFloat3(&temp, scale);
    m_scale.x = temp.x;
    m_scale.y = temp.y;
    m_scale.z = temp.z;
    m_scale.w = 1.0f;
}

DirectX::XMFLOAT3 TransformComponent::GetLookAt3f() const {
    DirectX::XMFLOAT3 result;
    DirectX::XMStoreFloat3(&result, GetLookAt());

    return result;
}

DirectX::XMVECTOR TransformComponent::GetLookAt() const {
    DirectX::XMFLOAT4X4 justRot4x4 = m_transform;
    justRot4x4._41 = 0.0f;
    justRot4x4._42 = 0.0f;
    justRot4x4._43 = 0.0f;
    justRot4x4._44 = 1.0f;
    DirectX::XMMATRIX justRot = DirectX::XMLoadFloat4x4(&justRot4x4);

    DirectX::XMVECTOR forward = DirectX::XMLoadFloat4(&m_forward);
    DirectX::XMVECTOR out = DirectX::XMVector4Transform(forward, justRot);

    return out;
}

DirectX::XMFLOAT3 TransformComponent::GetForward3f() const {
    return DirectX::XMFLOAT3(m_forward.x, m_forward.y, m_forward.z);
}

DirectX::XMFLOAT4 TransformComponent::GetForward4f() const {
    return m_forward;
}

DirectX::XMVECTOR TransformComponent::GetForward() const {
    DirectX::XMVECTOR directionXM = DirectX::XMLoadFloat4(&m_forward);
    return directionXM;
}

DirectX::XMFLOAT3 TransformComponent::GetUp3f() const {
    return DirectX::XMFLOAT3(m_up.x, m_up.y, m_up.z);
}

DirectX::XMFLOAT4 TransformComponent::GetUp4f() const {
    return m_up;
}

DirectX::XMVECTOR TransformComponent::GetUp() const {
    DirectX::XMVECTOR directionXM = DirectX::XMLoadFloat4(&m_up);
    return directionXM;
}

DirectX::XMFLOAT3 TransformComponent::GetRight3f() const {
    return DirectX::XMFLOAT3(m_right.x, m_right.y, m_right.z);
}

DirectX::XMFLOAT4 TransformComponent::GetRight4f() const {
    return m_right;
}

DirectX::XMVECTOR TransformComponent::GetRight() const {
    DirectX::XMVECTOR directionXM = DirectX::XMLoadFloat4(&m_right);
    return directionXM;
}

bool TransformComponent::Init(TiXmlElement* pData) {
    DirectX::XMFLOAT3 position;
    TiXmlElement* pPositionElement = pData->FirstChildElement("Position");
    if (pPositionElement) {
        double x = 0;
        double y = 0;
        double z = 0;
        pPositionElement->Attribute("x", &x);
        pPositionElement->Attribute("y", &y);
        pPositionElement->Attribute("z", &z);
        position = DirectX::XMFLOAT3(x, y, z);
    }
    else {
        position = GetPosition3f();
    }

    DirectX::XMFLOAT3 yawPitchRoll;
    TiXmlElement* pOrientationElement = pData->FirstChildElement("YawPitchRoll");
    if (pOrientationElement) {
        double yaw = 0;
        double pitch = 0;
        double roll = 0;
        pOrientationElement->Attribute("x", &yaw);
        pOrientationElement->Attribute("y", &pitch);
        pOrientationElement->Attribute("z", &roll);
        const char* at = pOrientationElement->Attribute("angleType");
        std::string angleType(at == nullptr ? "" : at);
        if (angleType == "rad") {
            yawPitchRoll = DirectX::XMFLOAT3(yaw, pitch, roll);
        }
        else {
            yawPitchRoll = DirectX::XMFLOAT3(DirectX::XMConvertToRadians(yaw), DirectX::XMConvertToRadians(pitch), DirectX::XMConvertToRadians(roll));
        }
    }
    else {
        yawPitchRoll = GetYawPitchRoll3f();
    }

    DirectX::XMFLOAT4 scale;
    TiXmlElement* pScaleElement = pData->FirstChildElement("Scale");
    if (pScaleElement) {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        pScaleElement->Attribute("x", &x);
        pScaleElement->Attribute("y", &y);
        pScaleElement->Attribute("z", &z);
        scale = DirectX::XMFLOAT4(x, y, z, 1.0f);
    }
    else {
        scale = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    DirectX::XMMATRIX translationXM = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX rotationXM = DirectX::XMMatrixRotationRollPitchYaw(yawPitchRoll.y, yawPitchRoll.x, yawPitchRoll.z);

    m_scale = scale;
    DirectX::XMStoreFloat4x4(
        &m_transform,
        DirectX::XMMatrixMultiply(
            DirectX::XMMatrixMultiply(
                rotationXM,
                DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
            ),
            translationXM
        )
    );

    return true;
}

DirectX::XMFLOAT3 TransformComponent::GetYawPitchRoll3f() const {

    float pitch = DirectX::XMScalarASin(-m_transform._32);

    DirectX::XMVECTOR from(DirectX::XMVectorSet(m_transform._12, m_transform._31, 0.0f, 0.0f));
    DirectX::XMVECTOR to(DirectX::XMVectorSet(m_transform._22, m_transform._33, 0.0f, 0.0f));
    DirectX::XMVECTOR res(DirectX::XMVectorATan2(from, to));

    float roll = DirectX::XMVectorGetX(res);
    float yaw = DirectX::XMVectorGetY(res);

    return DirectX::XMFLOAT3(yaw, pitch, roll);
}

DirectX::XMVECTOR TransformComponent::GetYawPitchRoll() const {

    DirectX::XMFLOAT3 yawPitchRoll = GetYawPitchRoll3f();
    return DirectX::XMLoadFloat3(&yawPitchRoll);
}

bool TransformComponent::VInit(TiXmlElement* pData) {
    return Init(pData);
}