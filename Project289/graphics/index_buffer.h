#pragma once

#include <memory>
#include <vector>
#include <wrl.h>
#include <d3d11.h>

class IndexBuffer {
public:
	IndexBuffer() {}
	IndexBuffer(const IndexBuffer& rhv) {
		m_buffer = rhv.m_buffer;
		m_buffer_size = rhv.m_buffer_size;
	};

	IndexBuffer& operator=(const IndexBuffer rhv) {
		if (this == &rhv) { return *this; }
		m_buffer = rhv.m_buffer;
		m_buffer_size = rhv.m_buffer_size;
		return *this;
	}

	ID3D11Buffer* Get() {
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() {
		return m_buffer.GetAddressOf();
	}

	UINT IndexCount() const {
		return m_buffer_size;
	}

	HRESULT Initialize(ID3D11Device* device, const std::vector<DWORD>& data) {
		if (m_buffer.Get() != nullptr) { m_buffer.Reset(); }

		m_buffer_size = (UINT)data.size();

		D3D11_BUFFER_DESC bdi;
		ZeroMemory(&bdi, sizeof(bdi));
		bdi.Usage = D3D11_USAGE_DEFAULT;
		bdi.ByteWidth = UINT(sizeof(DWORD) * data.size());
		bdi.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bdi.CPUAccessFlags = 0u;
		bdi.MiscFlags = 0u;
		bdi.StructureByteStride = sizeof(DWORD);

		D3D11_SUBRESOURCE_DATA sdi;
		ZeroMemory(&sdi, sizeof(sdi));
		sdi.pSysMem = data.data();
		return device->CreateBuffer(&bdi, &sdi, m_buffer.GetAddressOf());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_buffer_size = 0;
};