#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

class RasterizerStateBindable : public Bindable {
public:
	RasterizerStateBindable(ID3D11Device* device);
	RasterizerStateBindable(ID3D11Device* device, const D3D11_RASTERIZER_DESC& rasterDesc);
	void Bind(ID3D11DeviceContext* deviceContext) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
};