#include "x_logic.h"
#include "x_physics.h"
#include "x_human_view.h"
#include "../tools/memory_utility.h"
#include "../actors/transform_component.h"
#include "../actors/physics_component.h"
#include "../actors/particle_component.h"
#include "../actors/spawn_relation_component.h"
#include "../actors/spawn_component.h"
#include "../actors/character_stats_component.h"
#include "../events/evt_data_destroy_actor.h"
#include "../events/evt_data_new_actor.h"
#include "../events/evt_data_start_thrust.h"
#include "../events/evt_data_end_thrust.h"
#include "../events/evt_data_move_actor.h"
#include "../events/evt_data_request_start_game.h"
#include "../events/evt_data_environment_loaded.h"
#include "../events/evt_data_sphere_particle_contact.h"
#include "../events/i_event_manager.h"
#include "../engine/engine.h"
#include "../processes/exec_process.h"
#include "../processes/delay_process.h"
#include "../events/evt_data_request_destroy_actor.h"
#include "../actors/enemy_component.h"
#include "../actors/drop_component.h"

XLogic::XLogic() {
	
	m_physics = std::make_unique<XPhysics>();
	m_physics->VInitialize();

	RegisterAllDelegates();
}

XLogic::~XLogic() {
	RemoveAllDelegates();
}

void XLogic::VMoveActor(const ActorId id, DirectX::FXMMATRIX mat) {
    BaseEngineLogic::VMoveActor(id, mat);

    StrongActorPtr pActor = MakeStrongPtr(VGetActor(id));
    if (pActor) {
        std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
    }
}

void XLogic::VMoveActor4x4f(const ActorId id, const DirectX::XMFLOAT4X4& mat) {
    VMoveActor(id, DirectX::XMLoadFloat4x4(&mat));
}

void XLogic::VChangeState(BaseEngineState newState) {
	BaseEngineLogic::VChangeState(newState);

	switch (newState) {
		case BaseEngineState::BGS_LoadingGameEnvironment: {

			std::shared_ptr<ExecProcess> execOne = std::make_shared<ExecProcess>([]() {
				auto mainMenu = g_pApp->GetHumanView();
				mainMenu->VActivateScene(false);
				return true;
			});
			std::shared_ptr<DelayProcess> delay = std::make_shared<DelayProcess>(2.0f, [](float dt, float tt, float n) {
				return true;
			});
			std::shared_ptr<ExecProcess> exec2 = std::make_shared<ExecProcess>([]() {
				std::shared_ptr<HumanView> gameView(new XHumanView(g_pApp->GetRenderer()));
				g_pApp->GetGameLogic()->VLoadGame("World.xml", gameView);
				gameView->VCanDraw(false);

				g_pApp->GetGameLogic()->VAddView(gameView);
				return true;
			});
			std::shared_ptr<ExecProcess> exec3 = std::make_shared<ExecProcess>([]() {
				g_pApp->GetTimer().Reset();
				std::shared_ptr<EvtData_Environment_Loaded> pEvent(new EvtData_Environment_Loaded());
				IEventManager::Get()->VTriggerEvent(pEvent);
				return true;
			});
			execOne->AttachChild(delay);
			delay->AttachChild(exec2);
			exec2->AttachChild(exec3);
			m_process_manager->AttachProcess(execOne);

			m_state = BaseEngineState::BGS_LoadingGameEnvironment;
		}
		break;
		case BaseEngineState::BGS_Running : {
			m_state = BaseEngineState::BGS_Running;
		}
		break;
	}
}

void XLogic::VAddView(std::shared_ptr<IEngineView> pView, ActorId actorId) {
	BaseEngineLogic::VAddView(pView, actorId);
	if (std::dynamic_pointer_cast<XHumanView>(pView)) {
		m_human_players_attached++;
	}
}

IEnginePhysics* XLogic::VGetGamePhysics() {
	return m_physics.get();
}

void XLogic::RequestStartGameDelegate(IEventDataPtr pEventData) {}

void XLogic::EnvironmentLoadedDelegate(IEventDataPtr pEventData) {
	VChangeState(BaseEngineState::BGS_Running);

	std::shared_ptr<ExecProcess> exec1 = std::make_shared<ExecProcess>([]() {
		StrongActorPtr pActorLoading = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("loading"));
		std::shared_ptr<EvtData_Request_Destroy_Actor> pNewGameEvent(new EvtData_Request_Destroy_Actor(pActorLoading->GetId()));
		IEventManager::Get()->VTriggerEvent(pNewGameEvent);

		return true;
	});

	std::shared_ptr<ExecProcess> exec2 = std::make_shared<ExecProcess>([]() {
		std::shared_ptr<HumanView> menuView = g_pApp->GetHumanViewByName("MainMenu");
		menuView->VCanDraw(false);

		std::shared_ptr<HumanView> levelView = g_pApp->GetHumanViewByName("Level");
		levelView->VCanDraw(true);

		g_pApp->GetGameLogic()->VRemoveView(menuView);

		StrongActorPtr pActorTeapot = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("pers"));
		levelView->VSetControlledActor(pActorTeapot->GetId());

		return true;
	});
	exec1->AttachChild(exec2);
	m_process_manager->AttachProcess(exec1);
	++m_human_games_loaded;
}

void XLogic::ThrustDelegate(IEventDataPtr pEventData) {}

void XLogic::SteerDelegate(IEventDataPtr pEventData) {}

void XLogic::StartThrustDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_StartThrust> pCastEventData = std::static_pointer_cast<EvtData_StartThrust>(pEventData);
	StrongActorPtr pActor = MakeStrongPtr(VGetActor(pCastEventData->GetActorId()));
	if (pActor) {
		std::shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(pActor->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name));
		if (pPhysicalComponent)         {
			pPhysicalComponent->ApplyAcceleration(pCastEventData->GetAcceleration());
		}
	}
}

void XLogic::EndThrustDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_EndThrust> pCastEventData = std::static_pointer_cast<EvtData_EndThrust>(pEventData);
	StrongActorPtr pActor = MakeStrongPtr(VGetActor(pCastEventData->GetActorId()));
	if (pActor) {
		std::shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(pActor->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name));
		if (pPhysicalComponent) {
			pPhysicalComponent->RemoveAcceleration();
		}
	}
}

void XLogic::StartSteerDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_StartThrust> pCastEventData = std::static_pointer_cast<EvtData_StartThrust>(pEventData);
	StrongActorPtr pActor = MakeStrongPtr(VGetActor(pCastEventData->GetActorId()));
	if (pActor) {
		std::shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(pActor->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name));
		if (pPhysicalComponent) {
			pPhysicalComponent->ApplyAngularAcceleration(pCastEventData->GetAcceleration());
		}
	}
}

void XLogic::EndSteerDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_StartThrust> pCastEventData = std::static_pointer_cast<EvtData_StartThrust>(pEventData);
	StrongActorPtr pActor = MakeStrongPtr(VGetActor(pCastEventData->GetActorId()));
	if (pActor) {
		std::shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(pActor->GetComponent<PhysicsComponent>(PhysicsComponent::g_Name));
		if (pPhysicalComponent) {
			pPhysicalComponent->RemoveAngularAcceleration();
		}
	}
}

void XLogic::SphereParticleContactDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_Sphere_Particle_Contact> pCastEventData = std::static_pointer_cast<EvtData_Sphere_Particle_Contact>(pEventData);
	StrongActorPtr pActor1 = MakeStrongPtr(VGetActor(pCastEventData->GetActorId1()));
	StrongActorPtr pActor2 = MakeStrongPtr(VGetActor(pCastEventData->GetActorId2()));
	std::shared_ptr<EnemyComponent> ec1 = MakeStrongPtr(pActor1->GetComponent<EnemyComponent>(ActorComponent::GetIdFromName("EnemyComponent")));
	std::shared_ptr<EnemyComponent> ec2 = MakeStrongPtr(pActor2->GetComponent<EnemyComponent>(ActorComponent::GetIdFromName("EnemyComponent")));
	std::shared_ptr<CharacterStatsComponent> pCharacterStatsComponent1 = MakeStrongPtr(pActor1->GetComponent<CharacterStatsComponent>(CharacterStatsComponent::g_Name));
	std::shared_ptr<CharacterStatsComponent> pCharacterStatsComponent2 = MakeStrongPtr(pActor2->GetComponent<CharacterStatsComponent>(CharacterStatsComponent::g_Name));
	std::shared_ptr<DropComponent> dc1 = MakeStrongPtr(pActor1->GetComponent<DropComponent>(DropComponent::GetIdFromName("DropComponent")));
	std::shared_ptr<DropComponent> dc2 = MakeStrongPtr(pActor2->GetComponent<DropComponent>(DropComponent::GetIdFromName("DropComponent")));

	if (ec1 && !ec2) {
		if (pActor2->GetName() == "pers") {
			int current_drops = pCharacterStatsComponent2->GetDrops();
			if (current_drops > 0) {
				std::shared_ptr<SpawnRelationComponent> rc = MakeStrongPtr(pActor1->GetComponent<SpawnRelationComponent>(ActorComponent::GetIdFromName("SpawnRelationComponent")));
				rc->Respawn();
				pCharacterStatsComponent2->ApplyDrops(-1);
			}
		}
		if (pActor2->GetName() == ec1->GetTargetName()) {
			std::shared_ptr<SpawnRelationComponent> rc = MakeStrongPtr(pActor1->GetComponent<SpawnRelationComponent>(ActorComponent::GetIdFromName("SpawnRelationComponent")));
			rc->Respawn();
			int current_health = pCharacterStatsComponent2->GetCurrentHealth();
			if (current_health > 0) {
				pCharacterStatsComponent2->ApplyHealth(-1);
			}
		}
	}

	if (ec2 && !ec1) {
		if (pActor1->GetName() == "pers") {
			int current_drops = pCharacterStatsComponent1->GetDrops();
			if (current_drops > 0) {
				std::shared_ptr<SpawnRelationComponent> rc = MakeStrongPtr(pActor2->GetComponent<SpawnRelationComponent>(ActorComponent::GetIdFromName("SpawnRelationComponent")));
				rc->Respawn();
				pCharacterStatsComponent1->ApplyDrops(-1);
			}
		}
		if (pActor1->GetName() == ec2->GetTargetName()) {
			std::shared_ptr<SpawnRelationComponent> rc = MakeStrongPtr(pActor2->GetComponent<SpawnRelationComponent>(ActorComponent::GetIdFromName("SpawnRelationComponent")));
			rc->Respawn();
			int current_health = pCharacterStatsComponent1->GetCurrentHealth();
			if (current_health > 0) {
				pCharacterStatsComponent1->ApplyHealth(-1);
			}
		}
	}

	if (dc1 && !dc2) {
		if (pActor2->GetName() == "pers") {
			std::shared_ptr<SpawnRelationComponent> rc = MakeStrongPtr(pActor1->GetComponent<SpawnRelationComponent>(ActorComponent::GetIdFromName("SpawnRelationComponent")));
			rc->Respawn();
			pCharacterStatsComponent2->ApplyDrops(1);
		}
	}

	if (dc2 && !dc1) {
		if (pActor1->GetName() == "pers") {
			std::shared_ptr<SpawnRelationComponent> rc = MakeStrongPtr(pActor2->GetComponent<SpawnRelationComponent>(ActorComponent::GetIdFromName("SpawnRelationComponent")));
			rc->Respawn();
			pCharacterStatsComponent1->ApplyDrops(1);
		}
	}

	/*
	if (pActor1->GetName() == "emenmy1") {
		std::shared_ptr<SpawnRelationComponent> rc = MakeStrongPtr(pActor1->GetComponent<SpawnRelationComponent>(ActorComponent::GetIdFromName("SpawnRelationComponent")));
		rc->Respawn();
	}
	else {
		std::shared_ptr<SpawnRelationComponent> rc = MakeStrongPtr(pActor2->GetComponent<SpawnRelationComponent>(ActorComponent::GetIdFromName("SpawnRelationComponent")));
		rc->Respawn();
	}*/
}

bool XLogic::VLoadGameDelegate(TiXmlElement* pLevelData) {
	return true;
}

void XLogic::RegisterAllDelegates() {
	IEventManager* pGlobalEventManager = IEventManager::Get();
	pGlobalEventManager->VAddListener({ connect_arg<&XLogic::MoveActorDelegate>, this }, EvtData_Move_Actor::sk_EventType);
	pGlobalEventManager->VAddListener({ connect_arg<&XLogic::RequestStartGameDelegate>, this }, EvtData_Request_Start_Game::sk_EventType);
	pGlobalEventManager->VAddListener({ connect_arg<&XLogic::EnvironmentLoadedDelegate>, this }, EvtData_Environment_Loaded::sk_EventType);
	pGlobalEventManager->VAddListener({ connect_arg<&XLogic::StartThrustDelegate>, this }, EvtData_StartThrust::sk_EventType);
	pGlobalEventManager->VAddListener({ connect_arg<&XLogic::EndThrustDelegate>, this }, EvtData_EndThrust::sk_EventType);
	pGlobalEventManager->VAddListener({ connect_arg<&XLogic::SphereParticleContactDelegate>, this }, EvtData_Sphere_Particle_Contact::sk_EventType);
}

void XLogic::RemoveAllDelegates() {
	
	IEventManager* pGlobalEventManager = IEventManager::Get();
	pGlobalEventManager->VRemoveListener({ connect_arg<&XLogic::MoveActorDelegate>, this }, EvtData_Move_Actor::sk_EventType);
	pGlobalEventManager->VRemoveListener({ connect_arg<&XLogic::RequestStartGameDelegate>, this }, EvtData_Request_Start_Game::sk_EventType);
	pGlobalEventManager->VRemoveListener({ connect_arg<&XLogic::EnvironmentLoadedDelegate>, this }, EvtData_Environment_Loaded::sk_EventType);
	pGlobalEventManager->VRemoveListener({ connect_arg<&XLogic::StartThrustDelegate>, this }, EvtData_StartThrust::sk_EventType);
	pGlobalEventManager->VRemoveListener({ connect_arg<&XLogic::EndThrustDelegate>, this }, EvtData_EndThrust::sk_EventType);
	pGlobalEventManager->VRemoveListener({ connect_arg<&XLogic::SphereParticleContactDelegate>, this }, EvtData_Sphere_Particle_Contact::sk_EventType);
}
