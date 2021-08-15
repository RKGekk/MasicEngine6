#pragma once

#include <string>

#include <DirectXMath.h>

#include "engine/renderer_enum.h"

struct EngineOptions {

	Renderer m_Renderer;
	bool m_runFullSpeed;
	bool m_fullScreen;
	int m_screenWidth;
	int m_screenHeight;
	bool m_debug_ui;

	float m_screenNear;
	float m_screenFar;
	float m_fov;
	float m_aspectRatio;
	float m_game_cam_offset_x;
	float m_game_cam_offset_y;
	float m_game_cam_offset_z;
	float m_game_cam_rotate_x;

	float m_fog_start;
	float m_fog_range;
	DirectX::XMFLOAT4 m_fog_color;

	float m_soundEffectsVolume;
	float m_musicVolume;

	EngineOptions();
	EngineOptions(const std::string& xmlFilePath);
	~EngineOptions();

	void Init(const std::string& xmlFilePath);
};
