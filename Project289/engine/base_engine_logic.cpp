#include "base_engine_logic.h"
#include "../events/i_event_manager.h"
#include "../events/evt_data_request_destroy_actor.h"
#include "../events/evt_data_request_new_actor.h"
#include "../events/evt_data_destroy_actor.h"
#include "../events/evt_data_new_actor.h"
#include "../events/evt_data_environment_loaded.h"
#include "../events/evt_data_move_actor.h"
#include "../actors/actor_factory.h"
#include "i_engine_view.h"
#include "engine.h"

BaseEngineLogic::BaseEngineLogic() {
	m_last_actor_id = 0;
	m_life_time = 0;
	m_process_manager = std::make_unique<ProcessManager>();
	m_random.Randomize();
	m_state = BaseEngineState::BGS_Initializing;
	m_render_diagnostics = false;
	m_expected_players = 0;
	m_human_players_attached = 0;
	m_human_games_loaded = 0;
	m_actor_factory = nullptr;

	m_level_manager = std::make_unique<LevelManager>();
	m_level_manager->Initialize();
}

BaseEngineLogic::~BaseEngineLogic() {
	while (!m_game_views.empty()) {
		m_game_views.pop_front();
	}

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it) {
		it->second->Destroy();
	}
	m_actors.clear();
	m_actors_names.clear();

	IEventManager::Get()->VRemoveListener({ connect_arg<&BaseEngineLogic::RequestDestroyActorDelegate>, this }, EvtData_Request_Destroy_Actor::sk_EventType);
}

bool BaseEngineLogic::Init() {
	m_actor_factory = VCreateActorFactory();

	IEventManager::Get()->VAddListener({ connect_arg<&BaseEngineLogic::RequestDestroyActorDelegate>, this }, EvtData_Request_Destroy_Actor::sk_EventType);

	return true;
}

ActorId BaseEngineLogic::GetNewActorID() {
	return ++m_last_actor_id;
}

MTRandom& BaseEngineLogic::GetRNG() {
	return m_random;
}

void BaseEngineLogic::VAddView(std::shared_ptr<IEngineView> pView, ActorId actorId) {
	int viewId = static_cast<int>(m_game_views.size());
	m_game_views.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnRestore();
}

void BaseEngineLogic::VRemoveView(std::shared_ptr<IEngineView> pView) {
	m_game_views.remove(pView);
}

StrongActorPtr BaseEngineLogic::VCreateActor(const std::string& actorResource, TiXmlElement* overrides, DirectX::FXMMATRIX initialTransform, const ActorId serversActorId) {
	DirectX::XMFLOAT4X4 t;
	DirectX::XMStoreFloat4x4(&t, initialTransform);
	return VCreateActor(actorResource, overrides, &t, serversActorId);
}

StrongActorPtr BaseEngineLogic::VCreateActor(const std::string& actorResource, TiXmlElement* overrides, const DirectX::XMFLOAT4X4* initialTransform, const ActorId serversActorId) {
	StrongActorPtr pActor = m_actor_factory->CreateActor(actorResource, overrides, initialTransform, serversActorId);
	if (pActor) {
		m_actors.insert(std::make_pair(pActor->GetId(), pActor));
		if (pActor->GetName() != "NoName") { m_actors_names.insert(std::make_pair(pActor->GetName(), pActor)); }
		return pActor;
	}
	else {
		return StrongActorPtr();
	}
}

void BaseEngineLogic::VDestroyActor(const ActorId actorId) {
	std::shared_ptr<EvtData_Destroy_Actor> pEvent(new EvtData_Destroy_Actor(actorId));
	IEventManager::Get()->VTriggerEvent(pEvent);

	auto findIt = m_actors.find(actorId);
	if (findIt != m_actors.end()) {
		findIt->second->Destroy();
		if (findIt->second->GetName() != "NoName") {
			m_actors_names.erase(findIt->second->GetName());
		}
		m_actors.erase(findIt);
	}
}

WeakActorPtr BaseEngineLogic::VGetActor(const ActorId actorId) {
	ActorMap::iterator findIt = m_actors.find(actorId);
	if (findIt != m_actors.end()) {
		return findIt->second;
	}
	return WeakActorPtr();
}

WeakActorPtr BaseEngineLogic::VGetActorByName(const std::string& actor_name) {
	auto findIt = m_actors_names.find(actor_name);
	if (findIt != m_actors_names.end()) {
		return findIt->second;
	}
	return WeakActorPtr();
}

void BaseEngineLogic::VModifyActor(const ActorId actorId, TiXmlElement* overrides) {
	if (!m_actor_factory) { return; }

	auto findIt = m_actors.find(actorId);
	if (findIt != m_actors.end()) {
		m_actor_factory->ModifyActor(findIt->second, overrides);
	}
}

void BaseEngineLogic::VMoveActor(const ActorId id, DirectX::FXMMATRIX mat) {}

void BaseEngineLogic::VMoveActor4x4f(const ActorId id, const DirectX::XMFLOAT4X4& mat) {}

std::string BaseEngineLogic::GetActorXml(const ActorId id) {
	StrongActorPtr pActor = MakeStrongPtr(VGetActor(id));
	if (pActor) {
		return pActor->ToXML();
	}

	return std::string();
}

const LevelManager* BaseEngineLogic::GetLevelManager() {
	return m_level_manager.get();
}

bool BaseEngineLogic::VLoadGame(const char* levelResource) {
	TiXmlDocument xml;
	xml.LoadFile(levelResource);

	TiXmlElement* pRoot = xml.RootElement();
	if (!pRoot) {
		return false;
	}

	TiXmlElement* pActorsNode = pRoot->FirstChildElement("Actors");
	if (pActorsNode) {
		for (TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
			const char* actorResource = pNode->Attribute("resource");

			StrongActorPtr pActor = VCreateActor(actorResource, pNode, nullptr);
			if (pActor) {
				std::shared_ptr<EvtData_New_Actor> pNewActorEvent(new EvtData_New_Actor(pActor->GetId()));
				IEventManager::Get()->VQueueEvent(pNewActorEvent);
			}
		}
	}

	for (auto it = m_game_views.begin(); it != m_game_views.end(); ++it) {
		std::shared_ptr<IEngineView> pView = *it;
		if (pView->VGetType() == EngineViewType::GameView_Human) {
			std::shared_ptr<HumanView> pHumanView = std::static_pointer_cast<HumanView, IEngineView>(pView);
			pHumanView->LoadGame(pRoot);
		}
	}

	if (!VLoadGameDelegate(pRoot)) { return false; }

	//std::shared_ptr<EvtData_Environment_Loaded> pNewGameEvent(new EvtData_Environment_Loaded);
	//IEventManager::Get()->VTriggerEvent(pNewGameEvent);

	return true;
}

bool BaseEngineLogic::VLoadGame(const char* levelResource, std::shared_ptr<HumanView> pHumanView) {
	TiXmlDocument xml;
	xml.LoadFile(levelResource);

	TiXmlElement* pRoot = xml.RootElement();
	if (!pRoot) {
		return false;
	}

	TiXmlElement* pActorsNode = pRoot->FirstChildElement("Actors");
	if (pActorsNode) {
		for (TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement()) {
			const char* actorResource = pNode->Attribute("resource");

			StrongActorPtr pActor = VCreateActor(actorResource, pNode, nullptr);
			if (pActor) {
				std::shared_ptr<EvtData_New_Actor> pNewActorEvent(new EvtData_New_Actor(pActor->GetId()));
				IEventManager::Get()->VQueueEvent(pNewActorEvent);
			}
		}
	}

	pHumanView->LoadGame(pRoot);
	if (!VLoadGameDelegate(pRoot)) { return false; }

	return true;
}

void BaseEngineLogic::VOnUpdate(float time, float elapsedTime) {
	m_life_time += elapsedTime;
	m_process_manager->UpdateProcesses(elapsedTime);

	switch (m_state) {
		case BaseEngineState::BGS_Initializing: {
			VChangeState(BaseEngineState::BGS_MainMenu);
		}
		break;

		case BaseEngineState::BGS_MainMenu: {}
		break;

		case BaseEngineState::BGS_LoadingGameEnvironment: {
		}
		break;

		case BaseEngineState::BGS_Running: {
			if (elapsedTime > 0.016f) { m_physics->VOnUpdate(0.016f); }
			else { m_physics->VOnUpdate(elapsedTime); }
			m_physics->VSyncVisibleScene();
		}
		break;
	}

	for (GameViewList::iterator it = m_game_views.begin(); it != m_game_views.end(); ++it) 	{
		(*it)->VOnUpdate(elapsedTime);
	}

	for (ActorMap::const_iterator it = m_actors.begin(); it != m_actors.end(); ++it)     {
		it->second->Update(elapsedTime);
	}
}

void BaseEngineLogic::VChangeState(BaseEngineState newState) {
	if (newState == BaseEngineState::BGS_MainMenu) {
		m_state = newState;
		if (!g_pApp->VLoadGame()) {
			g_pApp->AbortGame();
		}
	}

	m_state = newState;
}

const BaseEngineState BaseEngineLogic::GetState() const {
	return m_state;
}

IEnginePhysics* BaseEngineLogic::VGetGamePhysics() {
	return m_physics.get();
}

void BaseEngineLogic::AttachProcess(StrongProcessPtr pProcess) {
	if (m_process_manager) {
		m_process_manager->AttachProcess(pProcess);
	}
}

void BaseEngineLogic::RequestDestroyActorDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_Request_Destroy_Actor> pCastEventData = std::static_pointer_cast<EvtData_Request_Destroy_Actor>(pEventData);
	VDestroyActor(pCastEventData->GetActorId());
}

std::unique_ptr<ActorFactory> BaseEngineLogic::VCreateActorFactory() {
	return std::make_unique<ActorFactory>();
}

bool BaseEngineLogic::VLoadGameDelegate(TiXmlElement* pLevelData) {
	return true;
}

void BaseEngineLogic::MoveActorDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_Move_Actor> pCastEventData = std::static_pointer_cast<EvtData_Move_Actor>(pEventData);
	VMoveActor(pCastEventData->GetId(), pCastEventData->GetMatrix());
}

void BaseEngineLogic::RequestNewActorDelegate(IEventDataPtr pEventData) {
	std::shared_ptr<EvtData_Request_New_Actor> pCastEventData = std::static_pointer_cast<EvtData_Request_New_Actor>(pEventData);

	StrongActorPtr pActor = VCreateActor(pCastEventData->GetActorResource(), nullptr, pCastEventData->GetInitialTransform(), pCastEventData->GetServerActorId());
	if (pActor) {
		std::shared_ptr<EvtData_New_Actor> pNewActorEvent(new EvtData_New_Actor(pActor->GetId(), pCastEventData->GetViewId()));
		IEventManager::Get()->VQueueEvent(pNewActorEvent);
	}
}
