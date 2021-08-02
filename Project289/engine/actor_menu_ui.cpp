#include "actor_menu_UI.h"

#include "../graphics/imgui/imgui.h"
#include "../graphics/imgui/imgui_impl_win32.h"
#include "../graphics/imgui/imgui_impl_dx11.h"

#include "../tools/memory_utility.h"
#include "../actors/transform_component.h"
#include "../engine/engine.h"

void ActorMenuUI::Set(ProcessManager* pm) {
	if (m_actor_id == 0) {
		m_transform_exists = false;
		DirectX::XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity());
		return;
	}
	std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_actor_id));
	if (act) {
		m_actor_name = act->GetName();

		unsigned int componentId = ActorComponent::GetIdFromName("TransformComponent");
		std::shared_ptr<TransformComponent> rc = MakeStrongPtr(act->GetComponent<TransformComponent>(componentId));
		if (rc) {
			m_transform_exists = true;
			m_transform = rc->GetTransform4x4f();
		}
	}
}

ActorMenuUI::ActorMenuUI(ProcessManager* pm) : m_pm(pm) {
	m_show_menu = true;
	m_actor_id = 0;

	Set(pm);
}

ActorMenuUI::~ActorMenuUI() {}

HRESULT ActorMenuUI::VOnRestore() {
	return S_OK;
}

HRESULT ActorMenuUI::VOnRender(double fTime, float fElapsedTime) {
	if (!m_show_menu) { return S_OK; }

	ImGui::Begin("Actor Menu");

	ImGui::Text(m_actor_name.c_str());

	if (ImGui::InputInt("Actor ID", &m_actor_id)) {
		std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_actor_id));
		if (act) {
			m_actor_name = act->GetName();

			unsigned int componentId = ActorComponent::GetIdFromName("TransformComponent");
			std::shared_ptr<TransformComponent> rc = MakeStrongPtr(act->GetComponent<TransformComponent>(componentId));
			if (rc) {
				m_transform_exists = true;
				m_transform = rc->GetTransform4x4f();
			}
			else {
				m_transform_exists = false;
				DirectX::XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity());
			}
		}
		else {
			m_transform_exists = false;
			DirectX::XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity());
		}
	}
	else {
		if (m_transform_exists && ImGui::SliderFloat4("Transform row 1", ((float*)&m_transform) + 0, -8.0f, 8.0f)) {}
		if (m_transform_exists && ImGui::SliderFloat4("Transform row 2", ((float*)&m_transform) + 4, -8.0f, 8.0f)) {}
		if (m_transform_exists && ImGui::SliderFloat4("Transform row 3", ((float*)&m_transform) + 8, -8.0f, 8.0f)) {}
		if (m_transform_exists && ImGui::SliderFloat4("Transform row 4", ((float*)&m_transform) + 12, -8.0f, 8.0f)) {
			std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_actor_id));
			if (act) {
				unsigned int componentId = ActorComponent::GetIdFromName("TransformComponent");
				std::shared_ptr<TransformComponent> rc = MakeStrongPtr(act->GetComponent<TransformComponent>(componentId));
				if (rc) {
					m_transform_exists = true;
					rc->SetTransform(m_transform);
				}
			}
		}
	}

	ImGui::End();

	return S_OK;
}

void ActorMenuUI::VOnUpdate(float deltaMilliseconds) {}

int ActorMenuUI::VGetZOrder() const {
	return 1;
}

LRESULT ActorMenuUI::VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) {
	return 0;
}




