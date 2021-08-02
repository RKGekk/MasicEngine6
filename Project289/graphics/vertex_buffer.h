#pragma once

#include <memory>
#include <vector>
#include <wrl.h>
#include <d3d11.h>

template<class T>
class VertexBuffer {
public:
	VertexBuffer() {};

	VertexBuffer(const VertexBuffer<T>& rhv) {
		m_buffer = rhv.m_buffer;
		m_buffer_size = rhv.m_buffer_size;
		m_stride = rhv.m_stride;
	};

	VertexBuffer<T>& operator=(const VertexBuffer<T> rhv) {
		if (this == &rhv) { return *this; }
		m_buffer = rhv.m_buffer;
		m_buffer_size = rhv.m_buffer_size;
		m_stride = rhv.m_stride;
		return *this;
	}

	ID3D11Buffer* Get() const {
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() {
		return m_buffer.GetAddressOf();
	}

	UINT VertexCount() const {
		return m_buffer_size;
	}

	UINT Stride() const {
		return m_stride;
	}

	const UINT* StridePtr() {
		return &m_stride;
	}

	HRESULT Initialize(ID3D11Device* device, const std::vector<T>& data) {
		if (m_buffer.Get() != nullptr) { m_buffer.Reset(); }

		m_buffer_size = (UINT)data.size();
		m_stride = sizeof(T);

		D3D11_BUFFER_DESC bdv;
		ZeroMemory(&bdv, sizeof(bdv));
		bdv.Usage = D3D11_USAGE_DEFAULT;
		bdv.ByteWidth = UINT(m_stride * m_buffer_size);
		bdv.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bdv.CPUAccessFlags = 0u;
		bdv.MiscFlags = 0u;
		bdv.StructureByteStride = m_stride;

		D3D11_SUBRESOURCE_DATA sdv;
		ZeroMemory(&sdv, sizeof(sdv));
		sdv.pSysMem = data.data();
		return device->CreateBuffer(&bdv, &sdv, m_buffer.GetAddressOf());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_stride = 0;
	UINT m_buffer_size = 0;
};