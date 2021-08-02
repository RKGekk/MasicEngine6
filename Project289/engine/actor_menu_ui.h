#pragma once

#include <string>

#include <DirectXMath.h>

#include "base_ui.h"
#include "../processes/process_manager.h"
#include "../actors/actor.h"

class ActorMenuUI : public BaseUI {
protected:
	void Set(ProcessManager* pm);

public:
	ActorMenuUI(ProcessManager* pm);
	virtual ~ActorMenuUI();

	virtual HRESULT VOnRestore() override;
	virtual HRESULT VOnRender(double fTime, float fElapsedTime) override;
	virtual void VOnUpdate(float deltaMilliseconds) override;

	virtual int VGetZOrder() const override;
	virtual void VSetZOrder(int const zOrder) override {}

	virtual LRESULT CALLBACK VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) override;

private:
	bool m_show_menu = false;
	ProcessManager* m_pm;

	int m_actor_id;

	bool m_transform_exists;
	DirectX::XMFLOAT4X4 m_transform;
	std::string m_actor_name;
};