#include "camera.h"

Camera::Camera() {
	DirectX::XMStoreFloat4x4(&m_projectionMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_viewMatrix, DirectX::XMMatrixIdentity());
}

void Camera::SetProjectionValues(float fov, float aspect_ratio, float near_z, float far_z) {
	DirectX::XMStoreFloat4x4(&m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect_ratio, near_z, far_z));
}

const DirectX::XMMATRIX Camera::GetViewMatrix() const {
	return DirectX::XMLoadFloat4x4(&m_viewMatrix);
}

const DirectX::XMMATRIX Camera::GetViewMatrixT() const {
	return DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_viewMatrix));
}

const DirectX::XMFLOAT4X4 Camera::GetViewMatrix4x4() const {
	return m_viewMatrix;
}

const DirectX::XMFLOAT4X4 Camera::GetViewMatrix4x4T() const {
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_viewMatrix)));
	return res;
}

const DirectX::XMMATRIX Camera::GetProjectionMatrix() const {
	return DirectX::XMLoadFloat4x4(&m_projectionMatrix);
}

const DirectX::XMMATRIX Camera::GetProjectionMatrixT() const {
	return DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_projectionMatrix));
}

const DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix4x4() const {
	return m_projectionMatrix;
}

const DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix4x4T() const {
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_viewMatrix)));
	return res;
}

void Camera::UpdateMatrix() {
	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);
	DirectX::XMVECTOR camDirection = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_FORWARD_VECTOR), camRotationMatrix);
	DirectX::XMVECTOR upDir = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_UP_VECTOR), camRotationMatrix);
	DirectX::XMStoreFloat4x4(&m_viewMatrix, DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&m_pos), camDirection, upDir));

	DirectX::XMMATRIX rotationMartix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, m_rot.y, 0.0f);
	DirectX::XMStoreFloat3(&m_forward, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_FORWARD_VECTOR), rotationMartix));
	DirectX::XMStoreFloat3(&m_backward, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_BACKWARD_VECTOR), rotationMartix));
	DirectX::XMStoreFloat3(&m_left, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_LEFT_VECTOR), rotationMartix));
	DirectX::XMStoreFloat3(&m_right, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_RIGHT_VECTOR), rotationMartix));
}