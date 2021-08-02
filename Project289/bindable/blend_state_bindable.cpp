#include "blend_state_bindable.h"

BlendStateBindable::BlendStateBindable(ID3D11Device* device) {

	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = false;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr = device->CreateBlendState(&alphaToCoverageDesc, &m_blendState);
	COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");
}

BlendStateBindable::BlendStateBindable(ID3D11Device* device, const D3D11_BLEND_DESC& blendDesc) {
	HRESULT hr = device->CreateBlendState(&blendDesc, m_blendState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");
}

void BlendStateBindable::Bind(ID3D11DeviceContext* deviceContext) {
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);
}
