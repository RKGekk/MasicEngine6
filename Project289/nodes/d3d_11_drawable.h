#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <d3d11.h>
#include <DirectXMath.h>

#include "scene_node.h"
#include "../bindable/bindable.h"
#include "../bindable/index_buffer_bindable.h"

class D3D11Drawable : public SceneNode {
public:
	D3D11Drawable(BaseRenderComponent* renderComponent, const DirectX::XMFLOAT4X4* pMatrix);
	D3D11Drawable(BaseRenderComponent* renderComponent, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from = false);
	virtual ~D3D11Drawable();

	virtual HRESULT VRender(Scene* pScene) override;

	virtual void AddBind(BindableType key, std::unique_ptr<Bindable> bind);

protected:
	std::unordered_map<BindableType, std::unique_ptr<Bindable>> m_binds;
};