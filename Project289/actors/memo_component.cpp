#include "memo_component.h"
#include "../tools/memory_utility.h"
#include "transform_component.h"
#include "mesh_render_component.h"
#include "character_stats_component.h"
#include "../engine/engine.h"

const std::string MemoComponent::g_Name = "MemoComponent"s;

const std::string& MemoComponent::VGetName() const {
	return MemoComponent::g_Name;
}

MemoComponent::MemoComponent() {
	m_start_time = 0.0f;
	m_duration = 0.0f;
	m_distance = 0.0f;
	m_condition_present = false;
	m_condition = "";
}

MemoComponent::~MemoComponent() {}

bool MemoComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pStartTime = pData->FirstChildElement("StartTime");
	if (pStartTime) {
		std::string sStartTime = pStartTime->FirstChild()->Value();
		m_start_time = std::stof(sStartTime);
	}

	TiXmlElement* pDuration = pData->FirstChildElement("Duration");
	if (pDuration) {
		std::string sDuration = pDuration->FirstChild()->Value();
		m_duration = std::stof(sDuration);
	}

	TiXmlElement* pDistance = pData->FirstChildElement("Distance");
	if (pDistance) {
		std::string sDistance = pDistance->FirstChild()->Value();
		m_distance = std::stof(sDistance);
	}

	TiXmlElement* pCondition = pData->FirstChildElement("Condition");
	if (pCondition) {
		m_condition_present = true;
		m_condition = pCondition->FirstChild()->Value();
	}

	TiXmlElement* pTarget = pData->FirstChildElement("Target");
	if (pTarget) {
		m_target = pTarget->FirstChild()->Value();
	}

	return true;
}

void MemoComponent::VPostInit() {}

void MemoComponent::VUpdate(float deltaMs) {
	using namespace DirectX;

	BaseEngineLogic* el = g_pApp->GetGameLogic();
	if (el->GetState() != BaseEngineState::BGS_Running) { return; }

	float tt = g_pApp->GetTimer().TotalTime();
	if (tt < m_start_time) { return; }

	std::shared_ptr<MeshRenderComponent> pMeshRenderComponent = MakeStrongPtr(m_pOwner->GetComponent<MeshRenderComponent>(MeshRenderComponent::g_Name));
	std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
	XMFLOAT3 scale = pTransformComponent->GetScale3f();

	std::shared_ptr<CameraNode> camera = g_pApp->GetHumanView()->VGetCamera();
	XMVECTOR cam_pos_xm = XMVectorSetW(camera->GetPosition(), 0.0f);

	XMVECTOR up_xm = camera->GetUp();
	XMVECTOR at_xm = camera->GetDirection();
	XMVECTOR right_xm = XMVector3Cross(up_xm, at_xm);
	
	XMMATRIX rot_xm {
		XMVectorSetW(right_xm, 0.0f),
		XMVectorSetW(up_xm, 0.0f),
		XMVectorSetW(at_xm, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
	};

	XMMATRIX rot_around_y = XMMatrixRotationAxis(up_xm, XM_PI);

	XMMATRIX transform = XMMatrixMultiply(
		XMMatrixMultiply(
			//XMMatrixMultiply(rot_xm, rot_around_y),
			rot_xm,
			XMMatrixScaling(scale.x, scale.y, scale.z)
		),
		XMMatrixTranslationFromVector(cam_pos_xm + at_xm * m_distance)
	);

	/*XMMATRIX transform = DirectX::XMMatrixMultiply(
		rot_xm,
		XMMatrixTranslationFromVector(our_pos_xm + up_xm * m_height)
	);*/

	pTransformComponent->SetTransform(transform);

	if (!pMeshRenderComponent) { return; }

	if (tt >= m_start_time) {
		if(m_condition_present) {
			StrongActorPtr pTargetActor = MakeStrongPtr(el->VGetActorByName(m_target));
			if (!pTargetActor) { return; }
			std::shared_ptr<CharacterStatsComponent> pCharacterStatsComponent = MakeStrongPtr(pTargetActor->GetComponent<CharacterStatsComponent>(CharacterStatsComponent::g_Name));
			if (!pCharacterStatsComponent) { return; }
			int health_level = pCharacterStatsComponent->GetCurrentHealth();
			if (health_level > 0 && m_condition == "win") {
				pMeshRenderComponent->VGetSceneNode()->SetActive(true);
			}
			else if (m_condition == "lose") {
				pMeshRenderComponent->VGetSceneNode()->SetActive(true);
			}
		}
		else {
			pMeshRenderComponent->VGetSceneNode()->SetActive(true);
		}
	}

	if (tt > m_start_time + m_duration) {
		if (m_condition_present) {

		}
		else {
			pMeshRenderComponent->VGetSceneNode()->SetActive(false);
		}
	}
}

TiXmlElement* MemoComponent::VGenerateXml() {
	return nullptr;
}
