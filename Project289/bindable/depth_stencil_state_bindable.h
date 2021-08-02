#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

class DepthStencilStateBindable : public Bindable {
public:
	DepthStencilStateBindable(ID3D11Device* device);
	DepthStencilStateBindable(ID3D11Device* device, const D3D11_DEPTH_STENCIL_DESC& depthStencilDesc);
	void Bind(ID3D11DeviceContext* deviceContext) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depthStencilState;
};