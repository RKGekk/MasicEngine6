#pragma once

#include <memory>

#include "human_view.h"
#include "movement_controller.h"
#include "actor_menu_ui.h"

class XHumanView : public HumanView {
private:
	static const std::string g_Name;

protected:
	bool m_bShowUI;
	bool m_bShowDebugUI;
	std::wstring m_gameplayText;
	std::shared_ptr<ActorMenuUI> m_ActorMenuUI;

	std::shared_ptr<MovementController> m_pFreeCameraController;
	std::shared_ptr<SceneNode> m_pTeapot;

public:
	XHumanView(IRenderer* renderer);
	virtual ~XHumanView();

	virtual LRESULT CALLBACK VOnMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void VRenderText() override;
	virtual void VOnUpdate(float deltaSeconds) override;
	virtual void VOnAttach(EngineViewId vid, ActorId aid) override;

	virtual void VSetControlledActor(ActorId actorId) override;
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) override;

	void GameplayUiUpdateDelegate(IEventDataPtr pEventData);
	void SetControlledActorDelegate(IEventDataPtr pEventData);

	virtual const std::string& VGetName() override;

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();
};