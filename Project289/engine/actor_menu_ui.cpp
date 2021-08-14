#include "actor_menu_UI.h"

#include "../graphics/imgui/imgui.h"
#include "../graphics/imgui/imgui_impl_win32.h"
#include "../graphics/imgui/imgui_impl_dx11.h"

#include "../tools/memory_utility.h"
#include "../actors/transform_component.h"
#include "../actors/light_render_component.h"
#include "../actors/particle_component.h"
#include "../engine/engine.h"

void ActorMenuUI::Set(ProcessManager* pm) {
	if (m_actor_id == 0) {
		m_transform_exists = false;
		m_light_exists = false;
		m_particle_exists = false;
		DirectX::XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity());
		return;
	}
	std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_actor_id));
	if (act) {
		m_actor_name = act->GetName();

		std::shared_ptr<TransformComponent> rc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
		if (rc) {
			m_transform_exists = true;
			m_transform = rc->GetTransform4x4f();
		}

		std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(ActorComponent::GetIdFromName("LightRenderComponent")));
		if (lc) {
			m_light_exists = true;
			const LightProperties& lp = lc->GetLight();
			m_ambient = lp.m_Ambient;
			m_diffuse = lp.m_Diffuse;
			m_specular = lp.m_Specular;
			m_const = lp.m_Attenuation[0];
			m_linear = lp.m_Attenuation[1];
			m_exp = lp.m_Attenuation[2];
			m_Range = lp.m_Range;
			m_Spot = lp.m_Spot;
		}
		else {
			m_light_exists = false;
			m_ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
			m_diffuse = { 0.0f, 0.0f, 0.0f, 1.0f };
			m_specular = { 0.0f, 0.0f, 0.0f, 1.0f };
			m_const = 0.0f;
			m_linear = 0.0f;
			m_exp = 0.0f;
			m_Range = 0.0f;
			m_Spot = 0.0f;
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

	if (ImGui::CollapsingHeader("Camera")) {
		m_screenFar = g_pApp->GetConfig().m_screenFar;
		if (ImGui::SliderFloat("Far", ((float*)&m_screenFar), 1.0f, 100.0f)) {
			g_pApp->GetConfig().m_screenFar = m_screenFar;
			std::shared_ptr<HumanView> hv = g_pApp->GetHumanView();
			Frustum& frustum = hv->VGetCamera()->GetFrustum();
			frustum.m_Far = m_screenFar;
			hv->VOnRestore();
		}

		m_screenNear = g_pApp->GetConfig().m_screenNear;
		if (ImGui::SliderFloat("Near", ((float*)&m_screenNear), 0.1f, 1.0f)) {
			g_pApp->GetConfig().m_screenNear = m_screenNear;
			std::shared_ptr<HumanView> hv = g_pApp->GetHumanView();
			Frustum& frustum = hv->VGetCamera()->GetFrustum();
			frustum.m_Near = m_screenNear;
			hv->VOnRestore();
		}

		m_fov = g_pApp->GetConfig().m_fov;
		if (ImGui::SliderFloat("Fov", ((float*)&m_fov), 18.0f, 120.0f)) {
			g_pApp->GetConfig().m_fov = m_fov;
			std::shared_ptr<HumanView> hv = g_pApp->GetHumanView();
			Frustum& frustum = hv->VGetCamera()->GetFrustum();
			frustum.m_Fov = DirectX::XMConvertToRadians(m_fov);
			hv->VOnRestore();
		}

		m_game_cam_offset_x = g_pApp->GetConfig().m_game_cam_offset_x;
		if (ImGui::SliderFloat("Offset X", ((float*)&m_game_cam_offset_x), -3.0f, 3.0f)) {
			g_pApp->GetConfig().m_game_cam_offset_x = m_game_cam_offset_x;
		}

		m_game_cam_offset_y = g_pApp->GetConfig().m_game_cam_offset_y;
		if (ImGui::SliderFloat("Offset Y", ((float*)&m_game_cam_offset_y), -3.0f, 3.0f)) {
			g_pApp->GetConfig().m_game_cam_offset_y = m_game_cam_offset_y;
		}

		m_game_cam_offset_z = g_pApp->GetConfig().m_game_cam_offset_z;
		if (ImGui::SliderFloat("Offset Z", ((float*)&m_game_cam_offset_z), -3.0f, 3.0f)) {
			g_pApp->GetConfig().m_game_cam_offset_z = m_game_cam_offset_z;
		}

		m_game_cam_rotate_x = g_pApp->GetConfig().m_game_cam_rotate_x;
		if (ImGui::SliderFloat("Rotate X", ((float*)&m_game_cam_rotate_x), -90.0f, 90.0f)) {
			g_pApp->GetConfig().m_game_cam_rotate_x = m_game_cam_rotate_x;
		}
	}

	if (ImGui::CollapsingHeader("Fog")) {
		m_fog_start = g_pApp->GetConfig().m_fog_start;
		if (ImGui::SliderFloat("Fog Start At", ((float*)&m_fog_start), 0.1f, 10.0f)) {
			g_pApp->GetConfig().m_fog_start = m_fog_start;
		}

		m_fog_range = g_pApp->GetConfig().m_fog_range;
		if (ImGui::SliderFloat("Fog Range To", ((float*)&m_fog_range), 0.5f, 30.0f)) {
			g_pApp->GetConfig().m_fog_range = m_fog_range;
		}
		
		m_fog_color = g_pApp->GetConfig().m_fog_color;
		if (ImGui::ColorEdit4("Fog Color", ((float*)&m_fog_color))) {
			g_pApp->GetConfig().m_fog_color = m_fog_color;
		}
	}

	if (ImGui::CollapsingHeader("Actors")) {
		ImGui::Text(m_actor_name.c_str());

		if (ImGui::InputInt("Actor ID", &m_actor_id)) {
			std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_actor_id));
			if (act) {
				m_actor_name = act->GetName();

				unsigned int componentId = ActorComponent::GetIdFromName("TransformComponent");
				std::shared_ptr<TransformComponent> tc = MakeStrongPtr(act->GetComponent<TransformComponent>(componentId));
				if (tc) {
					m_transform_exists = true;
					m_transform = tc->GetTransform4x4f();
					m_yaw_pith_roll = tc->GetYawPitchRoll3f();
					m_yaw_pith_roll.x = DirectX::XMConvertToDegrees(m_yaw_pith_roll.x);
					m_yaw_pith_roll.y = DirectX::XMConvertToDegrees(m_yaw_pith_roll.y);
					m_yaw_pith_roll.z = DirectX::XMConvertToDegrees(m_yaw_pith_roll.z);
					m_scale = tc->GetScale3f();
				}
				else {
					m_transform_exists = false;
					DirectX::XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity());
					m_yaw_pith_roll = { 0.0f, 0.0f, 0.0f };
					m_scale = { 1.0f, 1.0f, 1.0f };
				}

				componentId = ActorComponent::GetIdFromName("LightRenderComponent");
				std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
				if (lc) {
					m_light_exists = true;
					const LightProperties& lp = lc->GetLight();
					m_ambient = lp.m_Ambient;
					m_diffuse = lp.m_Diffuse;
					m_specular = lp.m_Specular;
					m_const = lp.m_Attenuation[0];
					m_linear = lp.m_Attenuation[1];
					m_exp = lp.m_Attenuation[2];
					m_Range = lp.m_Range;
					m_Spot = lp.m_Spot;
				}
				else {
					m_light_exists = false;
					m_ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
					m_diffuse = { 0.0f, 0.0f, 0.0f, 1.0f };
					m_specular = { 0.0f, 0.0f, 0.0f, 1.0f };
					m_const = 0.0f;
					m_linear = 0.0f;
					m_exp = 0.0f;
					m_Range = 0.0f;
					m_Spot = 0.0f;
				}
				std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
				if (pc) {
					m_particle_exists = true;
					Particle& p = pc->VGetParticle();
					m_damping = p.getDamping();
					m_radius = p.getRadius();
					m_mass = p.getMass();
				}
				else {
					m_particle_exists = false;
					m_damping = 1.0f;
					m_radius = 1.0f;
					m_mass = 1.0f;
				}
			}
			else {
				m_transform_exists = false;
				DirectX::XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity());
			}
		}
		else {
			std::shared_ptr<Actor> act = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_actor_id));
			if (act) {
				std::shared_ptr<TransformComponent> tc = MakeStrongPtr(act->GetComponent<TransformComponent>(ActorComponent::GetIdFromName("TransformComponent")));
				if (tc) {
					m_transform = tc->GetTransform4x4f();
				}

				if (m_transform_exists && ImGui::SliderFloat4("Transform row 1", ((float*)&m_transform) + 0, -8.0f, 8.0f)) {}
				if (m_transform_exists && ImGui::SliderFloat4("Transform row 2", ((float*)&m_transform) + 4, -8.0f, 8.0f)) {}
				if (m_transform_exists && ImGui::SliderFloat4("Transform row 3", ((float*)&m_transform) + 8, -8.0f, 8.0f)) {}
				if (m_transform_exists && ImGui::SliderFloat4("Transform row 4", ((float*)&m_transform) + 12, -8.0f, 8.0f)) {
					std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
					if (pc) {
						m_particle_exists = true;
						m_transform_exists = true;
						pc->VGetParticle().setPosition(m_transform._41, m_transform._42, m_transform._43);
					}
					else if (tc) {
						m_transform_exists = true;
						tc->SetTransform(m_transform);
					}

				}
				if (m_transform_exists && ImGui::SliderFloat3("Scale", ((float*)&m_scale), 0.0f, 3.0f)) {
					if (tc) {
						m_transform_exists = true;
						tc->SetScale3f(m_scale);
					}
				}
				if (m_transform_exists && ImGui::SliderFloat3("Yaw Pith Roll", ((float*)&m_yaw_pith_roll), -180.0f, 180.0f)) {
					if (tc) {
						m_transform_exists = true;
						tc->SetYawPitchRollDeg3f(m_yaw_pith_roll);
					}
				}
				if (m_light_exists && ImGui::ColorEdit4("Ambient", ((float*)&m_ambient))) {

					unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
					std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
					if (lc) {
						m_light_exists = true;
						lc->SetAmbient4f(m_ambient);
					}

				}
				if (m_light_exists && ImGui::ColorEdit4("Diffuse", ((float*)&m_diffuse))) {

					unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
					std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
					if (lc) {
						m_light_exists = true;
						lc->SetDiffuse4f(m_diffuse);
					}

				}
				if (m_light_exists && ImGui::ColorEdit4("Specular", ((float*)&m_specular))) {

					unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
					std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
					if (lc) {
						m_light_exists = true;
						lc->SetSpecular4f(m_specular);
					}

				}
				if (m_light_exists && ImGui::SliderFloat("Const", ((float*)&m_const), 0.0f, 5.0f)) {

					unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
					std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
					if (lc) {
						m_light_exists = true;
						lc->SetAttenuation(m_const, m_linear, m_exp);
					}

				}
				if (m_light_exists && ImGui::SliderFloat("Linear", ((float*)&m_linear), 0.0f, 5.0f)) {

					unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
					std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
					if (lc) {
						m_light_exists = true;
						lc->SetAttenuation(m_const, m_linear, m_exp);
					}

				}
				if (m_light_exists && ImGui::SliderFloat("Exp", ((float*)&m_exp), 0.0f, 5.0f)) {

					unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
					std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
					if (lc) {
						m_light_exists = true;
						lc->SetAttenuation(m_const, m_linear, m_exp);
					}

				}
				if (m_light_exists && ImGui::SliderFloat("Range", ((float*)&m_Range), 0.0f, 5.0f)) {

					unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
					std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
					if (lc) {
						m_light_exists = true;
						lc->SetRange(m_Range);
					}

				}
				if (m_light_exists && ImGui::SliderFloat("Spot", ((float*)&m_Spot), 0.0f, 5.0f)) {

					unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
					std::shared_ptr<LightRenderComponent> lc = MakeStrongPtr(act->GetComponent<LightRenderComponent>(componentId));
					if (lc) {
						m_light_exists = true;
						lc->SetRange(m_Spot);
					}

				}
				if (m_particle_exists && ImGui::SliderFloat("Damping", ((float*)&m_damping), 0.0f, 1.0f)) {
					m_particle_exists = true;
					std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
					if (pc) {
						m_particle_exists = true;
						pc->VGetParticle().setDamping(m_damping);
					}
				}
				if (m_particle_exists && ImGui::SliderFloat("Radius", ((float*)&m_radius), 0.0f, 1.0f)) {
					m_particle_exists = true;
					std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
					if (pc) {
						m_particle_exists = true;
						pc->VGetParticle().setRadius(m_radius);
					}
				}
				if (m_particle_exists && ImGui::SliderFloat("Mass", ((float*)&m_mass), 0.0f, 1000.0f)) {
					m_particle_exists = true;
					std::shared_ptr<ParticleComponent> pc = MakeStrongPtr(act->GetComponent<ParticleComponent>(ActorComponent::GetIdFromName("ParticleComponent")));
					if (pc) {
						m_particle_exists = true;
						pc->VGetParticle().setMass(m_mass);
					}
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




