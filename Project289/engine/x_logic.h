#pragma once

#include <memory>

#include "base_engine_logic.h"
#include "i_engine_physics.h"
#include "base_engine_state.h"
#include "i_engine_view.h"

class XLogic : public BaseEngineLogic {
public:
    XLogic();
    virtual ~XLogic();

    virtual void VMoveActor(const ActorId id, DirectX::FXMMATRIX mat) override;
    virtual void VMoveActor4x4f(const ActorId id, const DirectX::XMFLOAT4X4& mat) override;

    virtual void VChangeState(BaseEngineState newState) override;
    virtual void VAddView(std::shared_ptr<IEngineView> pView, ActorId actorId = INVALID_ACTOR_ID) override;
    virtual IEnginePhysics* VGetGamePhysics() override;

    void RequestStartGameDelegate(IEventDataPtr pEventData);
    void EnvironmentLoadedDelegate(IEventDataPtr pEventData);
    void ThrustDelegate(IEventDataPtr pEventData);
    void SteerDelegate(IEventDataPtr pEventData);
    void StartThrustDelegate(IEventDataPtr pEventData);
    void EndThrustDelegate(IEventDataPtr pEventData);
    void StartSteerDelegate(IEventDataPtr pEventData);
    void EndSteerDelegate(IEventDataPtr pEventData);

protected:
    virtual bool VLoadGameDelegate(TiXmlElement* pLevelData);

private:
    void RegisterAllDelegates();
    void RemoveAllDelegates();
};
