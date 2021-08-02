#pragma once

#include <memory>

#include "render_window.h"
#include "../keyboard/keyboard_class.h"
#include "../mouse/mouse_class.h"
#include "../tools/game_timer.h"
#include "base_engine_logic.h"
#include "i_screen_element.h"
#include "renderer_enum.h"
#include "i_renderer.h"
#include "../events/event_manager.h"
#include "human_view.h"

class Engine {
public:
	Engine();
	virtual ~Engine();

	bool Initialize(const RenderWindowConfig& cfg);
	void Run();
	void AbortGame();
	bool IsRunning();

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnDisplayChange(int colorDepth, int width, int height);
	LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnClose();
	LRESULT OnAltEnter();
	LRESULT OnNcCreate(LPCREATESTRUCT cs);

	const EngineOptions& GetConfig();
	const RenderWindow& GetRenderWindow();
	static Renderer GetRendererImpl();

	BaseEngineLogic* GetGameLogic();
	IRenderer* GetRenderer();
	std::shared_ptr<HumanView> GetHumanView();
	std::shared_ptr<HumanView> GetHumanViewByName(std::string name);
	const GameTimer& GetTimer();

	bool ProcessMessages();
	bool Update();
	void RenderFrame();
	virtual bool VLoadGame();
	int Modal(std::shared_ptr<IScreenElement> pModalScreen, int defaultAnswer);

protected:
	virtual std::unique_ptr<BaseEngineLogic> VCreateGameAndView();
	virtual void VRegisterEvents();

private:
	bool m_is_running;
	bool m_is_editor_running;
	bool m_is_quit_requested;
	bool m_is_quitting;
	int m_has_modal_dialog;

	EngineOptions m_options;
	RenderWindow m_render_window;
	GameTimer m_timer;

	std::unique_ptr<EventManager> m_event_manager;
	std::unique_ptr<IRenderer> m_renderer;

	std::unique_ptr<BaseEngineLogic> m_game;
	
};

extern Engine* g_pApp;