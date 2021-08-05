#include "pers_texture_anim_state_component.h"
#include "particle_component.h"
#include "../tools/memory_utility.h"

const std::string PersTextureAnimStateComponent::g_Name = "PersTextureAnimStateComponent"s;

const std::string& PersTextureAnimStateComponent::VGetName() const {
	return PersTextureAnimStateComponent::g_Name;
}


PersTextureAnimStateComponent::PersTextureAnimStateComponent() {
	DirectX::XMStoreFloat4x4(&m_text_transform, DirectX::XMMatrixIdentity());

	m_current_state = PersCurrentStateEnum::IdleToward;

	m_atlas_width = 1;
	m_atlas_height = 1;
	
	m_total_time = 0.0f;

	m_walk_left_from = 1;
	m_walk_left_to = 1;
	m_walk_right_from = 1;
	m_walk_right_to = 1;
	m_walk_toward_from = 1;
	m_walk_toward_to = 1;
	m_walk_outward_from = 1;
	m_walk_outward_to = 1;

	m_jump_left_from = 1;
	m_jump_left_to = 1;
	m_jump_right_from = 1;
	m_jump_right_to = 1;
	m_jump_toward_from = 1;
	m_jump_toward_to = 1;
	m_jump_outward_from = 1;
	m_jump_outward_to = 1;

	m_idle_left_from = 1;
	m_idle_left_to = 1;
	m_idle_right_from = 1;
	m_idle_right_to = 1;
	m_idle_toward_from = 1;
	m_idle_toward_to = 1;
	m_idle_outward_from = 1;
	m_idle_outward_to = 1;
}

PersTextureAnimStateComponent::~PersTextureAnimStateComponent() {}

bool PersTextureAnimStateComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pAtlasWidth = pData->FirstChildElement("AtlasWidth");
	if (pAtlasWidth) {
		std::string sAtlasWidth = pAtlasWidth->FirstChild()->Value();
		m_atlas_width = std::stoi(sAtlasWidth);
	}

	TiXmlElement* pAtlasHeight = pData->FirstChildElement("AtlasHeight");
	if (pAtlasHeight) {
		std::string sAtlasHeight = pAtlasHeight->FirstChild()->Value();
		m_atlas_height = std::stoi(sAtlasHeight);
	}

	TiXmlElement* pWalkLeft = pData->FirstChildElement("WalkLeft");
	if (pWalkLeft) {
		const char* cFrom = pWalkLeft->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_walk_left_from = std::stoi(sFrom);
		}
		const char* cTo = pWalkLeft->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_walk_left_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pWalkRight = pData->FirstChildElement("WalkRight");
	if (pWalkRight) {
		const char* cFrom = pWalkRight->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_walk_right_from= std::stoi(sFrom);
		}
		const char* cTo = pWalkRight->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_walk_right_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pWalkToward = pData->FirstChildElement("WalkToward");
	if (pWalkToward) {
		const char* cFrom = pWalkToward->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_walk_toward_from = std::stoi(sFrom);
		}
		const char* cTo = pWalkToward->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_walk_toward_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pWalkOutward = pData->FirstChildElement("WalkOutward");
	if (pWalkOutward) {
		const char* cFrom = pWalkOutward->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_walk_outward_from = std::stoi(sFrom);
		}
		const char* cTo = pWalkOutward->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_walk_outward_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pJumpLeft = pData->FirstChildElement("JumpLeft");
	if (pJumpLeft) {
		const char* cFrom = pJumpLeft->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_jump_left_from = std::stoi(sFrom);
		}
		const char* cTo = pJumpLeft->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_jump_left_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pJumpRight = pData->FirstChildElement("JumpRight");
	if (pJumpRight) {
		const char* cFrom = pJumpRight->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_jump_right_from = std::stoi(sFrom);
		}
		const char* cTo = pJumpRight->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_jump_right_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pJumpToward = pData->FirstChildElement("JumpToward");
	if (pJumpToward) {
		const char* cFrom = pJumpToward->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_jump_toward_from = std::stoi(sFrom);
		}
		const char* cTo = pJumpToward->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_jump_toward_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pJumpOutward = pData->FirstChildElement("JumpOutward");
	if (pJumpOutward) {
		const char* cFrom = pJumpOutward->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_jump_outward_from = std::stoi(sFrom);
		}
		const char* cTo = pJumpOutward->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_jump_outward_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pIdleLeft = pData->FirstChildElement("IdleLeft");
	if (pIdleLeft) {
		const char* cFrom = pIdleLeft->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_idle_left_from = std::stoi(sFrom);
		}
		const char* cTo = pIdleLeft->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_idle_left_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pIdleRight = pData->FirstChildElement("IdleRight");
	if (pIdleRight) {
		const char* cFrom = pIdleRight->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_idle_right_from = std::stoi(sFrom);
		}
		const char* cTo = pIdleRight->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_idle_right_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pIdleToward = pData->FirstChildElement("IdleToward");
	if (pIdleToward) {
		const char* cFrom = pIdleToward->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_idle_toward_from = std::stoi(sFrom);
		}
		const char* cTo = pIdleToward->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_idle_toward_to = std::stoi(sTo);
		}
	}

	TiXmlElement* pIdleOutward = pData->FirstChildElement("IdleOutward");
	if (pIdleOutward) {
		const char* cFrom = pIdleOutward->Attribute("From");
		std::string sFrom(cFrom == nullptr ? "" : cFrom);
		if (!sFrom.empty()) {
			m_idle_outward_from = std::stoi(sFrom);
		}
		const char* cTo = pIdleOutward->Attribute("To");
		std::string sTo(cTo == nullptr ? "" : cTo);
		if (!sTo.empty()) {
			m_idle_outward_to = std::stoi(sTo);
		}
	}

	return true;
}

void PersTextureAnimStateComponent::VPostInit() {}

void PersTextureAnimStateComponent::VUpdate(float deltaMs) {
	using namespace DirectX;
	std::shared_ptr<ParticleComponent> pParticleComponent = MakeStrongPtr(GetOwner()->GetComponent<ParticleComponent>(ParticleComponent::g_Name));

	//PersCurrentStateEnum state = m_current_state;
	PersCurrentStateEnum state = PersCurrentStateEnum::WalkOutward;


	float frame_u = 1.0f / ((float)m_atlas_width);
	float frame_v = 1.0f / ((float)m_atlas_height);

	XMMATRIX scale_to_one_frame = XMMatrixScaling(frame_u, frame_v, 1.0f);
	XMMATRIX translation = XMMatrixTranslation(frame_u * 0.0f, frame_v * 2.0f, 0.0f);
	//XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	XMMATRIX transform = XMMatrixMultiply(scale_to_one_frame, translation);
	XMStoreFloat4x4(&m_text_transform, transform);
}

DirectX::XMFLOAT4X4 PersTextureAnimStateComponent::GetTexTransform() {
	return m_text_transform;
}

TiXmlElement* PersTextureAnimStateComponent::VGenerateXml() {
	return nullptr;
}
