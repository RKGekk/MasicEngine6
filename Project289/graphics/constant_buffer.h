#pragma once

#include <wrl.h>
#include <d3d11.h>

#include "../tools/error_logger.h"

template<class T>
class ConstantBuffer {
public:
	ConstantBuffer(const ConstantBuffer<T>& rhs) = delete;
	ConstantBuffer() {}

	T data;

	ID3D11Buffer* Get() const {
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() {
		return m_buffer.GetAddressOf();
	}

	HRESULT Initialize(ID3D11Device* device) {
		if (m_buffer.Get() != nullptr) { m_buffer.Reset(); }

		size_t sz = sizeof(T);
		D3D11_BUFFER_DESC cb_desc;
		ZeroMemory(&cb_desc, sizeof(cb_desc));
		cb_desc.Usage = D3D11_USAGE_DYNAMIC;
		cb_desc.ByteWidth = UINT(sz + (16 - (sz % 16)));
		cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb_desc.MiscFlags = 0u;
		cb_desc.StructureByteStride = 0u;
		return device->CreateBuffer(&cb_desc, 0, m_buffer.GetAddressOf());
	}

	bool ApplyChanges(ID3D11DeviceContext* deviceContext) {
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		HRESULT hr = deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
		if (FAILED(hr)) {
			ErrorLogger::Log(hr, "Failed to map constant buffer.");
			return false;
		}
		CopyMemory(mapped_resource.pData, &data, sizeof(T));
		deviceContext->Unmap(m_buffer.Get(), 0);
		return true;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
};