#pragma once

#include <memory>
#include <queue>

#include <DirectXMath.h>

#include "../graphics/imgui/imgui.h"
#include "../graphics/imgui/imgui_impl_win32.h"
#include "../graphics/imgui/imgui_impl_dx11.h"

#include "i_engine_view.h"
#include "../processes/process_manager.h"
#include "base_engine_state.h"
#include "i_screen_element.h"
#include "i_renderer.h"
#include "i_pointer_handler.h"
#include "i_keyboard_handler.h"
#include "../nodes/camera_node.h"
#include "screen_element_scene.h"

class HumanView : public IEngineView {
	friend class GameCodeApp;

protected:
	EngineViewId m_view_id;
	ActorId m_actor_id;
	BaseEngineState m_base_game_state;

	float m_current_tick;
	float m_last_draw;
	bool m_run_full_speed;
	bool m_can_draw = true;

	std::unique_ptr<ProcessManager> m_process_manager;
	ScreenElementList m_screen_elements;
	std::shared_ptr<ScreenElementScene> m_scene;
	std::shared_ptr<CameraNode> m_camera;

	int m_PointerRadius;
	std::shared_ptr<IPointerHandler> m_pointer_handler;
	std::shared_ptr<IKeyboardHandler> m_keyboard_handler;

	virtual void VRenderText();

public:
	HumanView(IRenderer* renderer);
	virtual ~HumanView();

	bool LoadGame(TiXmlElement* pLevelData);

	virtual HRESULT VOnRestore() override;
	virtual HRESULT VOnLostDevice() override;
	virtual void VOnRender(double fTime, float fElapsedTime) override;
	virtual EngineViewType VGetType() override;
	virtual EngineViewId VGetId() const override;

	virtual void VOnAttach(EngineViewId vid, ActorId aid) override;
	virtual LRESULT CALLBACK VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) override;
	virtual void VOnUpdate(float deltaMilliseconds) override;

	virtual void VPushElement(std::shared_ptr<IScreenElement> pElement);
	virtual void VRemoveElement(std::shared_ptr<IScreenElement> pElement);

	virtual void VActivateScene(bool isActive);
	virtual void VCanDraw(bool isCanDraw);

	void TogglePause(bool active);
	void HandleGameState(BaseEngineState newState);

	ProcessManager* GetProcessManager();

	virtual void VSetCameraOffset(const DirectX::XMFLOAT4& camOffset);
	virtual void VSetControlledActor(ActorId actorId);

	void GameStateDelegate(IEventDataPtr pEventData);

protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData);

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();
};