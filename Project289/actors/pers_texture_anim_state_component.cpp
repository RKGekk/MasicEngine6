#include "pers_texture_anim_state_component.h"
#include "particle_component.h"
#include "../tools/memory_utility.h"

#include <cmath>

const std::string PersTextureAnimStateComponent::g_Name = "PersTextureAnimStateComponent"s;

const std::string& PersTextureAnimStateComponent::VGetName() const {
	return PersTextureAnimStateComponent::g_Name;
}

PersTextureAnimStateComponent::PersTextureAnimStateComponent() {
	DirectX::XMStoreFloat4x4(&m_text_transform, DirectX::XMMatrixIdentity());
	m_anim_time = 0.0f;
	m_current_state = PersCurrentStateEnum::IdleToward;

	m_atlas_width = 1;
	m_atlas_height = 1;
	
	m_total_time = 0.0f;

	m_walk_left_from = 1;
	m_walk_left_to = 1;
	m_walk_left_frame_time = 0.016f;
	m_walk_right_from = 1;
	m_walk_right_to = 1;
	m_walk_right_frame_time = 0.016f;
	m_walk_toward_from = 1;
	m_walk_toward_to = 1;
	m_walk_toward_frame_time = 0.016f;
	m_walk_outward_from = 1;
	m_walk_outward_to = 1;
	m_walk_outward_frame_time = 0.016f;

	m_jump_left_from = 1;
	m_jump_left_to = 1;
	m_jump_left_frame_time = 0.016f;
	m_jump_right_from = 1;
	m_jump_right_to = 1;
	m_jump_right_frame_time = 0.016f;
	m_jump_toward_from = 1;
	m_jump_toward_to = 1;
	m_jump_toward_frame_time = 0.016f;
	m_jump_outward_from = 1;
	m_jump_outward_to = 1;
	m_jump_outward_frame_time = 0.016f;

	m_idle_left_from = 1;
	m_idle_left_to = 1;
	m_idle_left_frame_time = 0.016f;
	m_idle_right_from = 1;
	m_idle_right_to = 1;
	m_idle_right_frame_time = 0.016f;
	m_idle_toward_from = 1;
	m_idle_toward_to = 1;
	m_idle_toward_frame_time = 0.016f;
	m_idle_outward_from = 1;
	m_idle_outward_to = 1;
	m_idle_outward_frame_time = 0.016f;
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
		const char* cFrameTime = pWalkLeft->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_walk_left_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pWalkRight->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_walk_right_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pWalkToward->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_walk_toward_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pWalkOutward->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_walk_outward_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pJumpLeft->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_jump_left_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pJumpRight->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_jump_right_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pJumpToward->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_jump_toward_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pJumpOutward->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_jump_outward_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pIdleLeft->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_idle_left_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pIdleRight->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_idle_right_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pIdleToward->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_idle_toward_frame_time = std::stof(sFrameTime);
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
		const char* cFrameTime = pIdleOutward->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		if (!sFrameTime.empty()) {
			m_idle_outward_frame_time = std::stof(sFrameTime);
		}
	}

	return true;
}

void PersTextureAnimStateComponent::VPostInit() {}

void PersTextureAnimStateComponent::VUpdate(float deltaMs) {
	using namespace DirectX;
	std::shared_ptr<ParticleComponent> pParticleComponent = MakeStrongPtr(GetOwner()->GetComponent<ParticleComponent>(ParticleComponent::g_Name));

	PersCurrentStateEnum state = m_current_state;
	int start_frame_num = 0;
	int total_frames_num = 0;
	int frames_per_row = m_atlas_width;
	float frame_time = 0.5f;
	switch (state) 	{
	case PersCurrentStateEnum::WalkLeft:
		start_frame_num = m_walk_left_from;
		total_frames_num = m_walk_left_to - m_walk_left_from + 1;
		frame_time = m_walk_left_frame_time;
		break;
	case PersCurrentStateEnum::WalkRight:
		start_frame_num = m_walk_right_from;
		total_frames_num = m_walk_right_to - m_walk_right_from + 1;
		frame_time = m_walk_right_frame_time;
		break;
	case PersCurrentStateEnum::WalkToward:
		start_frame_num = m_walk_toward_from;
		total_frames_num = m_walk_toward_to - m_walk_toward_from + 1;
		frame_time = m_walk_toward_frame_time;
		break;
	case PersCurrentStateEnum::WalkOutward:
		start_frame_num = m_walk_outward_from;
		total_frames_num = m_walk_outward_to - m_walk_outward_from + 1;
		frame_time = m_walk_outward_frame_time;
		break;
	case PersCurrentStateEnum::JumpLeft:
		start_frame_num = m_jump_left_from;
		total_frames_num = m_jump_left_to - m_jump_left_from + 1;
		frame_time = m_jump_left_frame_time;
		break;
	case PersCurrentStateEnum::JumpRight:
		start_frame_num = m_jump_right_from;
		total_frames_num = m_jump_right_to - m_jump_right_from + 1;
		frame_time = m_jump_right_frame_time;
		break;
	case PersCurrentStateEnum::JumpToward:
		start_frame_num = m_jump_toward_from;
		total_frames_num = m_jump_toward_to - m_jump_toward_from + 1;
		frame_time = m_jump_toward_frame_time;
		break;
	case PersCurrentStateEnum::JumpOutward:
		start_frame_num = m_jump_outward_from;
		total_frames_num = m_jump_outward_to - m_jump_outward_from + 1;
		frame_time = m_jump_outward_frame_time;
		break;
	case PersCurrentStateEnum::IdleLeft:
		start_frame_num = m_idle_left_from;
		total_frames_num = m_idle_left_to - m_idle_left_from + 1;
		frame_time = m_idle_left_frame_time;
		break;
	case PersCurrentStateEnum::IdleRight:
		start_frame_num = m_idle_right_from;
		total_frames_num = m_idle_right_to - m_idle_right_from + 1;
		frame_time = m_idle_right_frame_time;
		break;
	case PersCurrentStateEnum::IdleToward:
		start_frame_num = m_idle_toward_from;
		total_frames_num = m_idle_toward_to - m_idle_toward_from + 1;
		frame_time = m_idle_toward_frame_time;
		break;
	case PersCurrentStateEnum::IdleOutward:
		start_frame_num = m_idle_outward_from;
		total_frames_num = m_idle_outward_to - m_idle_outward_from + 1;
		frame_time = m_idle_outward_frame_time;
		break;
	default:
		break;
	}
	float total_anim_time = frame_time * ((float)total_frames_num);
	if (m_anim_time > total_anim_time && state != PersCurrentStateEnum::IdleToward) {
		std::string fsddf = "";
	}
	float loop_time = std::fmodf(m_anim_time, total_anim_time);
	int current_frame = (int)(loop_time / frame_time) % total_frames_num;
	float frame_shift = start_frame_num % frames_per_row;
	float row_shift = start_frame_num / frames_per_row;


	float frame_u = 1.0f / ((float)m_atlas_width);
	float frame_v = 1.0f / ((float)m_atlas_height);

	XMMATRIX scale_to_one_frame = XMMatrixScaling(frame_u, frame_v, 1.0f);
	//XMMATRIX translation = XMMatrixTranslation(frame_u * 0.0f, frame_v * 2.0f, 0.0f);
	XMMATRIX translation = XMMatrixTranslation(frame_u * (frame_shift + current_frame), frame_v * row_shift, 0.0f);
	//XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	XMMATRIX transform = XMMatrixMultiply(scale_to_one_frame, translation);
	XMStoreFloat4x4(&m_text_transform, transform);
	m_anim_time += deltaMs;
}

DirectX::XMFLOAT4X4 PersTextureAnimStateComponent::GetTexTransform() {
	return m_text_transform;
}

PersCurrentStateEnum PersTextureAnimStateComponent::GetState() {
	return m_current_state;
}

PersCurrentStateClassEnum PersTextureAnimStateComponent::GetStateClass() {
	PersCurrentStateClassEnum res = PersCurrentStateClassEnum::Idle;
	switch (m_current_state) 	{
	case PersCurrentStateEnum::WalkLeft:
		res = PersCurrentStateClassEnum::Walk;
		break;
	case PersCurrentStateEnum::WalkRight:
		res = PersCurrentStateClassEnum::Walk;
		break;
	case PersCurrentStateEnum::WalkToward:
		res = PersCurrentStateClassEnum::Walk;
		break;
	case PersCurrentStateEnum::WalkOutward:
		res = PersCurrentStateClassEnum::Walk;
		break;
	case PersCurrentStateEnum::JumpLeft:
		res = PersCurrentStateClassEnum::Jump;
		break;
	case PersCurrentStateEnum::JumpRight:
		res = PersCurrentStateClassEnum::Jump;
		break;
	case PersCurrentStateEnum::JumpToward:
		res = PersCurrentStateClassEnum::Jump;
		break;
	case PersCurrentStateEnum::JumpOutward:
		res = PersCurrentStateClassEnum::Jump;
		break;
	case PersCurrentStateEnum::IdleLeft:
		res = PersCurrentStateClassEnum::Idle;
		break;
	case PersCurrentStateEnum::IdleRight:
		res = PersCurrentStateClassEnum::Idle;
		break;
	case PersCurrentStateEnum::IdleToward:
		res = PersCurrentStateClassEnum::Idle;
		break;
	case PersCurrentStateEnum::IdleOutward:
		res = PersCurrentStateClassEnum::Idle;
		break;
	default:
		break;
	}

	return res;
}

PersCurrentOrientClassEnum PersTextureAnimStateComponent::GetOrientClass() {
	PersCurrentOrientClassEnum res = PersCurrentOrientClassEnum::Toward;
	switch (m_current_state) {
	case PersCurrentStateEnum::WalkLeft:
		res = PersCurrentOrientClassEnum::Left;
		break;
	case PersCurrentStateEnum::WalkRight:
		res = PersCurrentOrientClassEnum::Right;
		break;
	case PersCurrentStateEnum::WalkToward:
		res = PersCurrentOrientClassEnum::Toward;
		break;
	case PersCurrentStateEnum::WalkOutward:
		res = PersCurrentOrientClassEnum::Outward;
		break;
	case PersCurrentStateEnum::JumpLeft:
		res = PersCurrentOrientClassEnum::Left;
		break;
	case PersCurrentStateEnum::JumpRight:
		res = PersCurrentOrientClassEnum::Right;
		break;
	case PersCurrentStateEnum::JumpToward:
		res = PersCurrentOrientClassEnum::Toward;
		break;
	case PersCurrentStateEnum::JumpOutward:
		res = PersCurrentOrientClassEnum::Outward;
		break;
	case PersCurrentStateEnum::IdleLeft:
		res = PersCurrentOrientClassEnum::Left;
		break;
	case PersCurrentStateEnum::IdleRight:
		res = PersCurrentOrientClassEnum::Right;
		break;
	case PersCurrentStateEnum::IdleToward:
		res = PersCurrentOrientClassEnum::Toward;
		break;
	case PersCurrentStateEnum::IdleOutward:
		res = PersCurrentOrientClassEnum::Outward;
		break;
	default:
		break;
	}

	return res;
}

void PersTextureAnimStateComponent::SetState(PersCurrentStateEnum state) {
	m_current_state = state;
	//m_anim_time = 0.0f;
}

TiXmlElement* PersTextureAnimStateComponent::VGenerateXml() {
	return nullptr;
}
