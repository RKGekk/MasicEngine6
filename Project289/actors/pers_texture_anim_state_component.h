#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"
#include "pers_current_state_enum.h"

using namespace std::literals;

class PersTextureAnimStateComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	PersTextureAnimStateComponent();
	~PersTextureAnimStateComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(float deltaMs) override;

	virtual DirectX::XMFLOAT4X4 GetTexTransform();
	virtual PersCurrentStateEnum GetState();
	virtual PersCurrentStateClassEnum GetStateClass();
	virtual PersCurrentOrientClassEnum GetOrientClass();
	virtual void SetState(PersCurrentStateEnum state);

	virtual TiXmlElement* VGenerateXml() override;

protected:
	DirectX::XMFLOAT4X4 m_text_transform;
	float m_anim_time;
	PersCurrentStateEnum m_current_state;

	int m_atlas_width;
	int m_atlas_height;
	std::string m_atlas_name;
	float m_total_time;

	int m_walk_left_from;
	int m_walk_left_to;
	float m_walk_left_frame_time;

	int m_walk_right_from;
	int m_walk_right_to;
	float m_walk_right_frame_time;

	int m_walk_toward_from;
	int m_walk_toward_to;
	float m_walk_toward_frame_time;

	int m_walk_outward_from;
	int m_walk_outward_to;
	float m_walk_outward_frame_time;

	int m_jump_left_from;
	int m_jump_left_to;
	float m_jump_left_frame_time;

	int m_jump_right_from;
	int m_jump_right_to;
	float m_jump_right_frame_time;

	int m_jump_toward_from;
	int m_jump_toward_to;
	float m_jump_toward_frame_time;

	int m_jump_outward_from;
	int m_jump_outward_to;
	float m_jump_outward_frame_time;

	int m_idle_left_from;
	int m_idle_left_to;
	float m_idle_left_frame_time;

	int m_idle_right_from;
	int m_idle_right_to;
	float m_idle_right_frame_time;

	int m_idle_toward_from;
	int m_idle_toward_to;
	float m_idle_toward_frame_time;

	int m_idle_outward_from;
	int m_idle_outward_to;
	float m_idle_outward_frame_time;
};