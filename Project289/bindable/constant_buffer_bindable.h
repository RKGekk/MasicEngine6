#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "Bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

template <typename T>
class ConstantBufferBindable : public Bindable {
public:
	ConstantBufferBindable(ID3D11Device* device, const T& consts) {
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		HRESULT hr = device->CreateBuffer(&cbd, &csd, &pConstantBuffer);
		COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");
	}

	ConstantBufferBindable(ID3D11Device* device) {
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0u;
		HRESULT hr = device->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
		COM_ERROR_IF_FAILED(hr, "Failed to create constant buffer");
	}

	void Update(ID3D11DeviceContext* deviceContext, const T& consts) {
		D3D11_MAPPED_SUBRESOURCE msr;
		HRESULT hr = deviceContext->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer");
		memcpy(msr.pData, &consts, sizeof(consts));
		deviceContext->Unmap(pConstantBuffer.Get(), 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};