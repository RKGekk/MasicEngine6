#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

class SamplerStateBindable : public Bindable {
public:
	SamplerStateBindable(ID3D11Device* device);
	SamplerStateBindable(ID3D11Device* device, const D3D11_SAMPLER_DESC& samplerDesc);
	void Bind(ID3D11DeviceContext* deviceContext) override;
protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampleState;
};