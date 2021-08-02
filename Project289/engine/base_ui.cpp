#include "base_ui.h"

BaseUI::BaseUI() {
	m_bIsVisible = true;
	m_PosX = m_PosY = 0;
	m_Width = 100;
	m_Height = 100;
}

void BaseUI::VOnUpdate(float deltaMilliseconds) {}

HRESULT BaseUI::VOnLostDevice() {
	return S_OK;
}

bool BaseUI::VIsVisible() const {
	return m_bIsVisible;
}

void BaseUI::VSetVisible(bool visible) {
	m_bIsVisible = visible;
}
