#include "engine_options.h"

#include <DirectXMath.h>
#include "tools/tinyxml/tinyxml.h"
#include <memory>

EngineOptions::EngineOptions() {
	m_Renderer = Renderer::Renderer_D3D11;
	m_screenWidth = 800;
	m_screenHeight = 600;
	m_runFullSpeed = false;
	m_fullScreen = false;
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
}

EngineOptions::EngineOptions(const std::string& xmlFilePath) : EngineOptions() {
	Init(xmlFilePath);
}

EngineOptions::~EngineOptions() {}

void EngineOptions::Init(const std::string& xmlFileName) {
	std::shared_ptr<TiXmlDocument> m_pDoc(new TiXmlDocument(xmlFileName.data()));

	if (m_pDoc && m_pDoc->LoadFile()) {
		TiXmlElement* pRoot = m_pDoc->RootElement();
		if (!pRoot) {
			return;
		}

		TiXmlElement* pNode = NULL;
		pNode = pRoot->FirstChildElement("Graphics");
		if (pNode) {
			std::string attribute;

			attribute = pNode->Attribute("renderer");
			if (attribute == "Direct3D 11") {
				m_Renderer = Renderer::Renderer_D3D11;
			}

			if (pNode->Attribute("width")) {
				m_screenWidth = atoi(pNode->Attribute("width"));
			}

			if (pNode->Attribute("height")) {
				m_screenHeight = atoi(pNode->Attribute("height"));
			}

			if (m_screenHeight != 0) {
				m_aspectRatio = static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight);
			}

			if (pNode->Attribute("runfullspeed")) {
				attribute = pNode->Attribute("runfullspeed");
				m_runFullSpeed = (attribute == "yes") ? true : false;
			}

			if (pNode->Attribute("fullscreen")) {
				attribute = pNode->Attribute("fullscreen");
				m_fullScreen = (attribute == "yes") ? true : false;
			}

			if (pNode->Attribute("screenFar")) {
				m_screenFar = atof(pNode->Attribute("screenFar"));
			}

			if (pNode->Attribute("screenNear")) {
				m_screenNear = atof(pNode->Attribute("screenNear"));
			}

			if (pNode->Attribute("fov")) {
				m_fov = atof(pNode->Attribute("fov"));
			}

			if (pNode->Attribute("gamecamoffsetx")) {
				m_game_cam_offset_x = atof(pNode->Attribute("gamecamoffsetx"));
			}

			if (pNode->Attribute("gamecamoffsety")) {
				m_game_cam_offset_y = atof(pNode->Attribute("gamecamoffsety"));
			}

			if (pNode->Attribute("gamecamoffsetz")) {
				m_game_cam_offset_z = atof(pNode->Attribute("gamecamoffsetz"));
			}

			if (pNode->Attribute("gamecamrotatex")) {
				m_game_cam_rotate_x = atof(pNode->Attribute("gamecamrotatex"));
			}
		}

		pNode = pRoot->FirstChildElement("Sound");
		if (pNode) {
			m_musicVolume = atoi(pNode->Attribute("musicVolume")) / 100.0f;
			m_soundEffectsVolume = atoi(pNode->Attribute("sfxVolume")) / 100.0f;
		}
	}
}