#pragma once

#include <optional>

#include "i_screen_element.h"

class BaseUI : public IScreenElement {
protected:
	int m_PosX;
	int m_PosY;
	int m_Width;
	int m_Height;
	bool m_bIsVisible;

	std::optional<int> m_Result;

public:
	BaseUI();
	virtual HRESULT VOnLostDevice() override;
	virtual void VOnUpdate(float deltaMilliseconds) override;
	
	virtual bool VIsVisible() const override;
	virtual void VSetVisible(bool visible) override;
};