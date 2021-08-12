#include "movement_controller.h"
#include "engine.h"
#include "../actors/transform_component.h"
#include "human_view.h"

MovementController::MovementController(std::shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown) : m_object(object) {
	m_matToWorld = m_object->VGet().ToWorld4x4();
	m_matFromWorld = m_object->VGet().FromWorld4x4();

	m_fTargetYaw = m_fYaw = DirectX::XMConvertToDegrees(-initialYaw);
	m_fTargetPitch = m_fPitch = DirectX::XMConvertToDegrees(initialPitch);

	m_maxSpeed = 30.0f;
	m_currentSpeed = 0.0f;

	DirectX::XMStoreFloat4x4(&m_matPosition, DirectX::XMMatrixIdentity());
	m_matPosition._41 = m_matToWorld._41;
	m_matPosition._42 = m_matToWorld._42;
	m_matPosition._43 = m_matToWorld._43;
	m_matPosition._44 = m_matToWorld._44;

	POINT ptCursor;
	GetCursorPos(&ptCursor);
	m_lastMousePos_x = ptCursor.x;
	m_lastMousePos_y = ptCursor.y;
	//ShowCursor(true);

	memset(m_bKey, 0x00, sizeof(m_bKey));

	m_mouseLButtonDown = false;
	m_bRotateWhenLButtonDown = rotateWhenLButtonDown;
}

void MovementController::SetObject(std::shared_ptr<SceneNode> newObject) {
	m_object = newObject;
}

void MovementController::OnUpdate(float elapsed_seconds) {
	using namespace DirectX;

	if (!m_object) { return; }
	ActorId act_id = m_object->VFindMyActor();
	std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(act_id));
	unsigned int componentId = ActorComponent::GetIdFromName("TransformComponent");
	std::shared_ptr<TransformComponent> tc;
	if (act) {
		tc = MakeStrongPtr(act->GetComponent<TransformComponent>(componentId));
	}

	bool bTranslating = false;
	DirectX::XMFLOAT4 atWorld(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 rightWorld(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 upWorld(0.0f, 0.0f, 0.0f, 0.0f);

	if (m_bKey['W'] || m_bKey['S']) {
		DirectX::XMFLOAT4 at(0.0f, 0.0f, 1.0f, 0.0f);
		if (m_bKey['S']) {
			at.x *= -1.0f;
			at.y *= -1.0f;
			at.z *= -1.0f;
		}
		DirectX::XMVECTOR at_xm = DirectX::XMLoadFloat4(&at);
		DirectX::XMStoreFloat4(&atWorld, DirectX::XMVector4Transform(at_xm, DirectX::XMLoadFloat4x4(&m_matToWorld)));
		bTranslating = true;
	}

	if (m_bKey['A'] || m_bKey['D']) 	{
		DirectX::XMFLOAT4 right(1.0f, 0.0f, 0.0f, 0.0f);
		if (m_bKey['A']) {
			right.x *= -1.0f;
			right.y *= -1.0f;
			right.z *= -1.0f;
		}
		DirectX::XMVECTOR right_xm = DirectX::XMLoadFloat4(&right);
		DirectX::XMStoreFloat4(&rightWorld, DirectX::XMVector4Transform(right_xm, DirectX::XMLoadFloat4x4(&m_matToWorld)));
		bTranslating = true;
	}

	if (m_bKey[' '] || m_bKey['C'] || m_bKey['X']) 	{
		DirectX::XMFLOAT4 up(0.0f, 1.0f, 0.0f, 0.0f);
		if (!m_bKey[' ']) {
			up.x *= -1.0f;
			up.y *= -1.0f;
			up.z *= -1.0f;
		}

		upWorld = up;
		bTranslating = true;
	}

	{
		m_fYaw += (m_fTargetYaw - m_fYaw) * (0.35f);
		m_fTargetPitch = std::max(-90.0f, std::min(90.0f, m_fTargetPitch));
		m_fPitch += (m_fTargetPitch - m_fPitch) * (0.35f);

		DirectX::XMMATRIX matRot = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(m_fPitch), DirectX::XMConvertToRadians(-m_fYaw), 0.0f);
		matRot.r[3] = DirectX::XMVectorSet(m_matPosition._41, m_matPosition._42, m_matPosition._43, m_matPosition._44);
		DirectX::XMStoreFloat4x4(&m_matToWorld, matRot);
		DirectX::XMStoreFloat4x4(&m_matFromWorld, DirectX::XMMatrixInverse(nullptr, matRot));
		if (tc) {
			tc->SetTransform(m_matToWorld);
		}
		else {
			m_object->VSetTransform4x4(&m_matToWorld, &m_matFromWorld);
		}
	}

	if (bTranslating) {

		DirectX::XMVECTOR direction = DirectX::XMLoadFloat4(&atWorld) + DirectX::XMLoadFloat4(&rightWorld) + DirectX::XMLoadFloat4(&upWorld);
		direction = DirectX::XMVector3Normalize(direction);

		float numberOfSeconds = 5.0f;
		m_currentSpeed += m_maxSpeed * ((elapsed_seconds * elapsed_seconds) / numberOfSeconds);
		if (m_currentSpeed > m_maxSpeed) { m_currentSpeed = m_maxSpeed; }

		direction *= m_currentSpeed;

		DirectX::XMVECTOR pos = DirectX::XMVectorSet(m_matPosition._41, m_matPosition._42, m_matPosition._43, m_matPosition._44) + direction;
		m_matPosition._41 = DirectX::XMVectorGetX(pos);
		m_matPosition._42 = DirectX::XMVectorGetY(pos);
		m_matPosition._43 = DirectX::XMVectorGetZ(pos);

		m_matToWorld._41 = DirectX::XMVectorGetX(pos);
		m_matToWorld._42 = DirectX::XMVectorGetY(pos);
		m_matToWorld._43 = DirectX::XMVectorGetZ(pos);

		DirectX::XMStoreFloat4x4(&m_matFromWorld, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&m_matToWorld)));
		if (tc) {
			tc->SetTransform(m_matToWorld);
		}
		else {
			m_object->VSetTransform4x4(&m_matToWorld, &m_matFromWorld);
		}
	}
	else {
		m_currentSpeed = 0.0f;
	}
}

bool MovementController::VOnPointerMove(int x, int y, const int radius) {
	if (m_bRotateWhenLButtonDown) {
		if ((m_lastMousePos_x != x || m_lastMousePos_y != y) && m_mouseLButtonDown) {
			m_fTargetYaw = m_fTargetYaw + (m_lastMousePos_x - x);
			m_fTargetPitch = m_fTargetPitch + (y - m_lastMousePos_y);
			m_lastMousePos_x = x;
			m_lastMousePos_y = y;
		}
	}
	else if (m_lastMousePos_x != x || m_lastMousePos_y != y) {
		m_fTargetYaw = m_fTargetYaw + (m_lastMousePos_x - x);
		m_fTargetPitch = m_fTargetPitch + (y - m_lastMousePos_y);
		m_lastMousePos_x = x;
		m_lastMousePos_y = y;
	}

	return true;
}

bool MovementController::VOnPointerButtonDown(int x, int y, const int radius, const std::string& buttonName) {
	if (buttonName == "PointerLeft") {
		m_mouseLButtonDown = true;
		m_lastMousePos_x = x;
		m_lastMousePos_y = y;
		return true;
	}
	return false;
}

bool MovementController::VOnPointerButtonUp(int x, int y, const int radius, const std::string& buttonName) {
	if (buttonName == "PointerLeft") {
		m_mouseLButtonDown = false;
		return true;
	}
	return false;
}

bool MovementController::VOnKeyDown(const BYTE c) {
	m_bKey[c] = true;
	return true;
}

bool MovementController::VOnKeyUp(const BYTE c) {
	m_bKey[c] = false;
	return true;
}

const DirectX::XMFLOAT4X4& MovementController::GetToWorld4x4() {
	return m_matToWorld;
}

DirectX::XMMATRIX MovementController::GetToWorld() {
	return DirectX::XMLoadFloat4x4(&m_matToWorld);
}

const DirectX::XMFLOAT4X4& MovementController::GetFromWorld4x4() {
	return m_matFromWorld;
}

DirectX::XMMATRIX MovementController::GetFromWorld() {
	return DirectX::XMLoadFloat4x4(&m_matFromWorld);
}