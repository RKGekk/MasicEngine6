#include "vertex_buffer_bindable.h"

void VertexBufferBindable::Bind(ID3D11DeviceContext* deviceContext) {
	const UINT offset = 0u;
	deviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}