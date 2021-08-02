#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>

#include "bindable.h"
#include "../tools/memory_utility.h"
#include "../tools/com_exception.h"

class VertexBufferBindable : public Bindable {
public:

	template<class V>
	VertexBufferBindable(ID3D11Device* device, const std::vector<V>& vertices) : stride(sizeof(V)) {
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();

		HRESULT hr = device->CreateBuffer(&bd, &sd, &pVertexBuffer);
		COM_ERROR_IF_FAILED(hr, "Failed to create vertex buffer");
	}

	void Bind(ID3D11DeviceContext* deviceContext) override;

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};