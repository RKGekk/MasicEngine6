#pragma once

#include <string>

#include "engine/renderer_enum.h"

struct EngineOptions {

	Renderer	m_Renderer;
	bool		m_runFullSpeed;
	bool		m_fullScreen;
	int			m_screenWidth;
	int			m_screenHeight;
	float		m_screenNear;
	float		m_screenFar;
	float		m_fov;
	float		m_aspectRatio;

	float m_soundEffectsVolume;
	float m_musicVolume;

	EngineOptions();
	EngineOptions(const std::string& xmlFilePath);
	~EngineOptions();

	void Init(const std::string& xmlFilePath);
};
