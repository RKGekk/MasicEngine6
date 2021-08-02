#pragma once

#include <DirectXMath.h>

#include "light_node.h"
#include "../tools/memory_utility.h"

class D3DLightNode11 : public LightNode {
public:
	D3DLightNode11(WeakBaseRenderComponentPtr renderComponent, const LightProperties& lightProps, const DirectX::XMFLOAT4X4* t);
	D3DLightNode11(WeakBaseRenderComponentPtr renderComponent, const LightProperties& lightProps, DirectX::FXMMATRIX to, DirectX::CXMMATRIX from, bool calulate_from = false);

	virtual HRESULT VOnRestore(Scene* pScene) override;
	virtual HRESULT VOnUpdate(Scene*, float elapsedSeconds) override;
};
