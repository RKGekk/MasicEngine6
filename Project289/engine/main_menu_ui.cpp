#include "main_menu_ui.h"
#include "../processes/delay_process.h"
#include "../processes/exec_process.h"
#include "../engine/engine.h"
#include "../tools/memory_utility.h"
#include "../actors/transform_component.h"
#include "../actors/mesh_render_component.h"
#include "../events/evt_data_move_actor.h"
#include "../events/evt_data_request_destroy_actor.h"

void MainMenuUI::Set(ProcessManager* pm) {
	std::shared_ptr<ExecProcess> execOne = std::make_shared<ExecProcess>([]() {
		StrongActorPtr pActorLoading = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("loading"));
		std::shared_ptr<MeshRenderComponent> pMeshComponentLoading = MakeStrongPtr(pActorLoading->GetComponent<MeshRenderComponent>(MeshRenderComponent::g_Name));
		pMeshComponentLoading->SetColorA(0.0f);
		return true;
	});
	pm->AttachProcess(execOne);

	std::shared_ptr<DelayProcess> delay = std::make_shared<DelayProcess>(1.0f, [](float dt, float tt, float n) {
		StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("logo"));
		if (pActor) {
			std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
			DirectX::XMFLOAT4X4 mx = pTransformComponent->GetTransform4x4f();
			static bool first = true;
			static float start = 0.0f;
			if (first) { start = mx._43; first = false; };
			float end = 3.0f;
			mx._43 = start + (end - start) * n;
			pTransformComponent->SetTransform(mx);

			std::shared_ptr<MeshRenderComponent> pMeshComponent = MakeStrongPtr(pActor->GetComponent<MeshRenderComponent>(MeshRenderComponent::g_Name));
			pMeshComponent->SetColorA(n);

			std::shared_ptr<EvtData_Move_Actor> pEvent(new EvtData_Move_Actor(pActor->GetId(), mx));
			IEventManager::Get()->VTriggerEvent(pEvent);
		}
		return true;
	});
	std::shared_ptr<ExecProcess> exec = std::make_shared<ExecProcess>([this]() {
		m_show_main_menu = true;
		return true;
	});
	delay->AttachChild(exec);
	pm->AttachProcess(delay);
}

MainMenuUI::MainMenuUI(ProcessManager* pm) : m_pm(pm) {
	g_pApp->GetRenderer()->VSetBackgroundColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	Set(pm);
}

MainMenuUI::~MainMenuUI() {}

HRESULT MainMenuUI::VOnRestore() {
	return S_OK;
}

HRESULT MainMenuUI::VOnRender(double fTime, float fElapsedTime) {
	if (!m_show_main_menu) { return S_OK; }

	ImGui::Begin("Main Menu");

	if (ImGui::Button("Play")) {
		std::shared_ptr<ExecProcess> exec = std::make_shared<ExecProcess>([this]() {
			m_show_main_menu = false;
			StrongActorPtr pActorLogo = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("logo"));
			std::shared_ptr<EvtData_Request_Destroy_Actor> pNewGameEvent(new EvtData_Request_Destroy_Actor(pActorLogo->GetId()));
			IEventManager::Get()->VTriggerEvent(pNewGameEvent);

			return true;
		});

		std::shared_ptr<DelayProcess> delay = std::make_shared<DelayProcess>(1.0f, [](float dt, float tt, float n) {
			StrongActorPtr pActorLoading = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("loading"));
			if (pActorLoading) {
				std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActorLoading->GetComponent<TransformComponent>(TransformComponent::g_Name));
				DirectX::XMFLOAT4X4 mx = pTransformComponent->GetTransform4x4f();
				static bool first = true;
				static float start = 0.0f;
				if (first) { start = mx._43; first = false; };
				float end = 3.0f;
				mx._43 = start + (end - start) * n;
				pTransformComponent->SetTransform(mx);

				std::shared_ptr<MeshRenderComponent> pMeshComponent = MakeStrongPtr(pActorLoading->GetComponent<MeshRenderComponent>(MeshRenderComponent::g_Name));
				pMeshComponent->SetColorA(n);

				std::shared_ptr<EvtData_Move_Actor> pEvent(new EvtData_Move_Actor(pActorLoading->GetId(), mx));
				IEventManager::Get()->VTriggerEvent(pEvent);
			}
			return true;
		});
		
		exec->AttachChild(delay);
		g_pApp->GetGameLogic()->AttachProcess(exec);

		std::shared_ptr<ExecProcess> exec2 = std::make_shared<ExecProcess>([this]() {
			g_pApp->GetGameLogic()->VChangeState(BaseEngineState::BGS_LoadingGameEnvironment);
			return true;
		});

		g_pApp->GetGameLogic()->AttachProcess(exec2);
	};

	ImGui::End();

	return S_OK;
}

int MainMenuUI::VGetZOrder() const {
	return 1;
}

LRESULT MainMenuUI::VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) {
	return 0;
}

void MainMenuUI::VOnUpdate(float deltaMilliseconds) {}
