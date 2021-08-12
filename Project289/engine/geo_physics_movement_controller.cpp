#include "geo_physics_movement_controller.h"

#include "engine.h"
#include "../actors/transform_component.h"
#include "../actors/orientation_relation_component.h"
#include "../actors/particle_component.h"
#include "../actors/pers_texture_anim_state_component.h"
#include "../tools/mt_random.h"
#include "../tools/memory_utility.h"

GeoPhysicsMovementController::GeoPhysicsMovementController(std::shared_ptr<SceneNode> object, bool rotateWhenLButtonDown) : m_object(object) {
	using namespace DirectX;

	m_maxSpeed = 30.0f;
	m_currentSpeed = 0.0f;
	m_force = 100.0f;
	m_jump_force = 2000.0f;

	m_matToWorld = m_object->VGet().ToWorld4x4();
	m_matFromWorld = m_object->VGet().FromWorld4x4();

	DirectX::XMStoreFloat4x4(&m_matPosition, DirectX::XMMatrixIdentity());
	m_matPosition._41 = m_matToWorld._41;
	m_matPosition._42 = m_matToWorld._42;
	m_matPosition._43 = m_matToWorld._43;
	m_matPosition._44 = m_matToWorld._44;

	POINT ptCursor;
	GetCursorPos(&ptCursor);
	m_lastMousePos_x = ptCursor.x;
	m_lastMousePos_y = ptCursor.y;
	//ShowCursor(false);
	//SetCapture(hwnd);
	//ReleaseCapture();

	memset(m_bKey, 0x00, sizeof(m_bKey));

	m_mouseRButtonDown = false;
	m_bRotateWhenRButtonDown = rotateWhenLButtonDown;
}

void GeoPhysicsMovementController::SetObject(std::shared_ptr<SceneNode> newObject) {
	m_object = newObject;
}

void GeoPhysicsMovementController::SetState(PersCurrentStateEnum state) {
	if (!m_object) { return; }
	ActorId act_id = m_object->VFindMyActor();
	std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(act_id));
	std::shared_ptr<PersTextureAnimStateComponent> tasc;
	if (act) {
		tasc = MakeStrongPtr(act->GetComponent<PersTextureAnimStateComponent>(ActorComponent::GetIdFromName("PersTextureAnimStateComponent")));
		tasc->SetState(state);
	}
}

PersCurrentStateEnum GeoPhysicsMovementController::GetState() {
	if (!m_object) { return PersCurrentStateEnum::IdleToward; }
	ActorId act_id = m_object->VFindMyActor();
	std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(act_id));
	std::shared_ptr<PersTextureAnimStateComponent> tasc;
	if (act) {
		tasc = MakeStrongPtr(act->GetComponent<PersTextureAnimStateComponent>(ActorComponent::GetIdFromName("PersTextureAnimStateComponent")));
	}
	if (tasc) {
		return tasc->GetState();
	}
	else {
		return PersCurrentStateEnum::IdleToward;
	}
}

std::shared_ptr<PersTextureAnimStateComponent> GeoPhysicsMovementController::GetStateComponent() {
	if (!m_object) { return std::shared_ptr<PersTextureAnimStateComponent>(); }
	ActorId act_id = m_object->VFindMyActor();
	std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(act_id));
	if (act) {
		return MakeStrongPtr(act->GetComponent<PersTextureAnimStateComponent>(ActorComponent::GetIdFromName("PersTextureAnimStateComponent")));
	}
	else {
		return std::shared_ptr<PersTextureAnimStateComponent>();
	}
}

void GeoPhysicsMovementController::DefineStateKeyDown(const BYTE c) {
	if (!(c == 'W' || c == 'S' || c == 'A' || c == 'D' || c == ' ')) { return; }
	
	std::shared_ptr<PersTextureAnimStateComponent> anim_state = GetStateComponent();
	if (!anim_state) { return; }

	PersCurrentStateEnum current_state = anim_state->GetState();
	PersCurrentStateClassEnum current_state_class = anim_state->GetStateClass();
	PersCurrentOrientClassEnum current_orient_class = anim_state->GetOrientClass();

	if (c == 'W') {
		if (current_state_class == PersCurrentStateClassEnum::Walk) {
			anim_state->SetState(PersCurrentStateEnum::WalkOutward);
		}
		if (current_state_class == PersCurrentStateClassEnum::Jump) {
			anim_state->SetState(PersCurrentStateEnum::JumpOutward);
		}
		if (current_state_class == PersCurrentStateClassEnum::Idle) {
			anim_state->SetState(PersCurrentStateEnum::WalkOutward);
		}
	}
	if (c == 'S') {
		if (current_state_class == PersCurrentStateClassEnum::Walk) {
			anim_state->SetState(PersCurrentStateEnum::WalkToward);
		}
		if (current_state_class == PersCurrentStateClassEnum::Jump) {
			anim_state->SetState(PersCurrentStateEnum::JumpToward);
		}
		if (current_state_class == PersCurrentStateClassEnum::Idle) {
			anim_state->SetState(PersCurrentStateEnum::WalkToward);
		}
	}
	if (c == 'A') {
		if (current_state_class == PersCurrentStateClassEnum::Walk) {
			anim_state->SetState(PersCurrentStateEnum::WalkLeft);
		}
		if (current_state_class == PersCurrentStateClassEnum::Jump) {
			anim_state->SetState(PersCurrentStateEnum::JumpLeft);
		}
		if (current_state_class == PersCurrentStateClassEnum::Idle) {
			anim_state->SetState(PersCurrentStateEnum::WalkLeft);
		}
	}
	if (c == 'D') {
		if (current_state_class == PersCurrentStateClassEnum::Walk) {
			anim_state->SetState(PersCurrentStateEnum::WalkRight);
		}
		if (current_state_class == PersCurrentStateClassEnum::Jump) {
			anim_state->SetState(PersCurrentStateEnum::JumpRight);
		}
		if (current_state_class == PersCurrentStateClassEnum::Idle) {
			anim_state->SetState(PersCurrentStateEnum::WalkRight);
		}
	}

	if (c == ' ') {
		if (current_orient_class == PersCurrentOrientClassEnum::Left) {
			anim_state->SetState(PersCurrentStateEnum::JumpLeft);
		}
		if (current_orient_class == PersCurrentOrientClassEnum::Right) {
			anim_state->SetState(PersCurrentStateEnum::JumpRight);
		}
		if (current_orient_class == PersCurrentOrientClassEnum::Outward) {
			anim_state->SetState(PersCurrentStateEnum::JumpOutward);
		}
		if (current_orient_class == PersCurrentOrientClassEnum::Toward) {
			anim_state->SetState(PersCurrentStateEnum::JumpToward);
		}
	}
}

void GeoPhysicsMovementController::DefineStateKeyUp(const BYTE c) {
	if (!(c == 'W' || c == 'S' || c == 'A' || c == 'D' || c == ' ')) { return; }

	std::shared_ptr<PersTextureAnimStateComponent> anim_state = GetStateComponent();
	if (!anim_state) { return; }

	PersCurrentStateEnum current_state = anim_state->GetState();
	PersCurrentStateClassEnum current_state_class = anim_state->GetStateClass();
	PersCurrentOrientClassEnum current_orient_class = anim_state->GetOrientClass();

	if (c == 'W') {
		if (current_state_class == PersCurrentStateClassEnum::Walk) {
			if (m_bKey['A']) {
				anim_state->SetState(PersCurrentStateEnum::WalkLeft);
			}
			else if (m_bKey['D']) {
				anim_state->SetState(PersCurrentStateEnum::WalkRight);
			}
			else {
				anim_state->SetState(PersCurrentStateEnum::IdleOutward);
			}
		}
		if (current_state_class == PersCurrentStateClassEnum::Jump) {
			if (m_bKey['A']) {
				anim_state->SetState(PersCurrentStateEnum::JumpLeft);
			}
			else if (m_bKey['D']) {
				anim_state->SetState(PersCurrentStateEnum::JumpRight);
			}
		}
	}
	if (c == 'S') {
		if (current_state_class == PersCurrentStateClassEnum::Walk) {
			if (m_bKey['A']) {
				anim_state->SetState(PersCurrentStateEnum::WalkLeft);
			}
			else if (m_bKey['D']) {
				anim_state->SetState(PersCurrentStateEnum::WalkRight);
			}
			else {
				anim_state->SetState(PersCurrentStateEnum::IdleToward);
			}
		}
		if (current_state_class == PersCurrentStateClassEnum::Jump) {
			if (m_bKey['A']) {
				anim_state->SetState(PersCurrentStateEnum::JumpLeft);
			}
			else if (m_bKey['D']) {
				anim_state->SetState(PersCurrentStateEnum::JumpRight);
			}
		}
	}
	if (c == 'A') {
		if (current_state_class == PersCurrentStateClassEnum::Walk) {
			if (m_bKey['W']) {
				anim_state->SetState(PersCurrentStateEnum::WalkOutward);
			}
			else if (m_bKey['S']) {
				anim_state->SetState(PersCurrentStateEnum::WalkToward);
			}
			else {
				anim_state->SetState(PersCurrentStateEnum::IdleLeft);
			}
		}
		if (current_state_class == PersCurrentStateClassEnum::Jump) {
			if (m_bKey['W']) {
				anim_state->SetState(PersCurrentStateEnum::JumpOutward);
			}
			else if (m_bKey['S']) {
				anim_state->SetState(PersCurrentStateEnum::JumpToward);
			}
		}
	}
	if (c == 'D') {
		if (current_state_class == PersCurrentStateClassEnum::Walk) {
			if (m_bKey['W']) {
				anim_state->SetState(PersCurrentStateEnum::WalkOutward);
			}
			else if (m_bKey['S']) {
				anim_state->SetState(PersCurrentStateEnum::WalkToward);
			}
			else {
				anim_state->SetState(PersCurrentStateEnum::IdleRight);
			}
		}
		if (current_state_class == PersCurrentStateClassEnum::Jump) {
			if (m_bKey['W']) {
				anim_state->SetState(PersCurrentStateEnum::JumpOutward);
			}
			else if (m_bKey['S']) {
				anim_state->SetState(PersCurrentStateEnum::JumpToward);
			}
		}
	}

	if (c == ' ') {
		if (m_bKey['W']) {
			anim_state->SetState(PersCurrentStateEnum::WalkOutward);
		}
		else if (m_bKey['S']) {
			anim_state->SetState(PersCurrentStateEnum::WalkToward);
		}
		else if (m_bKey['A']) {
			anim_state->SetState(PersCurrentStateEnum::WalkLeft);
		}
		else if (m_bKey['D']) {
			anim_state->SetState(PersCurrentStateEnum::WalkRight);
		}
		else {
			anim_state->SetState(PersCurrentStateEnum::IdleToward);
		}
	}
}

void GeoPhysicsMovementController::OnUpdate(float elapsed_seconds) {
	using namespace DirectX;

	if (!m_object) { return; }
	ActorId act_id = m_object->VFindMyActor();
	std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(act_id));
	std::shared_ptr<ParticleComponent> pc;
	std::shared_ptr<OrientationRelationComponent> oc;
	std::shared_ptr<TransformComponent> tc;
	if (act) {
		pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
		oc = MakeStrongPtr(act->GetComponent<OrientationRelationComponent>(ActorComponent::GetIdFromName("OrientationRelationComponent")));
		tc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
	}
	bool is_pcoc = pc && oc;

	bool bTranslating = false;
	XMFLOAT4 atWorld(0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT4 rightWorld(0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT4 upWorld(0.0f, 0.0f, 0.0f, 0.0f);

	if (m_bKey['W'] || m_bKey['S']) {
		XMFLOAT4 at;
		if (is_pcoc) { at = oc->VGetAt4f(); }
		else { at = {0.0f, 0.0f, 1.0f, 0.0f}; }
		if (m_bKey['S']) {
			at.x *= -1.0f;
			at.y *= -1.0f;
			at.z *= -1.0f;
		}
		if (is_pcoc) {
			atWorld = at;
		}
		else {
			XMVECTOR at_xm = XMLoadFloat4(&at);
			XMStoreFloat4(&atWorld, XMVector4Transform(at_xm, XMLoadFloat4x4(&m_matToWorld)));
		}
		bTranslating = true;
	}

	if (m_bKey['A'] || m_bKey['D']) {
		XMFLOAT4 right;
		if (is_pcoc) { right = oc->VGetRight4f(); }
		else { right = {1.0f, 0.0f, 0.0, 0.0f}; }
		if (m_bKey['A']) {
			right.x *= -1.0f;
			right.y *= -1.0f;
			right.z *= -1.0f;
		}
		if (is_pcoc) {
			rightWorld = right;
		}
		else {
			XMVECTOR right_xm = XMLoadFloat4(&right);
			XMStoreFloat4(&rightWorld, XMVector4Transform(right_xm, XMLoadFloat4x4(&m_matToWorld)));
		}
		bTranslating = true;
	}

	if (m_bKey[' '] || m_bKey['C'] || m_bKey['X']) {
		XMFLOAT4 up;
		if (is_pcoc) { up = oc->VGetUp4f(); }
		else { up = {0.0f, 1.0f, 0.0f, 0.0f}; }
		if (!m_bKey[' ']) {
			up.x *= -1.0f;
			up.y *= -1.0f;
			up.z *= -1.0f;
		}
		upWorld = up;
		bTranslating = true;
	}

	if (bTranslating) {

		if (pc) {
			XMVECTOR direction = XMLoadFloat4(&atWorld) * m_force + XMLoadFloat4(&rightWorld) * m_force + XMLoadFloat4(&upWorld) * m_jump_force;

			Particle& particle = pc->VGetParticle();
			particle.setAwake();
			particle.addForce(direction);
			particle.integrate(elapsed_seconds);
		}
		else {
			XMVECTOR direction = XMLoadFloat4(&atWorld) + XMLoadFloat4(&rightWorld) + XMLoadFloat4(&upWorld);
			direction = XMVector3Normalize(direction);

			float numberOfSeconds = 5.0f;
			m_currentSpeed += m_maxSpeed * ((elapsed_seconds * elapsed_seconds) / numberOfSeconds);
			if (m_currentSpeed > m_maxSpeed) { m_currentSpeed = m_maxSpeed; }

			XMVECTOR pos = XMVectorSet(m_matPosition._41, m_matPosition._42, m_matPosition._43, m_matPosition._44) + direction * m_currentSpeed;
			m_matPosition._41 = XMVectorGetX(pos);
			m_matPosition._42 = XMVectorGetY(pos);
			m_matPosition._43 = XMVectorGetZ(pos);

			m_matToWorld._41 = XMVectorGetX(pos);
			m_matToWorld._42 = XMVectorGetY(pos);
			m_matToWorld._43 = XMVectorGetZ(pos);

			XMStoreFloat4x4(&m_matFromWorld, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matToWorld)));

			m_object->VSetTransform4x4(&m_matToWorld, &m_matFromWorld);
		}
	}
}

bool GeoPhysicsMovementController::VOnPointerMove(int x, int y, const int radius) {
	using namespace DirectX;

	if (m_bRotateWhenRButtonDown) {
		if ((m_lastMousePos_x != x || m_lastMousePos_y != y) && m_mouseRButtonDown) {
			int dx = m_lastMousePos_x - x;
			//int dy = y - m_lastMousePos_y;
			int dy = m_lastMousePos_y - y;

			m_lastMousePos_x = x;
			m_lastMousePos_y = y;

			if (!m_object) { return true; }

			ActorId act_id = m_object->VFindMyActor();
			std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(act_id));
			std::shared_ptr<OrientationRelationComponent> oc;
			std::shared_ptr<TransformComponent> tc;
			if (act) {
				oc = MakeStrongPtr(act->GetComponent<OrientationRelationComponent>(ActorComponent::GetIdFromName("OrientationRelationComponent")));
				tc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
			}
			bool is_octc = oc && tc;

			if (is_octc) {
				//XMMATRIX new_transform = XMMatrixMultiply(tc->GetTransform(), XMMatrixRotationAxis(oc->VGetUp(), XMConvertToRadians(((float)dy) * 0.1f)));
				//tc->SetTransform(new_transform);
				oc->VRotateUp(((float)dx) * 0.5f);
			}
		}
	}
	else {
		if (m_lastMousePos_x != x || m_lastMousePos_y != y) {
			int dx = m_lastMousePos_x - x;
			//int dy = y - m_lastMousePos_y;
			int dy = m_lastMousePos_y - y;

			m_lastMousePos_x = x;
			m_lastMousePos_y = y;

			if (!m_object) { return true; }

			ActorId act_id = m_object->VFindMyActor();
			std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(act_id));
			std::shared_ptr<OrientationRelationComponent> oc;
			std::shared_ptr<TransformComponent> tc;
			if (act) {
				oc = MakeStrongPtr(act->GetComponent<OrientationRelationComponent>(ActorComponent::GetIdFromName("OrientationRelationComponent")));
				tc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
			}
			bool is_octc = oc && tc;

			if (is_octc) {
				//XMMATRIX new_transform = XMMatrixMultiply(tc->GetTransform(), XMMatrixRotationAxis(oc->VGetUp(), XMConvertToRadians(((float)dy) * 0.1f)));
				//tc->SetTransform(new_transform);
				oc->VRotateUp(((float)dx) * 0.5f);
			}
		}
	}

	return true;
}

bool GeoPhysicsMovementController::VOnPointerButtonDown(int x, int y, const int radius, const std::string& buttonName) {
	if (buttonName == "PointerRight") {
		m_mouseRButtonDown = true;
		m_lastMousePos_x = x;
		m_lastMousePos_y = y;
		return true;
	}
	return false;
}

bool GeoPhysicsMovementController::VOnPointerButtonUp(int x, int y, const int radius, const std::string& buttonName) {
	if (buttonName == "PointerRight") {
		m_mouseRButtonDown = false;
		return true;
	}
	return false;
}

bool GeoPhysicsMovementController::VOnKeyDown(const BYTE c) {
	DefineStateKeyDown(c);

	m_bKey[c] = true;
	return true;
}

bool GeoPhysicsMovementController::VOnKeyUp(const BYTE c) {
	DefineStateKeyUp(c);

	if (c == 'I') {
		g_pApp->GetHumanView()->VSetControlledActor(INVALID_ACTOR_ID);
	}
	m_bKey[c] = false;
	return true;
}

const DirectX::XMFLOAT4X4& GeoPhysicsMovementController::GetToWorld4x4() {
	return m_matToWorld;
}

DirectX::XMMATRIX GeoPhysicsMovementController::GetToWorld() {
	return DirectX::XMLoadFloat4x4(&m_matToWorld);
}

const DirectX::XMFLOAT4X4& GeoPhysicsMovementController::GetFromWorld4x4() {
	return m_matFromWorld;
}

DirectX::XMMATRIX GeoPhysicsMovementController::GetFromWorld() {
	return DirectX::XMLoadFloat4x4(&m_matFromWorld);
}