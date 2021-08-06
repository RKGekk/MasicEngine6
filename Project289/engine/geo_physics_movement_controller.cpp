#include "geo_physics_movement_controller.h"

#include "engine.h"
#include "../actors/transform_component.h"
#include "../actors/orientation_relation_component.h"
#include "../actors/particle_component.h"
#include "../actors/pers_texture_anim_state_component.h"
#include "../tools/mt_random.h"
#include "../tools/memory_utility.h"

GeoPhysicsMovementController::GeoPhysicsMovementController(std::shared_ptr<SceneNode> object, std::shared_ptr<SceneNode> camera) : m_object(object), m_camera(camera) {
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

	memset(m_bKey, 0x00, sizeof(m_bKey));
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
			/*XMVECTOR direction = XMLoadFloat4(&atWorld) + XMLoadFloat4(&rightWorld) + XMLoadFloat4(&upWorld);
			direction = XMVector3Normalize(direction);*/
			XMVECTOR direction = XMLoadFloat4(&atWorld) * m_force + XMLoadFloat4(&rightWorld) * m_force + XMLoadFloat4(&upWorld) * m_jump_force;

			Particle& particle = pc->VGetParticle();
			particle.setAwake();
			particle.addForce(direction);
			particle.integrate(elapsed_seconds);

			if (!m_camera) { return; }

			//XMMATRIX mat = tc->GetTransform();
			//XMVECTOR at = oc->VGetAt() * -5.0f;
			//XMVECTOR atWorld = DirectX::XMVector4Transform(at, mat);
			//mat.r[3] += atWorld;
			//m_camera->VSetTransform(mat, XMMatrixIdentity(), true);


			//DirectX::XMMATRIX matRot = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f), 0.0f);
			//DirectX::XMMATRIX matRot = tc->GetTransform();
			//matRot.r[3] = DirectX::XMVectorSet(m_matPosition._41, m_matPosition._42, m_matPosition._43, m_matPosition._44);
			//matRot.r[0] = tc->GetPosition() + oc->VGetAt() * -5.0f;
			//matRot.r[1] = tc->GetPosition() + oc->VGetAt() * -5.0f;
			//matRot.r[2] = tc->GetPosition() + oc->VGetAt() * -5.0f;
			//matRot.r[3] = tc->GetPosition() + oc->VGetAt() * -5.0f + oc->VGetUp() * -1.0f;
			
			//m_camera->VSetTransform(matRot, XMMatrixIdentity(), true);
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
	return true;
}

bool GeoPhysicsMovementController::VOnPointerButtonDown(int x, int y, const int radius, const std::string& buttonName) {
	return false;
}

bool GeoPhysicsMovementController::VOnPointerButtonUp(int x, int y, const int radius, const std::string& buttonName) {
	return false;
}

bool GeoPhysicsMovementController::VOnKeyDown(const BYTE c) {
	if (c == 'W') {
		SetState(PersCurrentStateEnum::WalkOutward);
	}
	if (c == 'S') {
		SetState(PersCurrentStateEnum::WalkToward);
	}
	if (c == 'A') {
		SetState(PersCurrentStateEnum::WalkLeft);
	}
	if (c == 'D') {
		SetState(PersCurrentStateEnum::WalkRight);
	}

	if (c == ' ') {
		if (m_bKey['W']) {
			SetState(PersCurrentStateEnum::JumpOutward);
		}
		else if (m_bKey['S']) {
			SetState(PersCurrentStateEnum::JumpToward);
		}
		else if (m_bKey['A']) {
			SetState(PersCurrentStateEnum::JumpLeft);
		}
		else if (m_bKey['D']) {
			SetState(PersCurrentStateEnum::JumpRight);
		}
		else {
			SetState(PersCurrentStateEnum::IdleToward);
		}
	}

	m_bKey[c] = true;
	return true;
}

bool GeoPhysicsMovementController::VOnKeyUp(const BYTE c) {
	if (c == 'W' || c == 'S' || c == 'A' || c == 'D' || c == ' ') {
		SetState(PersCurrentStateEnum::IdleToward);
	}

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