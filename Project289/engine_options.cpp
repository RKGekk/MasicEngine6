#include "engine_options.h"

#include <DirectXMath.h>
#include "tools/tinyxml/tinyxml.h"
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cctype>

EngineOptions::EngineOptions() {
	m_Renderer = Renderer::Renderer_D3D11;
	m_screenWidth = 800;
	m_screenHeight = 600;
	m_runFullSpeed = false;
	m_fullScreen = false;
	m_debug_ui = true;

	m_soundEffectsVolume = 1.0f;
	m_musicVolume = 1.0f;

	m_screenNear = 0.1f;
	m_screenFar = 1000.0f;
	m_fov = DirectX::XMConvertToDegrees(DirectX::XM_PIDIV2);
	m_aspectRatio = 1.0f;
	m_game_cam_offset_x = 0.0f;
	m_game_cam_offset_y = 1.0f;
	m_game_cam_offset_z = -2.0f;
	m_game_cam_rotate_x = DirectX::XMConvertToDegrees(DirectX::XM_PIDIV4);

	m_fog_start = 10.0f;
	m_fog_range = 100.0f;
	m_fog_color = { 0.0f, 0.0f, 0.0f, 1.0f };
}

EngineOptions::EngineOptions(const std::string& xmlFilePath) : EngineOptions() {
	Init(xmlFilePath);
}

EngineOptions::~EngineOptions() {}

void EngineOptions::Init(const std::string& xmlFileName) {
	std::shared_ptr<TiXmlDocument> m_pDoc(new TiXmlDocument(xmlFileName.data()));

	if (m_pDoc && m_pDoc->LoadFile()) {
		TiXmlElement* pRoot = m_pDoc->RootElement();
		if (!pRoot) { return; }

		TiXmlElement* pGraphics = pRoot->FirstChildElement("Graphics");
		if (pGraphics) {

			TiXmlElement* pRenderer = pGraphics->FirstChildElement("Renderer");
			if (pRenderer) {
				std::string sRenderer = pRenderer->FirstChild()->Value();
				if (sRenderer == "Direct3D 11") {
					m_Renderer = Renderer::Renderer_D3D11;
				}
			}

			TiXmlElement* pWidth = pGraphics->FirstChildElement("Width");
			if (pWidth) {
				std::string sWidth = pWidth->FirstChild()->Value();
				m_screenWidth = std::stoi(sWidth);
			}

			TiXmlElement* pHeight = pGraphics->FirstChildElement("Height");
			if (pHeight) {
				std::string sHeight = pHeight->FirstChild()->Value();
				m_screenHeight = std::stoi(sHeight);
			}

			if (m_screenHeight != 0) {
				m_aspectRatio = static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight);
			}

			TiXmlElement* pRunFullSpeed = pGraphics->FirstChildElement("RunFullSpeed");
			if (pRunFullSpeed) {
				std::string sRunFullSpeed = pRunFullSpeed->FirstChild()->Value();
				std::for_each(sRunFullSpeed.begin(), sRunFullSpeed.end(), [](char& c) { c = ::toupper(c); });
				m_runFullSpeed = (sRunFullSpeed == "YES" || sRunFullSpeed == "TRUE" || sRunFullSpeed == "1") ? true : false;
			}

			TiXmlElement* pFullScreen = pGraphics->FirstChildElement("FullScreen");
			if (pFullScreen) {
				std::string sFullScreen = pFullScreen->FirstChild()->Value();
				std::for_each(sFullScreen.begin(), sFullScreen.end(), [](char& c) { c = ::toupper(c); });
				m_fullScreen = (sFullScreen == "YES" || sFullScreen == "TRUE" || sFullScreen == "1") ? true : false;
			}

			TiXmlElement* pDebugUI = pGraphics->FirstChildElement("DebugUI");
			if (pFullScreen) {
				std::string sDebugUI = pDebugUI->FirstChild()->Value();
				std::for_each(sDebugUI.begin(), sDebugUI.end(), [](char& c) { c = ::toupper(c); });
				m_debug_ui = (sDebugUI == "YES" || sDebugUI == "TRUE" || sDebugUI == "1") ? true : false;
			}
		}

		TiXmlElement* pCamera = pRoot->FirstChildElement("Camera");
		if (pCamera) {
			TiXmlElement* pScreenFar = pCamera->FirstChildElement("ScreenFar");
			if (pScreenFar) {
				std::string sScreenFar = pScreenFar->FirstChild()->Value();
				m_screenFar = std::stof(sScreenFar);
			}

			TiXmlElement* pScreenNear = pCamera->FirstChildElement("ScreenNear");
			if (pScreenNear) {
				std::string sScreenNear = pScreenNear->FirstChild()->Value();
				m_screenNear = std::stof(sScreenNear);
			}

			TiXmlElement* pFov = pCamera->FirstChildElement("Fov");
			if (pFov) {
				std::string sFov = pFov->FirstChild()->Value();
				m_fov = std::stof(sFov);
			}

			TiXmlElement* pGameCameraOffsetX = pCamera->FirstChildElement("GameCameraOffsetX");
			if (pGameCameraOffsetX) {
				std::string sGameCameraOffsetX = pGameCameraOffsetX->FirstChild()->Value();
				m_game_cam_offset_x = std::stof(sGameCameraOffsetX);
			}

			TiXmlElement* pGameCameraOffsetY = pCamera->FirstChildElement("GameCameraOffsetY");
			if (pGameCameraOffsetY) {
				std::string sGameCameraOffsetY = pGameCameraOffsetY->FirstChild()->Value();
				m_game_cam_offset_y = std::stof(sGameCameraOffsetY);
			}

			TiXmlElement* pGameCameraOffsetZ = pCamera->FirstChildElement("GameCameraOffsetZ");
			if (pGameCameraOffsetZ) {
				std::string sGameCameraOffsetZ = pGameCameraOffsetZ->FirstChild()->Value();
				m_game_cam_offset_z = std::stof(sGameCameraOffsetZ);
			}

			TiXmlElement* pGameCameraRotateX = pCamera->FirstChildElement("GameCameraRotateX");
			if (pGameCameraRotateX) {
				std::string sGameCameraRotateX = pGameCameraRotateX->FirstChild()->Value();
				m_game_cam_rotate_x = std::stof(sGameCameraRotateX);
			}
		}

		TiXmlElement* pFog = pRoot->FirstChildElement("Fog");
		if (pFog) {
			TiXmlElement* pFogStart = pFog->FirstChildElement("FogStart");
			if (pFogStart) {
				std::string sFogStart = pFogStart->FirstChild()->Value();
				m_fog_start = std::stof(sFogStart);
			}

			TiXmlElement* pFogRange = pFog->FirstChildElement("FogRange");
			if (pFogRange) {
				std::string sFogRange = pFogRange->FirstChild()->Value();
				m_fog_range = std::stof(sFogRange);
			}

			TiXmlElement* pFogColor = pFog->FirstChildElement("FogColor");
			if (pFogColor) {
				DirectX::XMFLOAT4 color;

				double r = 1.0;
				double g = 1.0;
				double b = 1.0;
				double a = 1.0;

				pFogColor->Attribute("r", &r);
				pFogColor->Attribute("g", &g);
				pFogColor->Attribute("b", &b);
				pFogColor->Attribute("a", &a);

				color.x = (float)r;
				color.y = (float)g;
				color.z = (float)b;
				color.w = (float)a;

				m_fog_color = color;
			}
		}

		TiXmlElement* pSound = pRoot->FirstChildElement("Sound");
		if (pSound) {
			m_musicVolume = atoi(pSound->Attribute("musicVolume")) / 100.0f;
			m_soundEffectsVolume = atoi(pSound->Attribute("sfxVolume")) / 100.0f;
		}
	}
}