#include "rasterizer_state_bindable.h"

RasterizerStateBindable::RasterizerStateBindable(ID3D11Device* device) {
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	//rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	HRESULT hr = device->CreateRasterizerState(&rasterDesc, m_rasterState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state");
}

RasterizerStateBindable::RasterizerStateBindable(ID3D11Device* device, const D3D11_RASTERIZER_DESC& rasterDesc) {
	HRESULT hr = device->CreateRasterizerState(&rasterDesc, m_rasterState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state");
}

void RasterizerStateBindable::Bind(ID3D11DeviceContext* deviceContext) {
	deviceContext->RSSetState(m_rasterState.Get());
}
