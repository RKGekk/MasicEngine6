#include "human_view.h"
#include "engine.h"
#include "../tools/memory_utility.h"
#include "screen_element_scene.h"

void HumanView::VRenderText() {}

bool HumanView::LoadGame(TiXmlElement* pLevelData) {
	return VLoadGameDelegate(pLevelData);
}

bool HumanView::VLoadGameDelegate(TiXmlElement* pLevelData) {
	VPushElement(m_scene);
	return true;
}

HRESULT HumanView::VOnRestore() {
	HRESULT hr = S_OK;
	for (ScreenElementList::iterator i = m_screen_elements.begin(); i != m_screen_elements.end(); ++i) {
		hr = (*i)->VOnRestore();
		if (FAILED(hr)) { return hr; }
	}

	return hr;
}

HRESULT HumanView::VOnLostDevice() {
	HRESULT hr;
	for (ScreenElementList::iterator i = m_screen_elements.begin(); i != m_screen_elements.end(); ++i) 	{
		hr = (*i)->VOnLostDevice();
		if (FAILED(hr)) { return hr; }
	}

	return S_OK;
}

void HumanView::VOnRender(double fTime, float fElapsedTime) {
	m_current_tick = g_pApp->GetTimer().TotalTime();
	if (m_current_tick == m_last_draw) { return; }

	if (!m_can_draw) { return; }

	const float one_frame_time = 0.016f;
	if (m_run_full_speed || ((m_current_tick - m_last_draw) > one_frame_time)) {
		if (g_pApp->GetRenderer()->VPreRender()) {

			m_screen_elements.sort(SortBy_SharedPtr_Content<IScreenElement>());
			for (ScreenElementList::iterator i = m_screen_elements.begin(); i != m_screen_elements.end(); ++i) {
				if ((*i)->VIsVisible()) {
					(*i)->VOnRender(fTime, fElapsedTime);
				}
			}
			VRenderText();

			m_last_draw = m_current_tick;
			g_pApp->GetRenderer()->VPostRender();
		}
	}
}

EngineViewType HumanView::VGetType() {
	return EngineViewType::GameView_Human;
}

EngineViewId HumanView::VGetId() const {
	return m_view_id;
}

void HumanView::VOnAttach(EngineViewId vid, ActorId aid) {
	m_view_id = vid;
	m_actor_id = aid;
}

LRESULT HumanView::VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) {
	for (ScreenElementList::reverse_iterator i = m_screen_elements.rbegin(); i != m_screen_elements.rend(); ++i) {
		if ((*i)->VIsVisible()) {
			if ((*i)->VOnMsgProc(m_hWnd, m_uMsg, m_wParam, m_lParam)) {
				return 1;
			}
		}
	}

	LRESULT result = 0;
	switch (m_uMsg) {
		case WM_KEYDOWN: {
			if (((unsigned int)m_wParam) == (VK_ESCAPE)) { 
				g_pApp->AbortGame();
			}
			if (m_keyboard_handlers.size()) {
				for (auto& handler : m_keyboard_handlers) {
					result |= handler->VOnKeyDown(static_cast<const BYTE>(m_wParam));
				}
			}
		}
		break;
		case WM_KEYUP: {
			if (m_keyboard_handlers.size()) {
				for (auto& handler : m_keyboard_handlers) {
					result |= handler->VOnKeyUp(static_cast<const BYTE>(m_wParam));
				}
			}
			result = true;
		}
		break;
		case WM_MOUSEMOVE: {
			if (m_pointer_handlers.size()) {
				for (auto& handler : m_pointer_handlers) {
					result |= handler->VOnPointerMove(LOWORD(m_lParam), HIWORD(m_lParam), 1);
				}
			}
		}
		break;
		case WM_LBUTTONDOWN: {
			if (m_pointer_handlers.size()) {
				SetCapture(m_hWnd);
				for (auto& handler : m_pointer_handlers) {
					result |= handler->VOnPointerButtonDown(LOWORD(m_lParam), HIWORD(m_lParam), 1, "PointerLeft");
				}
			}
		}
		break;
		case WM_LBUTTONUP: {
			if (m_pointer_handlers.size()) {
				ReleaseCapture();
				for (auto& handler : m_pointer_handlers) {
					result |= handler->VOnPointerButtonUp(LOWORD(m_lParam), HIWORD(m_lParam), 1, "PointerLeft");
				}
			}
		}
		break;
		case WM_RBUTTONDOWN: {
			if (m_pointer_handlers.size()) {
				SetCapture(m_hWnd);
				for (auto& handler : m_pointer_handlers) {
					result |= handler->VOnPointerButtonDown(LOWORD(m_lParam), HIWORD(m_lParam), 1, "PointerRight");
				}
			}
		}
		break;
		case WM_RBUTTONUP: {
			if (m_pointer_handlers.size()) {
				ReleaseCapture();
				for (auto& handler : m_pointer_handlers) {
					result = handler->VOnPointerButtonUp(LOWORD(m_lParam), HIWORD(m_lParam), 1, "PointerRight");
				}
			}
		}
		break;
		case WM_CHAR: {}
		break;
	}

	return 0;
}

void HumanView::VOnUpdate(float deltaMilliseconds) {
	m_process_manager->UpdateProcesses(deltaMilliseconds);
	for (ScreenElementList::iterator i = m_screen_elements.begin(); i != m_screen_elements.end(); ++i) {
		(*i)->VOnUpdate(deltaMilliseconds);
	}
}

void HumanView::VPushElement(std::shared_ptr<IScreenElement> pElement) {
	m_screen_elements.push_front(pElement);
}

void HumanView::VRemoveElement(std::shared_ptr<IScreenElement> pElement) {
	m_screen_elements.remove(pElement);
}

void HumanView::VActivateScene(bool isActive) {
	if (m_scene) {
		m_scene->ActivateScene(isActive);
	}
}

void HumanView::VCanDraw(bool isCanDraw) {
	m_can_draw = isCanDraw;
}

void HumanView::TogglePause(bool active) {}

HumanView::~HumanView() {
	RemoveAllDelegates();
	while (!m_screen_elements.empty()) { m_screen_elements.pop_front(); }
}

HumanView::HumanView(IRenderer* renderer) {
	m_process_manager = std::make_unique<ProcessManager>();

	m_PointerRadius = 1;
	m_view_id = 0xffffffff;

	RegisterAllDelegates();
	m_base_game_state = BaseEngineState::BGS_Initializing;

	if (renderer) {
		m_scene.reset(new ScreenElementScene(renderer));

		Frustum frustum;
		//frustum.Init(DirectX::XM_PI / 4.0f, 1.0f, 1.0f, 100.0f);
		//frustum.Init(DirectX::XM_PI / 4.0f, 1.0f, 0.1f, 100.0f);
		const EngineOptions& options = g_pApp->GetConfig();
		frustum.Init(
			DirectX::XMConvertToRadians(options.m_fov),
			((float)options.m_screenWidth) / ((float)options.m_screenHeight),
			options.m_screenNear,
			options.m_screenFar
		);
		//frustum.Init(DirectX::XM_PI / 4.0f, 1280.0f/720.0f, 0.1f, 100.0f);
		//frustum.Init(DirectX::XM_PI / 2.0f, 1.0f, 0.1f, 100.0f);
		m_camera.reset(new CameraNode(DirectX::XMMatrixIdentity(), frustum));

		m_scene->VAddChild(INVALID_ACTOR_ID, INVALID_COMPONENT_ID, m_camera);
		m_scene->SetCamera(m_camera);
	}
}

ProcessManager* HumanView::GetProcessManager() {
	return m_process_manager.get();
}

void HumanView::VSetCameraOffset(const DirectX::XMFLOAT4& camOffset) {
	if (m_camera) {
		m_camera->SetCameraOffset(camOffset);
	}
}

void HumanView::HandleGameState(BaseEngineState newState) {}

void HumanView::VSetControlledActor(ActorId actorId) {
	m_actor_id = actorId;
}

std::shared_ptr<CameraNode> HumanView::VGetCamera() {
	return m_camera;
}

void HumanView::GameStateDelegate(IEventDataPtr pEventData) {}

void HumanView::RegisterAllDelegates() {

}

void HumanView::RemoveAllDelegates() {

}
