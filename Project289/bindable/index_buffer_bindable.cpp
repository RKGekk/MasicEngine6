#include "index_buffer_bindable.h"

IndexBufferBindable::IndexBufferBindable(ID3D11Device* device, const std::vector<DWORD>& indices) : count((UINT)indices.size()) {

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(count * sizeof(DWORD));
	ibd.StructureByteStride = sizeof(DWORD);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();

	HRESULT hr = device->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	COM_ERROR_IF_FAILED(hr, "Failed to create index buffer");
}

void IndexBufferBindable::Bind(ID3D11DeviceContext* deviceContext) {
	deviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

UINT IndexBufferBindable::GetCount() const {
	return count;
}