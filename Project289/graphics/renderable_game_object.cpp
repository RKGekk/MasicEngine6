#include "renderable_game_object.h"

RenderableGameObject::RenderableGameObject() {
	DirectX::XMStoreFloat4x4(&m_world_matrix, DirectX::XMMatrixIdentity());
}

bool RenderableGameObject::Initialize(ID3D11Device* device, const std::string& filePath, ConstantBuffer<CB_VS_VertexShader>& cbvs) {
	if (!m_model.Initialize(device, filePath, cbvs)) { return false; }

	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	UpdateMatrix();
	return true;
}

void RenderableGameObject::Draw(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix) {
	m_model.Draw(deviceContext, DirectX::XMLoadFloat4x4(&m_world_matrix), viewProjectionMatrix);
}

void RenderableGameObject::UpdateMatrix() {
	DirectX::XMStoreFloat4x4(&m_world_matrix, DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z), DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)));

	DirectX::XMMATRIX rotationMartix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, m_rot.y, 0.0f);
	DirectX::XMStoreFloat3(&m_forward, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_FORWARD_VECTOR), rotationMartix));
	DirectX::XMStoreFloat3(&m_backward, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_BACKWARD_VECTOR), rotationMartix));
	DirectX::XMStoreFloat3(&m_left, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_LEFT_VECTOR), rotationMartix));
	DirectX::XMStoreFloat3(&m_right, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&DEFAULT_RIGHT_VECTOR), rotationMartix));
}