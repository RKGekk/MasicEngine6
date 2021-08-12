#include "d3d_light_node_11.h"
#include "../actors/light_render_component.h"
#include "../actors/transform_component.h"

D3DLightNode11::D3DLightNode11(WeakBaseRenderComponentPtr renderComponent, const LightProperties& lightProps, const DirectX::XMFLOAT4X4* t) : LightNode(renderComponent, lightProps, t) {}

D3DLightNode11::D3DLightNode11(WeakBaseRenderComponentPtr renderComponent, const LightProperties& lightProps, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from) : LightNode(renderComponent, lightProps, to, from, calulate_from) {}

HRESULT D3DLightNode11::VOnRestore(Scene* pScene) {
	return S_OK;
}

HRESULT D3DLightNode11::VOnUpdate(Scene*, float elapsedSeconds) {
	StrongActorPtr pActor = m_RenderComponent->GetOwner();

	unsigned int componentId = ActorComponent::GetIdFromName("LightRenderComponent");
	std::shared_ptr<LightRenderComponent> rc = MakeStrongPtr(pActor->GetComponent<LightRenderComponent>(componentId));
	m_light_props = rc->GetLight();

	std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
	this->VSetTransform(pTransformComponent->GetTransform(), DirectX::XMMatrixIdentity(), true);
	
	return S_OK;
}

