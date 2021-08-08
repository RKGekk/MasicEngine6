#include "main_menu_view.h"
#include "engine.h"

const std::string MainMenuView::g_Name = "MainMenu"s;

MainMenuView::MainMenuView(IRenderer* renderer) : HumanView(renderer) {
	m_MainMenuUI = std::make_shared<MainMenuUI>(m_process_manager.get());
	VPushElement(m_MainMenuUI);

	//renderer->VSetBackgroundColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

MainMenuView::~MainMenuView() {}

LRESULT MainMenuView::VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) {
	return 0;
}

void MainMenuView::VRenderText() {}

void MainMenuView::VOnUpdate(float deltaMs) {
	HumanView::VOnUpdate(deltaMs);
}

void MainMenuView::VOnRender(double fTime, float fElapsedTime) {
	if (m_can_draw) {
		//g_pApp->GetRenderer()->VSetBackgroundColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	HumanView::VOnRender(fTime, fElapsedTime);
}

const std::string& MainMenuView::VGetName() {
	return g_Name;
}

bool MainMenuView::VLoadGameDelegate(TiXmlElement* pLevelData) {
	if (!HumanView::VLoadGameDelegate(pLevelData)) {
		return false;
	}

	m_scene->VOnRestore();
	return true;
}
