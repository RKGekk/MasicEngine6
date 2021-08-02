#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "../graphics/imgui/imgui.h"
#include "../graphics/imgui/imgui_impl_win32.h"
#include "../graphics/imgui/imgui_impl_dx11.h"

#include "base_ui.h"
#include "../processes/process_manager.h"

class MainMenuUI : public BaseUI {
protected:
	void Set(ProcessManager* pm);

public:
	MainMenuUI(ProcessManager* pm);
	virtual ~MainMenuUI();

	virtual HRESULT VOnRestore() override;
	virtual HRESULT VOnRender(double fTime, float fElapsedTime) override;
	virtual void VOnUpdate(float deltaMilliseconds) override;

	virtual int VGetZOrder() const override;
	virtual void VSetZOrder(int const zOrder) override {}

	virtual LRESULT CALLBACK VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) override;

private:
	bool m_show_main_menu = false;
	ProcessManager* m_pm;
};