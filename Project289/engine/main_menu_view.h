#pragma once

#include <memory>

#include "human_view.h"
#include "main_menu_ui.h"

class MainMenuView : public HumanView {
private:
	static const std::string g_Name;

protected:
	std::shared_ptr<MainMenuUI> m_MainMenuUI;

public:
	MainMenuView(IRenderer* renderer);
	virtual ~MainMenuView();

	virtual LRESULT CALLBACK VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) override;
	virtual void VRenderText();
	virtual void VOnUpdate(float deltaMs) override;
	virtual const std::string& VGetName() override;

protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) override;
};