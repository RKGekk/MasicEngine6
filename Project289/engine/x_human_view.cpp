#include "x_human_view.h"
#include "../events/evt_data_update_tick.h"
#include "../engine/engine.h"
#include "../actors/mesh_render_component.h"
#include "../actors/mesh_render_light_component.h"

const std::string XHumanView::g_Name = "Level"s;

XHumanView::XHumanView(IRenderer* renderer) : HumanView(renderer) {
	m_bShowUI = false;
	m_bShowDebugUI = g_pApp->GetConfig().m_debug_ui;
	if (m_bShowDebugUI) {
		m_ActorMenuUI = std::make_shared<ActorMenuUI>(m_process_manager.get());
		VPushElement(m_ActorMenuUI);
	}
	m_StandardHUD = std::make_shared<StandardHUD>(m_process_manager.get());
	VPushElement(m_StandardHUD);
	RegisterAllDelegates();
}

XHumanView::~XHumanView() {
	RemoveAllDelegates();
}

LRESULT XHumanView::VOnMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (HumanView::VOnMsgProc(hwnd, uMsg, wParam, lParam)) { return 1; }
	return 0;
}

void XHumanView::VRenderText() {

}

void XHumanView::VOnUpdate(float deltaSeconds) {
	HumanView::VOnUpdate(deltaSeconds);

	if (m_pGeoPhysicsMovementController) {
		m_pGeoPhysicsMovementController->OnUpdate(deltaSeconds);
	}
	if (m_pFreeCameraController) {
		m_pFreeCameraController->OnUpdate(deltaSeconds);
	}
	

	std::shared_ptr<EvtData_Update_Tick> pTickEvent(new EvtData_Update_Tick(deltaSeconds, g_pApp->GetTimer().TotalTime()));
	IEventManager::Get()->VTriggerEvent(pTickEvent);
}

void XHumanView::VOnRender(double fTime, float fElapsedTime) {
	if (m_can_draw) {
		g_pApp->GetRenderer()->VSetBackgroundColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
	HumanView::VOnRender(fTime, fElapsedTime);
}

void XHumanView::VOnAttach(EngineViewId vid, ActorId aid) {
	HumanView::VOnAttach(vid, aid);
}

void XHumanView::VSetControlledActor(ActorId actorId) {
	m_pTeapot = std::static_pointer_cast<SceneNode>(m_scene->FindActor(actorId, MeshRenderComponent::GetIdFromName(MeshRenderComponent::g_Name)));
	if (!m_pTeapot) {
		m_pTeapot = std::static_pointer_cast<SceneNode>(m_scene->FindActor(actorId, MeshRenderLightComponent::GetIdFromName(MeshRenderLightComponent::g_Name)));
	}
	if (!m_pTeapot) {
		m_keyboard_handlers.clear();
		m_pointer_handlers.clear();
		m_pGeoPhysicsMovementController.reset();
		m_pFreeCameraController.reset(new MovementController(m_camera, 0, 0, true));
		m_keyboard_handlers.push_back(m_pFreeCameraController);
		m_pointer_handlers.push_back(m_pFreeCameraController);

		m_camera->SetTarget(nullptr);
		return;
	}
	else {
		m_keyboard_handlers.clear();
		m_pointer_handlers.clear();
		m_pFreeCameraController.reset();
		m_pGeoPhysicsMovementController.reset(new GeoPhysicsMovementController(m_pTeapot, true));
		m_keyboard_handlers.push_back(m_pGeoPhysicsMovementController);
		m_pointer_handlers.push_back(m_pGeoPhysicsMovementController);

		m_camera->SetTarget(m_pTeapot);
	}

	HumanView::VSetControlledActor(actorId);
}

bool XHumanView::VLoadGameDelegate(TiXmlElement* pLevelData) {
	if (!HumanView::VLoadGameDelegate(pLevelData)) { return false; }

	m_keyboard_handlers.clear();
	m_pointer_handlers.clear();
	m_pFreeCameraController.reset(new MovementController(m_camera, 0, 0, true));
	m_keyboard_handlers.push_back(m_pFreeCameraController);
	m_pointer_handlers.push_back(m_pFreeCameraController);

	m_scene->VOnRestore();
	return true;
}

void XHumanView::GameplayUiUpdateDelegate(IEventDataPtr pEventData) {}

void XHumanView::SetControlledActorDelegate(IEventDataPtr pEventData) {}

const std::string& XHumanView::VGetName() {
	return g_Name;
}

void XHumanView::RegisterAllDelegates() {}

void XHumanView::RemoveAllDelegates() {}
