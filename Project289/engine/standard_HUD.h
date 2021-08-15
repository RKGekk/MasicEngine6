#pragma once

#include <string>

#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <memory>

#include "base_ui.h"
#include "../processes/process_manager.h"
#include "../actors/actor.h"
#include "../tools/game_timer.h"

class StandardHUD : public BaseUI {
public:
	StandardHUD(ProcessManager* pm);
	virtual ~StandardHUD();

	virtual HRESULT VOnRestore() override;
	virtual HRESULT VOnRender(double fTime, float fElapsedTime) override;
	virtual void VOnUpdate(float deltaMilliseconds) override;

	virtual int VGetZOrder() const override;
	virtual void VSetZOrder(int const zOrder) override {}

	virtual LRESULT CALLBACK VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) override;

private:
	bool m_show_HUD = true;
	ProcessManager* m_pm;

	std::unique_ptr<DirectX::SpriteBatch> m_sprite_batch;
	std::unique_ptr<DirectX::SpriteFont> m_sprite_font;

	GameTimer m_fps_timer;
};