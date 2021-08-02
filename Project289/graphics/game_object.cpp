#include "game_object.h"

GameObject::GameObject() : m_pos{ 0.0f, 0.0f, 0.0f }, m_rot{ 0.0f, 0.0f, 0.0f } {
	m_forward = DEFAULT_FORWARD_VECTOR;
	m_left = DEFAULT_LEFT_VECTOR;
	m_right = DEFAULT_RIGHT_VECTOR;
	m_backward = DEFAULT_BACKWARD_VECTOR;
}

GameObject::~GameObject() {}

const DirectX::XMVECTOR GameObject::GetPosition() const {
	return DirectX::XMLoadFloat3(&m_pos);
}

const DirectX::XMFLOAT3 GameObject::GetPosition3() const {
	return m_pos;
}

const DirectX::XMVECTOR GameObject::GetRotation() const {
	return DirectX::XMLoadFloat3(&m_rot);
}

const DirectX::XMFLOAT3 GameObject::GetRotation3() const {
	return m_rot;
}

void GameObject::SetPosition(DirectX::FXMVECTOR pos) {
	DirectX::XMStoreFloat3(&m_pos, pos);
	UpdateMatrix();
}

void GameObject::SetPosition(float x, float y, float z) {
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
	UpdateMatrix();
}

void GameObject::AdjustPosition(DirectX::FXMVECTOR pos) {
	DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_pos), pos));
	UpdateMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z) {
	m_pos.x += x;
	m_pos.y += y;
	m_pos.z += z;
	UpdateMatrix();
}

void GameObject::SetRotation(DirectX::FXMVECTOR rot) {
	DirectX::XMStoreFloat3(&m_rot, rot);
	UpdateMatrix();
}

void GameObject::SetRotation(float p, float y, float r) {
	m_rot.x = p;
	m_rot.y = y;
	m_rot.z = r;
	UpdateMatrix();
}

void GameObject::AdjustRotation(DirectX::FXMVECTOR rot) {
	DirectX::XMStoreFloat3(&m_rot, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_rot), rot));
	UpdateMatrix();
}

void GameObject::AdjustRotation(float p, float y, float r) {
	m_rot.x += p;
	m_rot.y += y;
	m_rot.z += r;
	UpdateMatrix();
}

DirectX::XMVECTOR GameObject::GetForward() {
	return DirectX::XMLoadFloat3(&m_forward);
}

DirectX::XMFLOAT3 GameObject::GetForward3() {
	return m_forward;
}

DirectX::XMVECTOR GameObject::GetRight() {
	return DirectX::XMLoadFloat3(&m_right);
}

DirectX::XMFLOAT3 GameObject::GetRight3() {
	return m_right;
}

DirectX::XMVECTOR GameObject::GetBackward() {
	return DirectX::XMLoadFloat3(&m_backward);
}

DirectX::XMFLOAT3 GameObject::GetBackward3() {
	return m_backward;
}

DirectX::XMVECTOR GameObject::GetLeft() {
	return DirectX::XMLoadFloat3(&m_left);
}

DirectX::XMFLOAT3 GameObject::GetLeft3() {
	return m_left;
}

void GameObject::SetLookAtPos(DirectX::FXMVECTOR lookAtPos) {
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_pos);
	if (DirectX::XMVector3NearEqual(lookAtPos, pos, DirectX::XMLoadFloat(&EPSILON))) { return; }

	DirectX::XMVECTOR lookAtDirection = DirectX::XMVectorSubtract(pos, lookAtPos);
	const float x = DirectX::XMVectorGetX(lookAtDirection);
	const float y = DirectX::XMVectorGetY(lookAtDirection);
	const float z = DirectX::XMVectorGetZ(lookAtDirection);


	float pitch = 0.0f;
	if (!DirectX::XMScalarNearEqual(y, 0.0f, EPSILON)) {
		const float distance = DirectX::XMVectorGetX(DirectX::XMVectorSqrt(DirectX::XMVectorSet(x * x + z * z, 0.0f, 0.0f, 0.0f)));
		pitch = DirectX::XMVectorGetX(DirectX::XMVectorATan2(DirectX::XMVectorSet(y, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(distance, 0.0f, 0.0f, 0.0f)));
	}

	float yaw = 0.0f;
	if (!DirectX::XMScalarNearEqual(x, 0.0f, EPSILON)) {
		//yaw = DirectX::XMVectorGetX(DirectX::XMVectorATan2(DirectX::XMVectorSet(x, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(z, 0.0f, 0.0f, 0.0f)));
		yaw = DirectX::XMVectorGetX(DirectX::XMVectorATan(DirectX::XMVectorSet(x / z, 0.0f, 0.0f, 0.0f)));
	}
	if (z > 0.0f) {
		yaw += DirectX::XM_PI;
	}

	SetRotation(pitch, yaw, 0.0f);
}

void GameObject::SetLookAtPos(DirectX::XMFLOAT3 lookAtPos) {
	SetLookAtPos(DirectX::XMLoadFloat3(&lookAtPos));
}

void GameObject::SetLookAtPos(float x, float y, float z) {
	SetLookAtPos(DirectX::XMVectorSet(x, y, z, 1.0f));
}