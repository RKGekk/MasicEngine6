#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"

using namespace std::literals;

enum class PersCurrentStateEnum {
	WalkLeft,
	WalkRight,
	WalkToward,
	WalkOutward,
	JumpLeft,
	JumpRight,
	JumpToward,
	JumpOutward,
	IdleLeft,
	IdleRight,
	IdleToward,
	IdleOutward
};

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

	virtual TiXmlElement* VGenerateXml() override;

protected:
	DirectX::XMFLOAT4X4 m_text_transform;

	PersCurrentStateEnum m_current_state;

	int m_atlas_width;
	int m_atlas_height;
	std::string m_atlas_name;
	float m_total_time;

	int m_walk_left_from;
	int m_walk_left_to;

	int m_walk_right_from;
	int m_walk_right_to;

	int m_walk_toward_from;
	int m_walk_toward_to;

	int m_walk_outward_from;
	int m_walk_outward_to;

	int m_jump_left_from;
	int m_jump_left_to;

	int m_jump_right_from;
	int m_jump_right_to;

	int m_jump_toward_from;
	int m_jump_toward_to;

	int m_jump_outward_from;
	int m_jump_outward_to;

	int m_idle_left_from;
	int m_idle_left_to;

	int m_idle_right_from;
	int m_idle_right_to;

	int m_idle_toward_from;
	int m_idle_toward_to;

	int m_idle_outward_from;
	int m_idle_outward_to;
};